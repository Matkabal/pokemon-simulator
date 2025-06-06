// Game.cpp
#include "core/Game.hpp"
#include <stdexcept>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Game::Game(HINSTANCE hInstance) : hInstance(hInstance), hwnd(nullptr),
    device(nullptr), context(nullptr), swapChain(nullptr), renderTargetView(nullptr), sceneManager(nullptr) {}

Game::~Game() {
    cleanUp();
}

bool Game::initialize() {
    if (!initWindow()) return false;
    if (!initDirectX()) return false;

    sceneManager = new SceneManager();
    return true;
}

int Game::run() {
    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            render();
        }
    }
    return static_cast<int>(msg.wParam);
}

bool Game::initWindow() {
    const wchar_t CLASS_NAME[] = L"PokemonSimulatorWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        0, CLASS_NAME, L"Pokemon Simulator", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr, nullptr, hInstance, nullptr);

    if (!hwnd) return false;

    ShowWindow(hwnd, SW_SHOW);
    return true;
}

bool Game::initDirectX() {
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hwnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
        D3D11_SDK_VERSION, &scd, &swapChain, &device, nullptr, &context);

    if (FAILED(hr)) return false;

    ID3D11Texture2D* backBuffer = nullptr;
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
    backBuffer->Release();

    context->OMSetRenderTargets(1, &renderTargetView, nullptr);

    return true;
}

void Game::render() {
    float clearColor[4] = { 0.1f, 0.1f, 0.3f, 1.0f };
    context->ClearRenderTargetView(renderTargetView, clearColor);

    sceneManager->update(0.016f);
    sceneManager->render();

    swapChain->Present(1, 0);
}

void Game::cleanUp() {
    if (sceneManager) delete sceneManager;
    if (renderTargetView) renderTargetView->Release();
    if (swapChain) swapChain->Release();
    if (context) context->Release();
    if (device) device->Release();
}