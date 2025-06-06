#pragma once

#include <windows.h>
#include <d3d11.h>
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
    SceneManager* sceneManager;

    ID3D11Device* device;
    ID3D11DeviceContext* context;
    IDXGISwapChain* swapChain;
    ID3D11RenderTargetView* renderTargetView;

    bool initWindow();
    bool initDirectX();
    void render();
    void cleanUp();
};
