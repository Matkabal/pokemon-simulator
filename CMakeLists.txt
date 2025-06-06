cmake_minimum_required(VERSION 3.15)
project(POKEMON_SIMULATOR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Inclui os diretórios de cabeçalhos
include_directories(include)

# Coleta os arquivos-fonte
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_definitions(-DUNICODE -D_UNICODE)

# Cria o executável
add_executable(POKEMON_SIMULATOR ${SOURCES})

# Inclui as libs necessárias do Windows SDK
target_link_libraries(POKEMON_SIMULATOR
  d3d11         # DirectX 11
  dxgi          # DirectX Graphics Infrastructure
  d3dcompiler   # Shader compiler
)
