#pragma once

#include <windows.h>
#include <wrl/client.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <string>
#include <memory>

#include "core/SceneManager.hpp"

class Game {
public:
    Game(HINSTANCE hInstance);
    ~Game();

    bool initialize();
    int run();

private:
    HINSTANCE hInstance;
    HWND hwnd;

    UINT width = 1280;
    UINT height = 720;
    std::wstring windowTitle = L"Pokemon Simulator";

    Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory;
    Microsoft::WRL::ComPtr<ID3D12Device> device;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;

    UINT frameIndex;
    UINT64 fenceValue;
    HANDLE fenceEvent;

    std::unique_ptr<SceneManager> sceneManager;

    bool initWindow();
    bool initD3D();
    void mainLoop();
    void update();
    void render();
    void cleanup();
};
