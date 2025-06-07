cmake_minimum_required(VERSION 3.15)
project(POKEMON_SIMULATOR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_WIN32_EXECUTABLE ON)

# Diretórios de cabeçalhos
include_directories(include)

# Define UNICODE para o Windows e suprime warnings do MSVC
add_definitions(-DUNICODE -D_UNICODE)
add_definitions(-D_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)

# Coleta todos os arquivos .cpp no diretório src/
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Cria o executável
add_executable(POKEMON_SIMULATOR ${SOURCES})

# Desativa ferramentas auxiliares do DirectXTex
set(BUILD_TOOLS OFF CACHE BOOL "Disable DirectXTex tools")

# Baixa e inclui o DirectXTex
include(FetchContent)
FetchContent_Declare(
  DirectXTex
  GIT_REPOSITORY https://github.com/microsoft/DirectXTex.git
  GIT_TAG main
)
FetchContent_MakeAvailable(DirectXTex)

# Baixa e inclui o spdlog
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.13.0
)
FetchContent_MakeAvailable(spdlog)

# Baixa o repositório com d3dx12.h (somente header)
FetchContent_Declare(
  D3DX12
  GIT_REPOSITORY https://github.com/microsoft/DirectX-Graphics-Samples.git
  GIT_TAG master
)
FetchContent_MakeAvailable(D3DX12)

# Adiciona os includes de D3DX12
target_include_directories(POKEMON_SIMULATOR
    PRIVATE
        ${d3dx12_SOURCE_DIR}/MiniEngine/Core
)

# Bibliotecas necessárias
target_link_libraries(POKEMON_SIMULATOR
    PRIVATE
        d3d12
        dxgi
        dxguid
        d3dcompiler
        DirectXTex
        spdlog::spdlog
)
