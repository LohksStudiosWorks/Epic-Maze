cmake_minimum_required(VERSION 3.28)
project(AllegroMonolith C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/installAllegro.cmake)

add_executable(${PROJECT_NAME} src/main.cpp)

include(cmake/MinGWFix.cmake)

target_compile_definitions(${PROJECT_NAME} PRIVATE ALLEGRO_STATICLINK)

target_link_libraries(${PROJECT_NAME} PRIVATE
    allegro
    allegro_main
    allegro_image
    allegro_font
    allegro_ttf
    allegro_audio
    allegro_acodec
    allegro_primitives
    allegro_color
    allegro_dialog
    allegro_video
    allegro_memfile
    allegro_physfs
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)