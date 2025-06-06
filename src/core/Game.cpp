#include "core/Game.hpp"
#include "scenes/MenuScene.hpp"
#include <stdexcept>
#include <chrono>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

Game::Game(HINSTANCE hInstance) : hInstance(hInstance), hwnd(nullptr), frameIndex(0), fenceValue(0), fenceEvent(nullptr) {}

Game::~Game() {
    cleanup();
}

bool Game::initialize() {
    if (!initWindow() || !initD3D()) return false;

    sceneManager = std::make_unique<SceneManager>();
    sceneManager->changeScene(std::make_unique<MenuScene>(device.Get()));

    return true;
}

bool Game::initWindow() {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"DX12WindowClass";

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        0, L"DX12WindowClass", windowTitle.c_str(),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        width, height, nullptr, nullptr, hInstance, nullptr
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
    if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue)))) return false;

    return true;
}

void Game::mainLoop() {
    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            update();
            render();
        }
    }
}

void Game::update() {
    if (sceneManager) sceneManager->update(0.016f); // delta time fixo por enquanto
}

void Game::render() {
    if (sceneManager) sceneManager->render();
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