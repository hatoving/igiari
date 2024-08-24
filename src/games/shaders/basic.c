#include "basic.h"

const char* IGIARI_SHADERS_BASIC_VERT = R"(
    #version 330 core
    attribute vec4 a_Position;
	void main() {
		gl_Position = a_Position;
	}
)";

const char* IGIARI_SHADERS_BASIC_FRAG = R"(
	#version 330 core
    precision mediump float;
	uniform vec4 u_Color;
	void main() {
		gl_FragColor = u_Color;
	}
)";