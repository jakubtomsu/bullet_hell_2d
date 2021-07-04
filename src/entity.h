
#ifndef ENTITY_H
#define ENTITY_H

#include "m_math.h"
#include "cmap.h"

struct entity_t;

enum collision_mode {
    COLLIDING,
    NOT_COLLIDING,
    STARTED_COLLIDING,
    STOPPED_COLLIDING
};

typedef void (*entity_update_func)(int, entity_t*);

typedef struct entity_t {
    
    m_v2 position;
    float scale;
    m_v3 color;
    unsigned int texture;
    
    // collision
    collision_mode col_mode;
    int colliding_entity;
    
    //functions
    entity_update_func update_func;
    
    
    // gameplay data
    float health;
    m_v2 velocity;
    float time;
    
} entity_t;

#define ENTITY_DEFAULT (entity_t{{},1,{1,1,1},0})
#define ENTITY_ID_NULL -1

using_cmap_3(entity, int, entity_t);
extern cmap_entity entity_global_container;


int entity_spawn(entity_t entity_data);
void entity_destroy(int entity_id);
entity_t* entity_get(int entity_id);
void entity_global_update();

#endif // ENTITY_H