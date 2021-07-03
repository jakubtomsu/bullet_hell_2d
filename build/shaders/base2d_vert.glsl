
#version 330 core

out vec2 frag_uv;
out vec2 frag_pos;

layout (location = 0) in vec2 vert_pos; // always quad. coords are from {-0.5,-0.5} to {0.5,0.5}

uniform vec2 view_pos;
uniform float view_dist;
uniform float view_rot;
uniform vec2 transform_pos;
uniform vec2 transform_scale;
uniform float transform_rot;

void main() {
    vec2 pos = (vert_pos * scale / view_dist) + (transform_position - view_pos);
    gl_Position = (pos, 1.0, 1.0);
	frag_uv = (vert_pos + vec2(1,1));
	frag_pos = pos;
}