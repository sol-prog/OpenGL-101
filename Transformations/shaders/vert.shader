#version 150

in vec4 position;
in vec2 texture_coord;
out vec2 texture_coord_from_vshader;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main() {
	gl_Position = Projection * View * Model * position;
	texture_coord_from_vshader = texture_coord;
}
