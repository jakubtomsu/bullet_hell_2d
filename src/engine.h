
#ifndef ENGINE_H
#define ENGINE_H



#include <string>

struct GLFWwindow;

extern float delta_time;
extern int window_x;
extern int window_y;
extern bool engine_should_quit;
extern GLFWwindow* engine_glfw_window;
extern std::string engine_root_path;
extern long long engine_frame;
extern float engine_time;

#endif