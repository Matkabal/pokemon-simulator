#pragma once

#include "core/Scene.hpp"
#include <d3d12.h>
#include <wrl/client.h>
#include <DirectXTex.h>
#include <memory>
#include <vector>
#include <string>

class MenuScene : public Scene {
public:
    MenuScene(ID3D12Device* device);
    ~MenuScene() override;

    void load() override;
    void unload() override;
    void update(float deltaTime) override;
    void render() override;

private:
    ID3D12Device* device;
    Microsoft::WRL::ComPtr<ID3D12Resource> backgroundTexture;

    struct Button {
        std::string text;
        RECT area;
    };
    std::vector<Button> buttons;
};