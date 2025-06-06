# 🕹️ POKEMON-SIMULATOR

Simulador gráfico inspirado em Pokémon, desenvolvido em C++ com OpenGL, ImGui e GLFW.  
Este projeto é uma base para jogos com interface gráfica, cenas controladas dinamicamente e uso de recursos modernos de GPU.

---

## 📁 Estrutura do Projeto

```
POKEMON-SIMULATOR/
├── CMakeLists.txt                 # Build do projeto via CMake
├── build/                         # Diretório gerado pelo CMake
├── bin/                           # Executáveis gerados
├── include/
│   ├── core/
│   │   ├── Game.hpp
│   │   ├── Scene.hpp
│   │   └── SceneManager.hpp
│   ├── scenes/
│   │   └── MenuScene.hpp
│   └── platform/
│       └── DirectXApp.hpp         # Interface com DirectX
├── src/
│   ├── main.cpp
│   ├── core/
│   │   ├── Game.cpp
│   │   ├── SceneManager.cpp
│   ├── scenes/
│   │   └── MenuScene.cpp
│   └── platform/
│       └── DirectXApp.cpp         # Implementação DirectX 11
├── assets/
│   └── images/
│       └── initial/
│           └── initial.png        # Imagem de fundo da tela inicial
├── README.md
```

---

## 🧩 Tecnologias e Bibliotecas

- **C++17**

---

## 🚀 Como compilar e executar

### 🔧 Requisitos:

- [CMake 3.15+](https://cmake.org/download/)
- [g++ / clang++ / MSVC]
- Git

### 💻 Build

```bash
# Configure
cmake -B build

# Compile
cmake --build build

# Executar (Linux/macOS)
./build/bin/POKEMON_SIMULATOR

# Executar (Windows)
build\bin\Debug\POKEMON_SIMULATOR.exe
```

---

## 🖼️ Tela Inicial (Start, Stop, Continue)

A `MenuScene` exibe:

- Uma imagem de fundo (`assets/images/initial/initial.png`)
- Botões interativos usando ImGui:
  - ▶️ Start
  - ⏹ Stop
  - ⏭ Continue

---

## 🛠️ Próximos passos

- Implementar `BattleScene` com lógica de combate
- Criar sistema de transição entre cenas via `SceneManager`
- Adicionar suporte a som/música (ex: SDL2_mixer)
- Sistema de save/load (modo “Continue” funcional)

---

## 📸 Screenshot (exemplo)

> Adicione aqui uma imagem do jogo:
```
assets/screenshots/menu.png
```

```markdown
![Tela inicial](assets/screenshots/menu.png)
```

---

## 📃 Licença

Projeto pessoal com fins educacionais. Sinta-se livre para estudar, modificar e expandir.

---