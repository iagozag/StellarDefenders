cmake_minimum_required(VERSION 3.26)

# Configurando o padrão c++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Garante que C++17 seja usado e falhe se não for possível

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
include_directories(vendor)

# --- Descoberta Recursiva de Arquivos ---
# Busca recursivamente por todos os arquivos .cpp e .h no diretório Source
file(GLOB_RECURSE SOURCE_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/Source/*.cpp"
    "${CMAKE_SOURCE_DIR}/Source/*.h"
    "${CMAKE_SOURCE_DIR}/Source/*.hpp"
)

# Adiciona os arquivos encontrados como fontes do seu projeto
target_sources(${PROJECT_NAME}
    PRIVATE
    ${SOURCE_FILES}
)
# ----------------------------------------

# Aplicando as flags de compilação (CXXFLAGS)
target_compile_options(${PROJECT_NAME} PRIVATE
    # Warnings - Aplicados em todos os tipos de build
    -Wall
    -Wshadow
    -Wno-unused-result
    -Wno-sign-compare
    -Wno-char-subscripts
    -O2
)

# Flags específicas para o tipo de build
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Flags de depuração e sanitizers (apenas para builds de Debug)
    target_compile_options(${PROJECT_NAME} PRIVATE
        -fsanitize=address
        -fsanitize=undefined
        -fno-omit-frame-pointer
        -g # Adiciona informações de depuração
    )
    # Sanitizers também precisam de flags no linkador
    target_link_options(${PROJECT_NAME} PRIVATE
        -fsanitize=address
        -fsanitize=undefined
    )
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    # As otimizações (-O3) e -DNDEBUG são adicionadas automaticamente
    # pelo CMake para o tipo de build "Release".
    # Não precisamos adicionar nada aqui, a menos que queiramos flags *adicionais* específicas.
    # Exemplo: target_compile_options(${PROJECT_NAME} PRIVATE -ffast-math)
elseif (NOT CMAKE_BUILD_TYPE)
    # Define um tipo de build padrão se CMAKE_BUILD_TYPE não for especificado.
    # Isso pode ser útil para IDEs ou para evitar o erro de build se o usuário não definir.
    # Você pode escolher 'Debug' ou 'Release' como padrão.
    message(STATUS "No CMAKE_BUILD_TYPE specified, defaulting to Debug.")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, e.g. Debug, Release, RelWithDebInfo, MinSizeRel." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()


# Linkando as bibliotecas SDL
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)

# Opções específicas para MinGW (compilação no Windows)
if(MINGW)
    target_link_options(${PROJECT_NAME} PRIVATE -mconsole)
endif()