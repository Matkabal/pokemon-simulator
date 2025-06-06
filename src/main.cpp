#include "core/Game.hpp"
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    Game game(hInstance);
    if (!game.initialize()) {
        MessageBox(nullptr, L"Falha ao inicializar o jogo.", L"Erro", MB_OK | MB_ICONERROR);
        return -1;
    }

    return game.run();
}
