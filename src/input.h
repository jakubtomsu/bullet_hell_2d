
#ifndef INPUT_H
#define INPUT_H

#include "math.h"

struct GLFWwindow;

enum input_state {
    UP,       // key is in idle state
    DOWN,     // key is held down by the user
    RELEASED, // key was released just this frame
    PRESSED,  // key was pressed during this frame
    REPEATED  // press input starts repeating after short delay defined by OS
};

extern m_v2 input_mouse_offset;

bool input_up(const int glfw_key);
bool input_down(const int glfw_key);
bool input_released(const int glfw_key);
bool input_pressed(const int glfw_key);
bool input_repeating(const int glfw_key);

void input_mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
void input_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void input_key_press_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void input_global_update();

#endif //INPUT_H
