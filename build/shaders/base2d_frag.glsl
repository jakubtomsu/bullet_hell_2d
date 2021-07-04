
#version 330 core

out vec4 frag_color;

in vec2 frag_uv;
in vec2 frag_pos;

uniform sampler2D main_texture;
uniform vec3 color;

void main() {


	vec4 tex = texture(main_texture, frag_uv);
	if(tex.a < 0.1) discard;
	frag_color = tex *vec4(color,1.0);

//	frag_color = vec4(color,1.0);
//	frag_color = vec4(frag_uv,1.0,1.0);


}
