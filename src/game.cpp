
#include "game.h"
#include "graphics.h"
#include "entity.h"
#include "input.h"
#include "engine.h"
#include "m_math.h"

unsigned int player_texture_idle_up;
unsigned int player_texture_idle_down;
unsigned int player_texture_idle_horizontal;
unsigned int player_texture_run_horizontal;
unsigned int player_texture_run_up;
unsigned int player_texture_run_down;
unsigned int player_texture_attack_horizontal;
unsigned int player_texture_attack_up;
unsigned int player_texture_attack_down;
unsigned int enemy_texture;
int player_entity_id;
entity_t* player_entity;


// id = unique number for each entity.
// entity = pointer to entity that we should update
void exaple_update_function(int id, entity_t* entity) {
    
    // all entity variables can be found in entity.cpp in struct entity_t
    entity->position.x += 0.1f;
    
}

m_v2 player_cursor = {0.5,0.5};
void player_update(int id, entity_t* entity) { 
    m_v2 dir = {};
    
#define  PLAYER_ON_RUN entity->texture_scale.x = 1.0f / 4.0f;
#define PLAYER_ON_STOP_RUNNING entity->texture_scale.x = 0.25f; entity->time = 0;
    
    if(input_down(GLFW_KEY_W)){
        dir.y++;
        entity->texture = player_texture_run_up;
        PLAYER_ON_RUN;
    }
    if(input_down(GLFW_KEY_S)){
        dir.y--;
        entity->texture = player_texture_run_down;
        PLAYER_ON_RUN;
    }
    if(input_down(GLFW_KEY_D)){
        dir.x++;
        entity->scale.x = 1;
        entity->texture = player_texture_run_horizontal;
        PLAYER_ON_RUN;
    }
    if(input_down(GLFW_KEY_A)){
        dir.x--;
        entity->scale.x = -1;
        entity->texture = player_texture_run_horizontal;
        PLAYER_ON_RUN;
    }
    /*
    if(input_released(GLFW_KEY_W)){
        entity->texture = player_texture_idle_up;
        PLAYER_ON_STOP_RUNNING;
    }
    if(input_released(GLFW_KEY_S)){
        entity->texture = player_texture_idle_down;
        PLAYER_ON_STOP_RUNNING;
    }
    if(input_released(GLFW_KEY_D)){
        entity->texture = player_texture_idle_horizontal;
        PLAYER_ON_STOP_RUNNING;
    }
    if(input_released(GLFW_KEY_A)){
        entity->texture = player_texture_idle_horizontal;
        PLAYER_ON_STOP_RUNNING;
    }*/
    
    if(input_pressed(GLFW_MOUSE_BUTTON_1)){
        entity->mode = 1;
        
    }
    const float mouse_speed = 30;
    player_cursor += input_mouse_offset* mouse_speed * m_v2{1,-1} / m_v2{(float)window_x, (float)window_y};
    m_v2 cursor_ndir = m_v2_normalize(player_cursor);
    player_cursor = m_v2_lerp(player_cursor, cursor_ndir * 5, delta_time * 30);
    cursor_pos = entity->position + player_cursor;
    
    
    
    
    entity->velocity = entity->velocity / ( 1.0f + (delta_time * 12.0f));
    entity->velocity += dir * delta_time * 3.5f;
    
    if(input_pressed(GLFW_KEY_SPACE)) entity->velocity += dir * 1.8f;
    
    entity->position += entity->velocity;
    
    if(dir != m_v2{}) {
        if(entity->time > 1.0f){
            entity->texture_offset.x += 1.0f;
            entity->time = 0;
            entity->mode = 0;
        }
        entity->time += delta_time * 14;
    }else{
        entity->texture_offset.x = 1.0f;
        entity->time = 0;
    }
    
    
    
    main_camera.position = m_v2_lerp(
                                     main_camera.position,
                                     entity->position + entity->velocity * 3.0f,
                                     delta_time * 8.0f
                                     );
    
    main_camera.position = m_v2_lerp(
                                     main_camera.position,
                                     entity->position + player_cursor * 5,
                                     delta_time * 1.0f
                                     );
    
}

void projectile_update(int id, entity_t* entity) {
    entity->position += entity->velocity;
    entity->color -= m_v3{0.8,1,1} * delta_time * 0.05f;
    
    if(entity->colliding_entity != ENTITY_ID_NULL){
        // damage hit entity
    }
}

void enemy_update(int id, entity_t* entity) {
    if(entity->time > 1.0f){
        
    }
    entity->time += delta_time;
}

void game_initalize() {
    
    // exaple texture
    // first argument is name of the file. all textures should be in folder ..\cgame\build\textures\
// GL_LINEAR is smooth interpolation. for pixelart look, use GL_NEAREST
    // GL_REPEAT is wrapping
    player_texture_idle_down = texture_import("player_idle_down.png", GL_NEAREST, GL_REPEAT);
    player_texture_idle_up = texture_import("player_idle_up.png", GL_NEAREST, GL_REPEAT);
    player_texture_idle_horizontal = texture_import("player_idle_horizontal.png", GL_NEAREST, GL_REPEAT);
    
    player_texture_run_down = texture_import("player_run_down.png", GL_NEAREST, GL_REPEAT);
    player_texture_run_up = texture_import("player_run_up.png", GL_NEAREST, GL_REPEAT);
    player_texture_run_horizontal = texture_import("player_run_horizontal.png", GL_NEAREST, GL_REPEAT);
    
    player_texture_attack_down = texture_import("player_attack_down.png", GL_NEAREST, GL_REPEAT);
    player_texture_attack_up = texture_import("player_attack_up.png", GL_NEAREST, GL_REPEAT);
    player_texture_attack_horizontal = texture_import("player_attack_horizontal.png", GL_NEAREST, GL_REPEAT);
    
    enemy_texture = texture_import("test.png", GL_LINEAR, GL_REPEAT);
    
    
    // spawning entity example
    // create sort of prefab of the entity. can be reused.
    // ENTITY_DEFAULT is just data for default entity so it isnt completely broken
    entity_t e = ENTITY_DEFAULT;
    // { x, y }
    e.position = { 2, 4 };
    // add texture to entity
    e.texture = player_texture_idle_up;
    // set the update function for the entity
    e.update_func = player_update;
    // each entity also has a color
    e.color = {1,1,1};
    e.texture_scale = {0.25,0.9};
    // finally, spawn the entity
    int entity_id = entity_spawn(e);
    
    
    for(int i = 0; i < 10; i++) {
        entity_t e = ENTITY_DEFAULT;
        e.position = m_randv2() * 10;
        e.update_func = enemy_update;
        e.texture = enemy_texture;
        //entity_spawn(e);
    }
    
    
    
    for(int i = 0; i < 10; i++) {
        entity_t e = ENTITY_DEFAULT;
        e.position = m_randv2() * 10;
        e.scale = {2,2};
        e.texture = 0;
        //e.flags.collision_static = 1;
        entity_spawn(e);
    }
    
    
};
