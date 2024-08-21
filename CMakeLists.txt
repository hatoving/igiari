cmake_minimum_required(VERSION 3.11)
project(igiari)

# Generate compile_commands.json
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O3")

# Define source files
set(SOURCE_FILES
    # Utils
    src/utils/lz4.c
    src/utils/unicode.c
    src/utils/reader.c
    src/utils/dxt1.c
    src/utils/dxt5.c

    # FMOD - .fsb
    src/assets/fmod/fsb.c
    src/assets/fmod/rebuild.c

    # Unity (2017.4) asset files - .unity3d
    src/assets/unity/texture2d.c
    src/assets/unity/audio_clip.c
    src/assets/unity/serialized_file.c
    src/assets/unity/bundle.c
    
    # Phoenix Wright: Ace Attorney Trilogy asset files - .mdt
    src/assets/pwaat/mdt.c
    
    # Engine
    src/engine/imgui.cpp
    src/engine/camera.c
    src/engine/shader.c
    src/engine/texture.c
    src/engine/sprite.c
    src/engine/audio.c
    src/engine/core.c
    
    src/games/trilogy/sound_data.c
    
    src/main.c
)

if(WIN32)
    list(APPEND SOURCE_FILES src/utils/glad.c)
endif()

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME} mingw32)
endif()

target_link_libraries(${PROJECT_NAME}
    -Wl,-Bstatic
    ogg
    vorbis
    vorbisfile
    
    imgui
    
    -Wl,-Bdynamic
    SDL2main
    SDL2
    SDL2_Mixer_ext
)
