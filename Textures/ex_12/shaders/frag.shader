#version 150

in vec2 texture_coord_from_vshader;
out vec4 out_color;

uniform sampler2D texture_sampler;

void main() {
	out_color = texture(texture_sampler, texture_coord_from_vshader);
}
