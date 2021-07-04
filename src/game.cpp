
#include "game.h"
#include "graphics.h"
#include "entity.h"
#include "input.h"
#include "engine.h"

unsigned int player_texture_idle_up;
unsigned int player_texture_idle_down;
unsigned int player_texture_idle_horizontal;
unsigned int player_texture_run_horizontal;
unsigned int player_texture_run_up;
unsigned int player_texture_run_down;
int player_entity_id;
entity_t* player_entity;


// id = unique number for each entity.
// entity = pointer to entity that we should update
void exaple_update_function(int id, entity_t* entity) {
    
    // all entity variables can be found in entity.cpp in struct entity_t
    entity->position.x += 0.1f;
    
}

void player_update(int id, entity_t* entity) { 
    m_v2 dir = {};
    
#define  PLAYER_ON_RUN entity->texture_scale.x = 1.0f / 8.0f;
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
        entity->scale.x = -1;
        entity->texture = player_texture_run_horizontal;
        PLAYER_ON_RUN;
    }
    if(input_down(GLFW_KEY_A)){
        dir.x--;
        entity->scale.x = 1;
        entity->texture = player_texture_run_horizontal;
        PLAYER_ON_RUN;
    }
    
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
    }
    
    entity->velocity = entity->velocity / ( 1.0f + (delta_time * 14.0f));
    entity->velocity += dir * delta_time * 4;
    
    entity->position += entity->velocity;
    
    main_camera.position = m_v2_lerp(
                                     main_camera.position,
                                     entity->position + entity->velocity * 3.0f,
                                     delta_time * 0.01f
                                     );
    
    if(entity->time > 1.0f){
        entity->texture_offset.x += 1.0f;
        entity->time = 0;
    }
    entity->time += delta_time * 10;
    
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
    
};
