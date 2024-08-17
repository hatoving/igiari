cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(igiari)

# Generate compile_commands.json
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/debug) 

find_library(VORBIS_LIBRARY vorbis)
find_library(VORBISFILE_LIBRARY vorbisfile)
find_library(OGG_LIBRARY ogg)
# Our Project

add_executable(${PROJECT_NAME}
    src/utils/base64.c
    src/utils/lz4.c
    src/utils/unicode.c
    src/utils/reader.c

    src/fmod/fsb.c
    src/fmod/rebuild.c

    src/unity/texture2d.c
    src/unity/audio_clip.c
    src/unity/serialized_file.c
    src/unity/bundle.c

    src/mdt.c
    src/main.c
)

target_link_libraries(${PROJECT_NAME}
    ogg
    vorbis
)