
#include <glad/glad.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HANDMADE_MATH_IMPLEMENTATION
#include "m_math.h"

#include "engine.h"
#include "graphics.h"
#include "input.h"
#include "entity.h"
#include "game.h"


float delta_time;
float last_frame;
int window_x = 900;
int window_y = 500;
bool engine_should_quit = false;
GLFWwindow* engine_glfw_window;
std::string engine_root_path;
long long engine_frame;
float engine_time;


static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	window_x = width;
	window_y = height;
	glViewport(0, 0, width, height);
	//camera_global_render_buffers_update();
}

void test_update_func(int id, entity_t* entity) {
    entity->position += m_randv2() * 0.1f;
}

int main(int argc, char* argv[]) {
    
    printf("hey ya!\n");
    
    {
		unsigned int pos = std::string(argv[0]).find_last_of("\\/");
        engine_root_path = std::string(argv[0]).substr(0, pos) + "\\";
		printf("(MAIN) root path: %s\n", engine_root_path.c_str());
	}
    
    glfwInit();
    
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
	int monitor_count;
	GLFWmonitor* monitor = glfwGetMonitors(&monitor_count)[0]; // 0 is main monitor
	
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	//window_height = mode->height;
	//window_width = mode->width;
    
	GLFWwindow* window = glfwCreateWindow(window_x, window_y, "game", NULL, NULL);
	if (window == NULL) {
		printf("(MAIN) failed to create GLFW window");
		glfwTerminate();
		return -1;
	}
    engine_glfw_window = window;
    printf("window created\n");
	//glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR,  GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window,       input_mouse_move_callback);
	glfwSetMouseButtonCallback(window,     input_mouse_button_callback);
	glfwSetKeyCallback(window,             input_key_press_callback);
	glfwSwapInterval(1); // 0 == no vsync
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("(MAIN) Failed to initialize GLAD\n");
		return -1;
	}
    glViewport(0, 0, window_x, window_y);
    srand(time(0)); // set seed for rand()
    
    
    
    
    
    graphics_initialize();
    
    
    game_initalize();
    
    
    for(int i = 0; i < 10; i++) {
        entity_t e = ENTITY_DEFAULT;
        e.position = m_randv2() * 10;
        e.scale = m_v2{1,1} * ((i % 4) + 1);
        e.texture = 0;
        e.flags.collision_static = 0;
        entity_spawn(e);
    }\
    
    printf("starting main game loop\n");
    
    engine_frame = 0;
    while(!engine_should_quit) {
        engine_frame++;
        printf("--- FRAME %li ------------------\n", engine_frame);
        float current_time = glfwGetTime();
        delta_time = current_time - last_frame;
        last_frame = current_time;
        engine_time += delta_time;
        
        
        input_global_update();
        
        /* spawn test
        entity_t e = ENTITY_DEFAULT;
        e.position = m_randv2() * 10;
        e.scale = 2;
        e.texture = 0;
        e.update_func = test_update_func;
        int eid = entity_spawn(e);
        */
        //if(m_rand01() > 0.5) entity_destroy(eid); // destroy test
        
        entity_global_update();
        
        
        graphics_render_world(&main_camera);
        
        if(input_pressed(GLFW_KEY_F5) || input_pressed(GLFW_KEY_ESCAPE)) engine_should_quit = true;
        
        glfwSetWindowShouldClose(window, engine_should_quit);
        
    }
    
    glfwTerminate();
    
    return 0;
}