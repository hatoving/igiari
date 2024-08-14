cmake_minimum_required(VERSION 3.8)

project(boshi LANGUAGES C)

add_library(boshi OBJECT
    core.c
    object.c
    sprite.c
    unicode.c
)