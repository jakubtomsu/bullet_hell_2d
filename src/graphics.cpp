
#include "graphics.h"
#include "engine.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "entity.h"
#include "input.h"
#include "engine.h"


camera_t main_camera;
unsigned int shader_used;
m_v2 cursor_pos = {0.5,0.5};


#define SHADER_FOLDER_PATH (engine_root_path + "\\shaders\\")

unsigned int shader_base2d;

unsigned int quadVAO, quadVBO;
unsigned int cursor_tex;
unsigned int floor_tex;

static void print_gl_errors(const char* text) {
	auto e = glGetError();
	if (e == GL_NO_ERROR) return;
	printf("openGL errors (%s):\n", text);
	//draw_dbg_text("openGL errors (" + std::string(text) + "):");
    
#define PRING_GL_ERROR(message) \
printf("\t" message "\n"); \
//draw_dbg_text(std::string("\t") + message);
    
	while (e != GL_NO_ERROR) {
		switch (e) {
            case GL_INVALID_ENUM:
			PRING_GL_ERROR("GL_INVALID_ENUM");
			break;
            case GL_INVALID_VALUE:
			PRING_GL_ERROR("GL_INVALID_VALUE");
			break;
            case GL_INVALID_OPERATION:
			PRING_GL_ERROR("GL_INVALID_OPERATION");
			break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
			PRING_GL_ERROR("GL_INVALID_FRAMEBUFFER_OPERATION");
			break;
            case GL_OUT_OF_MEMORY:
			PRING_GL_ERROR("GL_OUT_OF_MEMORY");
			break;
            default:
			break;
		}
		e = glGetError();
	}
}


unsigned int tex;


static void test_col_update(int id, entity_t* entity) {
    entity->color = entity->collision_count > 0 ? m_v3{1,1,0} : m_v3{0,1,1};
    
}

void graphics_initialize() {
    
    const m_v2 quad2d_verts[6] = {
		{ -0.5, -0.5 },
		{ -0.5,  0.5 },
		{  0.5,  0.5 },
        
		{ -0.5, -0.5 },
		{  0.5,  0.5 },
		{  0.5, -0.5 }
	};
    
    glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_v2) * 6, quad2d_verts, GL_STATIC_DRAW); // todo put data here
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(m_v2), 0);
    
    
    shader_base2d = shader_import("base2d_vert.glsl", "base2d_frag.glsl");
    shader_use(shader_base2d);
    
    shader_set_int("main_texture",0);
    tex = texture_import("test.png", GL_LINEAR, GL_REPEAT);
    
    
    cursor_tex = texture_import("cursor.png", GL_NEAREST, GL_REPEAT);
    floor_tex = texture_import("background.png",GL_NEAREST, GL_REPEAT);
    
    
    for(int i = 0; i < 10; i++) {
        entity_t e = ENTITY_DEFAULT;
        e.texture = cursor_tex;
        e.position = m_randv2() * 20;
        e.update_func = test_col_update;
        entity_spawn(e);
    }
    
}

void graphics_render_world(camera_t* cam) {
    /*
        glClearColor(cam->background_color.x, cam->background_color.y, cam->background_color.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    */
    
    shader_use(shader_base2d);
    shader_set_vec2("view_pos",   cam->position);
    shader_set_float("view_dist", cam->distance);
    shader_set_float("view_rot",  cam->rotation);
    shader_set_float("aspect_ratio",(float)window_x / (float)window_y);
    
    const float floor_scale = 1000;
    const float floor_tile_scale = 10;
    draw_quad(
              {0,0},
              {floor_scale, floor_scale},
              floor_tex,
              {floor_scale / floor_tile_scale,(floor_scale / floor_tile_scale) * 1.5f},
              {},
              {1,1,1}
              );
    
    draw_quad({0,0},{ 1,1}, tex,{1,1},{}, {1,1,1});
    draw_quad({1,0},{ 1,1}, tex,{1,1},{}, {1,0,0});
    draw_quad({0,1},{ 1,1}, tex,{1,1},{}, {0,1,0});
    
    
    int ecount = 0;
    c_foreach_3(i, cmap_entity, entity_global_container) {
        entity_t* e = &i.ref->second;
        draw_quad(e->position, e->scale, e->texture, e->texture_scale, e->texture_offset, e->color);
        ecount++;
        
    }
    printf("entity count = %i\n", ecount);
    
    const float cursor_scale = 0.25f;
    draw_quad(cursor_pos, {cursor_scale,cursor_scale}, cursor_tex,{1,1},{}, {0.8,1,1});
    
    
    glfwSwapBuffers(engine_glfw_window);
    print_gl_errors("after swap buffers");
    
}

void draw_quad(m_v2 pos, m_v2 scale,unsigned int texture, m_v2 texture_scale, m_v2 texture_offset, m_v3 col) {
    
    shader_set_vec2("transform_pos", pos);
    shader_set_vec2("transform_scale", scale);
    shader_set_int("main_texture", 0);
    shader_set_vec2("texture_scale", texture_scale);
    shader_set_vec2("texture_offset", texture_offset);
    shader_set_vec3("color", col);
    
    // render quad
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


unsigned int texture_import(const char* file_name, unsigned int gl_interpolation, unsigned int gl_wrapping) {
    std::string filePath = engine_root_path + "\\textures\\" + file_name;
    
    printf("importing texture from [%s] ... ", filePath.c_str());
    
    unsigned int textureID;
    
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_wrapping);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_wrapping);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // mipmap interpolation
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_interpolation); // magnify interpolation. use GL_NEAREST for no filtering
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    
    if (data) {
        unsigned int channel = nrChannels <= 3 ? GL_RGB : GL_RGBA;
        
        glTexImage2D(GL_TEXTURE_2D, 0, channel, width, height, 0, channel, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, textureID);
        printf("done\n");
    }
    else
        printf("failed\n");
    
    stbi_image_free(data);
    
    return textureID;
}

unsigned int shader_import(const char* vert_name, const char* frag_name) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    std::string vertexPathStdStr = SHADER_FOLDER_PATH + vert_name;
    std::string fragmentPathStdStr = SHADER_FOLDER_PATH + frag_name;
    
    try {
        // open files
        vShaderFile.open(vertexPathStdStr);
        fShaderFile.open(fragmentPathStdStr);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        printf("(ERROR)(SHADER) couldn't read shader file");
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    
    
    // compile shaders
    unsigned int vertex, fragment;
    int  success;
    char infoLog[512];
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        printf("(ERROR)(SHADER) couldn't compile vertex shader, info: %s", infoLog);
    }
    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        printf("(ERROR)(SHADER) couldn't compile fragment shader, info: %s", infoLog);
    }
    
    
    // shader Program
    unsigned int ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        printf("(ERROR)(SHADER) shader program linking failed %s", infoLog);
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    return ID;
}

void shader_use(unsigned int shader) {
    shader_used = shader;
    glUseProgram(shader);
}