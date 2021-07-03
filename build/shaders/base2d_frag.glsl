
#version 330 core

out vec4 frag_color;

in vec2 frag_uv;
in vec2 frag_pos;

void main() {

	frag_color = vec4(frag_uv,1.0,1.0);

}