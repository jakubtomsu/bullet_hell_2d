
#include "input.h"
#include "engine.h"

#include <GLFW/glfw3.h>



m_v2 input_mouse_last;
m_v2 input_mouse_offset;
m_v2 input_mouse_coords;
#define INPUT_STATE_BUFFER_SIZE GLFW_KEY_LAST + 1
input_state input_state_buffer[INPUT_STATE_BUFFER_SIZE];

bool input_up(const int glfw_key) {
    return (input_state_buffer[glfw_key] == UP || input_state_buffer[glfw_key] == RELEASED);
}
bool input_down(const int glfw_key) {
    return (input_state_buffer[glfw_key] == DOWN || input_state_buffer[glfw_key] == PRESSED || input_state_buffer[glfw_key] == REPEATED);
}
bool input_released(const int glfw_key) {
    return (input_state_buffer[glfw_key] == RELEASED);
}
bool input_pressed(const int glfw_key) {
    return (input_state_buffer[glfw_key] == PRESSED);
}
bool input_repeating(const int glfw_key) {
    return (input_state_buffer[glfw_key] == REPEATED || (input_state_buffer[glfw_key] == PRESSED));
}


void input_mouse_move_callback(GLFWwindow* window, double xpos, double ypos) {
    const float c_xoffset = xpos - input_mouse_last.x;
    const float c_yoffset = ypos - input_mouse_last.y;
    input_mouse_offset.x += c_xoffset;
    input_mouse_offset.y += c_yoffset;
    input_mouse_last.x = xpos;
    input_mouse_last.y = ypos;
    input_mouse_coords = input_mouse_last;
}

void input_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    input_key_press_callback(window, button, -1, action, mods);
}


void input_key_press_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    /*example use: if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)*/
    if (key < 0 || key > GLFW_KEY_LAST) return;
    
    switch (action) {
        case GLFW_PRESS: {
            input_state_buffer[key] = PRESSED;
        }break;
        case GLFW_RELEASE: {
            input_state_buffer[key] = RELEASED;
        }break;
        case GLFW_REPEAT: {
            input_state_buffer[key] = REPEATED;
        }break;
    }
}

void input_global_update() {
    
    for (int i = 0; i < INPUT_STATE_BUFFER_SIZE; i++) {
        input_state new_state;
        input_state current_state = input_state_buffer[i];
        switch (current_state) {
            case PRESSED: {
                new_state = DOWN;
            }break;
            case REPEATED: {
                new_state = DOWN;
            }break;
            case RELEASED: {
                new_state = UP;
            }break;
            default: {
                new_state = current_state;
            }break;
        }
        input_state_buffer[i] = new_state;
    }
    
    
    input_mouse_offset = {};
    glfwPollEvents();
}