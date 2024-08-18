# igiari
igiari (``異議あり; "Objection!"``) is a work-in-progress engine for the Ace Attorney games released by Capcom. Currently, work is set on support the Trilogy Remaster that's released on Steam.

# Warning
This is not yet playable as of the time that I'm writing this. There's still a long way to go, but if you wanna add in some help then feel free to make a pull request or ask!

# Dependecies
- SDL2
- OpenGL ES 2.0
- LZ4
- cJSON (not yet used)

Everything else is coded in pure C.

# Building (for Developers)

Make sure you have CMake, gcc and Raylib already installed in your dev enviroment.
Once you're sure, you can execute these commands to build the engine:
```
mkdir build && cd build
cmake ..
mkdir debug && cd debug
cd .. && make && cd debug && ./igiari
```
(WARNING: It would be advised though not to build right now as I'm tinkering around with decrypted files that would normally be encrypted by the game. Only build it if you know what you're doing.)
