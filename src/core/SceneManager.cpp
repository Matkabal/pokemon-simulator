#include "core/SceneManager.hpp"

void SceneManager::setScene(Scene* scene) {
    if (currentScene) {
        currentScene->unload();
    }
    currentScene = scene;
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

void SceneManager::unload() {
    if (currentScene) {
        currentScene->unload();
        currentScene = nullptr;
    }
}

SceneManager::~SceneManager() {
    unload();
}