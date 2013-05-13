#version 150

in vec4 color_from_vshader;
out vec4 out_color;

void main() {
	out_color = color_from_vshader;
}
