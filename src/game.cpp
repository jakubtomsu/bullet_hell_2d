
#include "game.h"
#include "graphics.h"
#include "entity.h"

unsigned int player_texture;



// id = unique number for each entity.
// entity = pointer to entity that we should update
void exaple_update_function(int id, entity_t* entity) {
    
    // ...
    
    // all entity variables can be found in entity.cpp in struct entity_t
    entity->position.x += 0.1f;
    
    // ...
    
    // bagr
}

void game_initalize() {
    
    // exaple texture
    // first argument is name of the file. all textures should be in folder ..\cgame\build\textures\
// GL_LINEAR is smooth interpolation. for pixelart look, use GL_NEAREST
    // GL_REPEAT is wrapping
    player_texture = texture_import("test.png", GL_LINEAR, GL_REPEAT);
    
    // spawning entity example
    // create sort of prefab of the entity. can be reused.
    // ENTITY_DEFAULT is just data for default entity so it isnt completely broken
    entity_t e = ENTITY_DEFAULT;
    // { x, y }
    e.position = { 2, 4 };
    e.scale = 30.5f;
    // add texture to entity
    e.texture = player_texture;
    // set the update function for the entity
    e.update_func = exaple_update_function;
    // each entity also has a color
    e.color = {1,0,1};
    // finally, spawn the entity
    int entity_id = entity_spawn(e);
    
};
