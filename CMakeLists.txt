cmake_minimum_required(VERSION 3.26)

# Configurando o padrão c++
set(CMAKE_CXX_STANDARD 17)

# Configurando o nome e executável do projeto
set(PROJECT_NAME "stellar-defenders")
project(${PROJECT_NAME})

# Encontrando as bibliotecas SDL
find_package(SDL2 2.32.2 EXACT REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Criando o executável sem fontes ainda
add_executable(${PROJECT_NAME})

# --- Descoberta Recursiva de Arquivos ---
# Busca recursivamente por todos os arquivos .cpp e .h no diretório Source
file(GLOB_RECURSE SOURCE_FILES
    CONFIGURE_DEPENDS
    "Source/*.cpp"
    "Source/*.h"
)

# Adiciona os arquivos encontrados como fontes do seu projeto
target_sources(${PROJECT_NAME}
    PRIVATE
    ${SOURCE_FILES}
)
# ----------------------------------------

# Linkando as bibliotecas SDL
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)

# Opções específicas para MinGW (compilação no Windows)
if(MINGW)
    target_link_options(${PROJECT_NAME} PRIVATE -mconsole)
endif()