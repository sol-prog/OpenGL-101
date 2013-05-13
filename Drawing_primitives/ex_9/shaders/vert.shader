#version 150

in vec4 position;
in vec4 color;
out vec4 color_from_vshader;

void main() {
	gl_Position = position;
	color_from_vshader = color;
}
