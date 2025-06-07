#pragma once

#include "core/Scene.hpp"
#include <d3d12.h>
#include <wrl.h>
#include <DirectXTex.h>
#include <string>
#include <vector>

class MenuScene : public Scene{
public:
    MenuScene(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12CommandQueue* commandQueue, ID3D12CommandAllocator* commandAllocator, ID3D12Resource* renderTargets[],
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[], UINT frameIndex);
    ~MenuScene();

    void load();
    void unload();
    void update(float deltaTime);
    void render();
    void setFrameIndex(UINT index);


private:
    ID3D12Device* device;
    ID3D12GraphicsCommandList* commandList;
    ID3D12CommandQueue* commandQueue;
    ID3D12Resource** renderTargets;
    D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandles;
    UINT frameIndex;
    ID3D12CommandAllocator* commandAllocator;

    Microsoft::WRL::ComPtr<ID3D12Resource> backgroundTexture;

    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;

    struct Button {
        std::string text;
        RECT area;
    };
    std::vector<Button> buttons;
};