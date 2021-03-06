
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "m_math.h"
#include <glad/glad.h>
#include "engine.h"

typedef struct camera_t {
    
	m_v2 position;
	float distance = 20;
    float rotation;
    
	m_v3 background_color = { 0.18,0.15,0.2 };
    
} camera_t;

extern camera_t main_camera;
extern unsigned int shader_used;
extern m_v2 cursor_pos;

void graphics_initialize();
void graphics_render_world(camera_t* cam);

static inline m_v2 camera_ndc_to_world(camera_t* cam, m_v2 a) {
    return (cam->position + (a / m_v2{1.0, (float)window_x / (float)window_y}  * cam->distance));
}

void draw_quad(m_v2 pos, m_v2 scale, unsigned texture,m_v2 texture_scale, m_v2 texture_offset, m_v3 col);

unsigned int texture_import(const char* file_name, unsigned int gl_interpolation, unsigned int gl_wrapping);

unsigned int shader_import(const char* vert_name, const char* frag_name);
void shader_use(unsigned int shader);
static inline void shader_set_bool(const char* name, bool value) {
    glUniform1i(glGetUniformLocation(shader_used, name), (int)value);
}
static inline void shader_set_int(const char* name, int value) {
    glUniform1i(glGetUniformLocation(shader_used, name), value);
}
static inline void shader_set_uint(const char* name, unsigned int value) {
    glUniform1ui(glGetUniformLocation(shader_used, name), value);
}
static inline void shader_set_float(const char* name, float value) {
    glUniform1f(glGetUniformLocation(shader_used, name), value);
}
static inline void shader_set_vec2(const char* name, m_v2 value) {
    glUniform2f(glGetUniformLocation(shader_used, name), value.x, value.y);
}
static inline void shader_set_vec3(const char* name, m_v3 value) {
    glUniform3f(glGetUniformLocation(shader_used, name), value.x, value.y, value.z);
}
/*
static inline void shader_set_vec4(const char* name, m_v4 value) {
    glUniform4f(glGetUniformLocation(shader_used, name), value.x, value.y, value.z, value.w);
}\
*/
static inline void shader_set_mat4(const char* name, m_mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(shader_used, name), 1, GL_FALSE, &value.columns[0][0]);
}
static inline void shader_set_texture2d(unsigned int texture_slot, unsigned int texture_id) {
    glActiveTexture(GL_TEXTURE0 + texture_slot);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}




#endif
