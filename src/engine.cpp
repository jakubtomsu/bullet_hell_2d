
#include <stdio.h>
#define GLFW_DLL
#include <glad/glad.h>
#include <GLFW/glfw3.h>


int window_x = 700;
int window_y = 400;


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
	const GLFWgammaramp* gammaramp = glfwGetGammaRamp(monitor);
	
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	//window_height = mode->height;
	//window_width = mode->width;
    
	GLFWwindow* window = glfwCreateWindow(window_x, window_y, "game", NULL, NULL);
	if (window == NULL) {
		//E_LOG_ERROR("(MAIN) failed to create GLFW window");
		glfwTerminate();
		return -1;
	}
	//glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSetCursorPosCallback(window, input_mouse_move_callback);
	//glfwSetMouseButtonCallback(window, input_mouse_button_callback);
	//glfwSetKeyCallback(window, input_key_press_callback);
	//glfwSwapInterval(0); // disable vsync
    
    while(true) {
        printf("LOL");
    }
    
    return 0;
}