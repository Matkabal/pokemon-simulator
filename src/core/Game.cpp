#include "core/Game.hpp"
#include "scenes/MenuScene.hpp"
#include <stdexcept>
#include <chrono>
#include <d3dx12.h>
#include <string> // Necessário para std::wstring
#include "logger/Logger.hpp"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

Game::Game(HINSTANCE hInstance)
    : hInstance(hInstance),
      hwnd(nullptr),
      frameIndex(0),
      fenceValue(0),
      fenceEvent(nullptr),
      windowTitle(L"Pokemon Simulator") {} // <- inicializa aqui

Game::~Game() {
    cleanup();
}

bool Game::initialize() {
    bool iw = initWindow();
    bool id = initD3D();

    if (!iw || !id) {
        Logger::error("Error in initialize game initWindow={} e initD3D={}", iw, id);
        return false;
    }

    ID3D12Resource* rawRenderTargets[FrameCount];
    for (UINT i = 0; i < FrameCount; ++i) {
        rawRenderTargets[i] = renderTargets[i].Get();
    }

    sceneManager = std::make_unique<SceneManager>();
    sceneManager->changeScene(std::make_unique<MenuScene>(
        device.Get(),
        commandList.Get(),
        commandQueue.Get(),
        commandAllocator.Get(),
        rawRenderTargets,
        rtvHandles,
        frameIndex
    ));

    return true;
}

bool Game::initWindow() {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"DX12WindowClass";

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        0,
        L"DX12WindowClass",
        windowTitle.c_str(),  // <- aqui estava o erro se não declarado corretamente
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        nullptr, nullptr,
        hInstance, nullptr
    );

    if (!hwnd) return false;

    ShowWindow(hwnd, SW_SHOW);
    return true;
}

bool Game::initD3D() {
    UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
    Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
        debugController->EnableDebugLayer();
        dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
    }
#endif

    if (FAILED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&dxgiFactory)))) return false;
    if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)))) return false;

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue)))) return false;

    if (FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)))) return false;
    if (FAILED(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)))) return false;

    RECT rect;
    GetClientRect(hwnd, &rect);
    UINT w = rect.right - rect.left;
    UINT h = rect.bottom - rect.top;

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = FrameCount;
    swapChainDesc.Width = w;
    swapChainDesc.Height = h;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    Microsoft::WRL::ComPtr<IDXGISwapChain1> tempSwapChain;
    if (FAILED(dxgiFactory->CreateSwapChainForHwnd(commandQueue.Get(), hwnd, &swapChainDesc, nullptr, nullptr, &tempSwapChain)))
        return false;
    if (FAILED(tempSwapChain.As(&swapChain))) return false;

    frameIndex = swapChain->GetCurrentBackBufferIndex();

    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = FrameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    if (FAILED(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap)))) return false;

    rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT i = 0; i < FrameCount; ++i) {
        if (FAILED(swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i])))) return false;
        device->CreateRenderTargetView(renderTargets[i].Get(), nullptr, rtvHandle);
        rtvHandles[i] = rtvHandle;
        rtvHandle.Offset(1, rtvDescriptorSize);
    }

    if (FAILED(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)))) return false;
    fenceValue = 1;
    fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (!fenceEvent) return false;

    return true;
}

void Game::mainLoop() {
    MSG msg = {};
    bool running = true;
    Logger::info("mainLoop initiate");

    while (running) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            Logger::info("PeekMessage: msg={}, hwnd={}, wParam={}, lParam={}", msg.message, (void*)msg.hwnd, msg.wParam, msg.lParam);
            if (msg.message == WM_QUIT) {
                Logger::info("Stop game, msg quit");
                running = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            Logger::info("render and update game");
            update();
            render();
        }
    }

    Logger::info("mainloop stopped, mainLoop::msg.message={}", msg.message);
}

void Game::update() {
    Logger::info("update run");
    if (sceneManager) sceneManager->update(0.016f);
}

void Game::render() {
    Logger::info("render run");

    waitForPreviousFrame();  

    frameIndex = swapChain->GetCurrentBackBufferIndex();

    if (sceneManager) {
        auto* scene = dynamic_cast<MenuScene*>(sceneManager->getCurrentScene());
        if (scene) scene->setFrameIndex(frameIndex);

        sceneManager->render();
    }

    swapChain->Present(1, 0);
}


void Game::waitForPreviousFrame() {
    const UINT64 currentFenceValue = fenceValue;
    commandQueue->Signal(fence.Get(), currentFenceValue);
    fenceValue++;

    if (fence->GetCompletedValue() < currentFenceValue) {
        fence->SetEventOnCompletion(currentFenceValue, fenceEvent);
        WaitForSingleObject(fenceEvent, INFINITE);
    }

    frameIndex = swapChain->GetCurrentBackBufferIndex();
}

void Game::cleanup() {
    if (fenceEvent) {
        CloseHandle(fenceEvent);
        fenceEvent = nullptr;
    }
    sceneManager.reset();
}

int Game::run() {
    mainLoop();
    return 0;
}