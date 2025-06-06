#pragma once

#include <windows.h>
#include <d3d11.h>
#include "core/SceneManager.hpp"

class DirectXApp {
public:
    DirectXApp(HINSTANCE hInstance);
    ~DirectXApp();

    bool initialize();
    int run();

private:
    bool initWindow();
    bool initDirect3D();
    void mainLoop();
    void render();

    HINSTANCE hInstance;
    HWND hWnd;
    SceneManager* sceneManager;

    ID3D11Device* device;
    ID3D11DeviceContext* context;
    IDXGISwapChain* swapChain;
    ID3D11RenderTargetView* renderTargetView;

    UINT width = 800;
    UINT height = 600;
};