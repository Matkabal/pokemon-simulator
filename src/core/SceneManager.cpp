#include "core/SceneManager.hpp"

SceneManager::SceneManager() {}

SceneManager::~SceneManager() {
    if (currentScene) {
        currentScene->unload();
        currentScene.reset();
    }
}

void SceneManager::changeScene(std::unique_ptr<Scene> newScene) {
    if (currentScene) {
        currentScene->unload();
    }

    currentScene = std::move(newScene);

    if (currentScene) {
        currentScene->load();
    }
}

void SceneManager::update(float deltaTime) {
    if (currentScene) {
        currentScene->update(deltaTime);
    }
}

void SceneManager::render() {
    if (currentScene) {
        currentScene->render();
    }
}

Scene* SceneManager::getCurrentScene() {
    return currentScene.get();
}
