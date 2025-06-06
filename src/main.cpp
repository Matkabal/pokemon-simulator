#include "core/Game.hpp"

int main() {
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    Game game(hInstance);
    if (!game.initialize()) {
        return -1;
    }

    return game.run();
}
