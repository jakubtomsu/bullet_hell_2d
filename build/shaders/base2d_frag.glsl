
#version 330 core

out vec4 frag_color;

in vec2 frag_uv;
in vec2 frag_pos;

uniform sampler2D main_texture;

void main() {

	frag_color = vec4(frag_uv,1.0,1.0);
	frag_color = vec4(texture(main_texture, frag_uv).rgb, 1.0);

}