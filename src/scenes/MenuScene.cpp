#include "scenes/MenuScene.hpp"
#include <DirectXTex.h>
#include <logger/Logger.hpp>
#include <d3dx12.h>

MenuScene::MenuScene(ID3D12Device* device,
                     ID3D12GraphicsCommandList* commandList,
                     ID3D12CommandQueue* commandQueue,
                     ID3D12CommandAllocator* commandAllocator,
                     ID3D12Resource* renderTargets[],
                     D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[],
                     UINT frameIndex)
    : device(device),
      commandList(commandList),
      commandQueue(commandQueue),
      commandAllocator(commandAllocator),
      renderTargets(renderTargets),
      rtvHandles(rtvHandles),
      frameIndex(frameIndex) {}

MenuScene::~MenuScene() {
    unload();
}

void MenuScene::setFrameIndex(UINT index) {
    frameIndex = index;
}

void MenuScene::load() {
    DirectX::TexMetadata metadata = {};
    DirectX::ScratchImage scratchImage;

    HRESULT hr = DirectX::LoadFromWICFile(
        L"../assets/images/initial/initial.png",
        DirectX::WIC_FLAGS_NONE,
        &metadata,
        scratchImage
    );

    if (FAILED(hr)) {
        Logger::error("Failed to load background image.");
        return;
    }

    hr = DirectX::CreateTexture(device, metadata, &backgroundTexture);
    if (FAILED(hr)) {
        Logger::error("Failed to create background texture.");
        return;
    }

    buttons.push_back({ "Start",    { 100, 300, 300, 350 } });
    buttons.push_back({ "Continue", { 100, 360, 300, 410 } });
    buttons.push_back({ "About",    { 100, 420, 300, 470 } });
}

void MenuScene::unload() {
    backgroundTexture.Reset();
    buttons.clear();
}

void MenuScene::update(float /*deltaTime*/) {
    // Lógica de interação pode vir aqui futuramente
}

void MenuScene::render() {
    try {
        if (!commandAllocator || !commandList || !commandQueue || !device) {
            Logger::error("MenuScene::render - Ponteiros nulos detectados");
            return;
        }

        Logger::info("MenuScene::render start, frameIndex={}", frameIndex);

        if (FAILED(commandList->Close())) {
            Logger::error("MenuScene::render - Falha ao fechar command list");
            return;
        }
        // Reset do command allocator e da command list
        HRESULT hr1 = commandAllocator->Reset();
        HRESULT hr2 = commandList->Reset(commandAllocator, nullptr);

        if (FAILED(hr1) || FAILED(hr2)) {
            Logger::error("MenuScene::render - Falha ao resetar allocator (hr1=0x{:X}) ou command list (hr2=0x{:X})", (unsigned)hr1, (unsigned)hr2);
            return;
        }

        // Transição de PRESENT → RENDER_TARGET
        D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            renderTargets[frameIndex],
            D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET
        );
        commandList->ResourceBarrier(1, &barrier);

        // Define a cor de fundo
        const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
        commandList->ClearRenderTargetView(rtvHandles[frameIndex], clearColor, 0, nullptr);

        // Transição de RENDER_TARGET → PRESENT
        barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            renderTargets[frameIndex],
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT
        );
        commandList->ResourceBarrier(1, &barrier);

        ID3D12CommandList* lists[] = { commandList };
        commandQueue->ExecuteCommandLists(_countof(lists), lists);

        Logger::info("MenuScene::render end");

    } catch (const std::exception& e) {
        spdlog::error("[MenuScene::render] Exceção: {}", e.what());
    }
}

