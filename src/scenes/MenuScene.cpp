#include "scenes/MenuScene.hpp"
#include <DirectXTex.h>

MenuScene::MenuScene(ID3D12Device* device)
    : device(device) {}

MenuScene::~MenuScene() {
    unload();
}

void MenuScene::load() {
    DirectX::TexMetadata metadata = {};
    DirectX::ScratchImage scratchImage;

    HRESULT hr = DirectX::LoadFromWICFile(
        L"assets/images/initial/initial.png",
        DirectX::WIC_FLAGS_NONE,
        &metadata,
        scratchImage
    );

    if (FAILED(hr)) {
        // log ou erro
        return;
    }

    hr = DirectX::CreateTexture(
        device,
        metadata,
        &backgroundTexture
    );

    if (FAILED(hr)) {
        // log ou erro
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
    // Lógica de interação pode vir aqui
}

void MenuScene::render() {
    // Renderização com DirectX 12 virá aqui futuramente
    // Exemplo: criar e usar command lists, pipeline, etc.
}