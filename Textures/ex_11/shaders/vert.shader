#version 150

in vec4 position;
in vec2 texture_coord;
out vec2 texture_coord_from_vshader;

void main() {
	gl_Position = position;
	texture_coord_from_vshader = texture_coord;
}
