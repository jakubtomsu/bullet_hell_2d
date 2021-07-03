
#include <glad/glad.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "engine.h"
#include "graphics.h"
#include "math.h"
#include "input.h"


float delta_time;
float last_frame;
int window_x = 900;
int window_y = 500;
bool engine_should_quit = false;


static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	window_x = width;
	window_y = height;
	glViewport(0, 0, width, height);
	//camera_global_render_buffers_update();
}

int main() {
    
    printf("hey ya!\n");
    
    
    
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
	//glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR,  GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window,       input_mouse_move_callback);
	glfwSetMouseButtonCallback(window,     input_mouse_button_callback);
	glfwSetKeyCallback(window,             input_key_press_callback);
	//glfwSwapInterval(0); // disable vsync
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("(MAIN) Failed to initialize GLAD\n");
		return -1;
	}
    glViewport(0, 0, window_x, window_y);
    srand(time(0)); // set seed for rand()
    
    printf("starting main game loop\n");
    
    long long engine_frame = 0;
    while(!engine_should_quit) {
        engine_frame++;
        printf("--- FRAME %li ------------------\n", engine_frame);
        float current_time = glfwGetTime();
        delta_time = current_time - last_frame;
        last_frame = current_time;
        
        input_global_update();
        
        
        
        if(input_pressed(GLFW_KEY_F5)) engine_should_quit = true;
        
        glfwSetWindowShouldClose(window, engine_should_quit);
        
    }
    
    glfwTerminate();
    
    return 0;
}