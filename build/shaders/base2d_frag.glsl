
#version 330 core

out vec4 frag_color;

in vec2 frag_uv;
in vec2 frag_pos;

uniform sampler2D main_texture;
uniform vec3 color;
uniform vec2  texture_scale;
uniform vec2 texture_offset;
uniform float aspect_ratio;




#define VLERP(a, b, t) (((a) * (1.0f - (t))) + ((b) * (t)))



void main() {


	vec4 tex = texture(main_texture, (frag_uv + texture_offset) * texture_scale);
	if(tex.a < 0.5) discard;
	frag_color = tex *vec4(color,1.0);


//	frag_color += vec4(round(frag_color.rgb * 32) / 32.0, 1.0) * 0.1;

	float vnx = abs(frag_pos.x) * aspect_ratio;
	float vny =  abs(frag_pos.y);
	float vingette =  pow(vnx * vnx + vny * vny, 0.5);
	vingette = pow(vingette, 3);
	vingette *= 0.05;
	frag_color -= vec4(vec3(vingette), 0.0);

	frag_color *= vec4(0.86,0.94,1,1);


	float cl = pow(length(frag_color.rgb), 2);
	const int checker_w = 6;
	const int checker_hw = checker_w / 2;
	vec2 checker_coord = gl_FragCoord.xy ;
	float checker_pattern = float(
		int(checker_coord.x) % checker_w  < checker_hw ?
			(int(checker_coord.y) % checker_w  < checker_hw ? cl : 1.0 ) :
			(int(checker_coord.y) % checker_w  < checker_hw ? 1.0 : cl)
	);
	frag_color = VLERP(frag_color, frag_color * checker_pattern, 0.07);

	vec3 ncol = normalize(frag_color.rgb);
	float val = length(frag_color.rgb);
	const float steps = 16;
	frag_color = vec4(ncol * round(val * steps) / steps,1.0);


//	frag_color = vec4(color,1.0);
//	frag_color = vec4(frag_uv,1.0,1.0);


}
