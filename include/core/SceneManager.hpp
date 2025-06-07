#pragma once

#include "core/Scene.hpp"
#include <memory>

class SceneManager {
public:
    SceneManager();
    ~SceneManager();

    void changeScene(std::unique_ptr<Scene> newScene);
    void update(float deltaTime);
    void render();

    Scene* getCurrentScene();

private:
    std::unique_ptr<Scene> currentScene;
};