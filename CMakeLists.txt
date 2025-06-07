cmake_minimum_required(VERSION 3.15)
project(POKEMON_SIMULATOR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_WIN32_EXECUTABLE ON)

# Diretórios de cabeçalhos
include_directories(include)

# Define UNICODE para o Windows
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

# Vincula todas as bibliotecas necessárias
target_link_libraries(POKEMON_SIMULATOR
    PRIVATE
        d3d12
        dxgi
        dxguid
        d3dcompiler
        DirectXTex
        spdlog::spdlog
)
