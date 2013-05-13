#version 150

in vec4 position;

void main() {
	gl_Position = position;
	gl_PointSize = 10.0;
}
