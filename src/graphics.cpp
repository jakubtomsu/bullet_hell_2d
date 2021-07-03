
#include "graphics.h"
#include "engine.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>



camera_t main_camera;
unsigned int shader_used;


#define SHADER_FOLDER_PATH (engine_root_path + "\\shaders\\")

unsigned int shader_base2d;

unsigned int quadVAO, quadVBO;


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
}

void graphics_update_buffers() {
    
}

void graphics_render_world(camera_t* cam) {
    
    glClearColor(cam->background_color.x, cam->background_color.y, cam->background_color.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    shader_use(shader_base2d);
    shader_set_vec2("view_pos",   cam->position);
    shader_set_float("view_dist", cam->distance);
    shader_set_float("view_rot",  cam->rotation);
    
    draw_quad({0,0}, 0, {0.1,0.1}, {1,1,0});
    
    glfwSwapBuffers(engine_glfw_window);
    print_gl_errors("after swap buffers");
}

void draw_quad(m_v2 pos, float rot, m_v2 scale, m_v3 col) {
    //shader_set_vec4("mainColor", { color,alpha });
	
	shader_set_vec2("transform_pos", pos);
    shader_set_float("transform_rot", rot);
	shader_set_vec2("transform_scale", scale);
    
	// render quad
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
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