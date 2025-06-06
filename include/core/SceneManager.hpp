#pragma once

#include "core/Scene.hpp"

class SceneManager {
private:
    Scene* currentScene = nullptr;

public:
    void setScene(Scene* scene);
    void update(float deltaTime);
    void render();
    void unload();
    ~SceneManager();
};