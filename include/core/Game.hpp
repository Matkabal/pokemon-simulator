#pragma once

#include <windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
#include "core/SceneManager.hpp"
#include <string>

class Game {
public:
    Game(HINSTANCE hInstance);
    ~Game();

    bool initialize();
    int run();

private:
    static const UINT FrameCount = 2;

    // Sistema de janelas
    HINSTANCE hInstance;
    HWND hwnd;
    UINT width = 1280;
    UINT height = 720;
    std::wstring windowTitle = L"Pokemon Simulator";

    // DXGI + D3D12
    Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory;
    Microsoft::WRL::ComPtr<ID3D12Device> device;
    Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain;

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;
    Microsoft::WRL::ComPtr<ID3D12Resource> renderTargets[FrameCount];
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[FrameCount];
    UINT rtvDescriptorSize = 0;

    UINT frameIndex;

    // Fence para sincronização GPU/CPU
    Microsoft::WRL::ComPtr<ID3D12Fence> fence;
    UINT64 fenceValue;
    HANDLE fenceEvent;

    // Gerenciador de cenas
    std::unique_ptr<SceneManager> sceneManager;

    // Métodos principais
    bool initWindow();
    bool initD3D();
    void mainLoop();
    void update();
    void render();
    void cleanup();
    void waitForPreviousFrame();
};
