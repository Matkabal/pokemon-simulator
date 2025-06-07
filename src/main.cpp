#include "core/Game.hpp"
#include <windows.h>
#include "logger/Logger.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {

    Logger::init();

    Logger::info("initiate class");

    Game game(hInstance);
    if (!game.initialize()) {
        MessageBox(nullptr, L"Falha ao inicializar o jogo.", L"Erro", MB_OK | MB_ICONERROR);
        Logger::error("Error in initialize game");

        return -1;
    }

    Logger::info("Game initialize with sucess");

    return game.run();
}
