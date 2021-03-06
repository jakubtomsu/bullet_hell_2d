
#version 330 core

out vec2 frag_uv;
out vec2 frag_pos;

layout (location = 0) in vec2 vert_pos; // always quad. coords are from {-0.5,-0.5} to {0.5,0.5}

uniform vec2 view_pos;
uniform float view_dist;
uniform float aspect_ratio;
uniform vec2 transform_pos;
uniform vec2 transform_scale;

void main() {
    vec2 pos = ((vert_pos * transform_scale) + (transform_pos - view_pos)) * vec2(1.0, aspect_ratio) / view_dist;
    gl_Position = vec4(pos.x, pos.y, 1.0, 1.0);
	frag_uv = (vert_pos + vec2(0.5,0.5));
	frag_pos = pos;
}