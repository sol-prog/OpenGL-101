#version 150

in vec2 texture_coord_from_vshader;
out vec4 out_color;

uniform sampler2D texture_sampler;

void main() {
	out_color = texture(texture_sampler, texture_coord_from_vshader);
	float average_color = (out_color.r + out_color.g + out_color.b)/3.0;
	out_color = vec4(average_color, average_color, average_color, 1.0);
}
