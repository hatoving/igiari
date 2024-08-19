# igiari
igiari is a work-in-progress game engine for the 2D Ace Attorney games published by Capcom.

# Compatiblity
Right now, work is being set on working on the remaster of the original trilogy.

| Game                                      | Support               | Progress  |
| ---                                       | ---                   | ---       |
| Phoenix Wright: Ace Attorney Trilogy      | W.I.P.                | 0.00%     |
| Apollo Justice: Ace Attorney Trilogy      | Not Planned (For now) | 0.00%     |

# Goals

- Have the entirety of Phoenix Wright: Ace Attorney Trilogy playable
	- (While having the entirety of the first game being playable as a beta release)
- Mod Support (custom textures, music, scenarios...) -- planned for way later!!
- Have it released on multiple platforms (Windows, Linux, macOS, etc.)

# Warning / For People Who Wanna Help
This is not yet playable as of the time that I'm writing this. There's still a long way to go, but if you wanna add in some help then feel free to make a pull request or ask!

# Dependecies
- SDL2
- SDL_Mixer_X
- OpenGL ES 2.0

# Libraries Used
- LZ4
- GLAD (for Windows builds)

Everything else is coded in pure C.

# Building (for Developers)
Make sure you have `CMake`, `gcc`,`OpenGL`, `SDL2` and `SDL2_Mixer_X` already installed in your dev environment.
Once you're sure, you can execute these commands to build the engine:
```
mkdir build && cd build
cmake ..
mkdir debug && cd debug
cd .. && make && cd debug && ./igiari
```
(WARNING: It would be advised though not to build right now as I'm tinkering around with decrypted files that would normally be encrypted by the game. Only build it if you know what you're doing.)