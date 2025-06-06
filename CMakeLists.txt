cmake_minimum_required(VERSION 3.15)
project(POKEMON_SIMULATOR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_WIN32_EXECUTABLE ON)

# Inclui os diretórios de cabeçalhos
include_directories(include)

add_definitions(-DUNICODE -D_UNICODE)

# Coleta os arquivos-fonte
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Cria o executável
add_executable(POKEMON_SIMULATOR ${SOURCES})


# Adiciona as libs do Windows SDK para DirectX 12
target_link_libraries(POKEMON_SIMULATOR
    d3d12
    dxgi
    dxguid
    d3dcompiler
)

# Usa o DirectXTex (opcional, para carregar texturas)
include(FetchContent)
FetchContent_Declare(
  DirectXTex
  GIT_REPOSITORY https://github.com/microsoft/DirectXTex.git
  GIT_TAG main
)
FetchContent_MakeAvailable(DirectXTex)
target_link_libraries(POKEMON_SIMULATOR DirectXTex)
