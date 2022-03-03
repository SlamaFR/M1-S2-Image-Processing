cmake_minimum_required(VERSION 3.16)

project("Traitement d'images")

add_compile_options(-std=c++17 -Wall -W -Werror)

# TPs
file(GLOB tp_folders TP*)

foreach(tp ${tp_folders})
    add_subdirectory(${tp})
endforeach()
