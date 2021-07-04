
#ifndef ENTITY_H
#define ENTITY_H

#include "m_math.h"
#include "cmap.h"

struct entity_t;

enum collision_mode {
    ENTER,
    STAY,
    LEAVE
};

typedef void (*entity_update_func)(int, entity_t*);
typedef void (*entity_collision_func)(int, entity_t*, collision_mode);

typedef struct entity_t {
    
    m_v2 position;
    float scale;
    m_v3 color;
    unsigned int texture;
    float health;
    
    entity_update_func update_func;
    entity_collision_func col_func;
    
} entity_t;

#define ENTITY_DEFAULT (entity_t{{},1,{1,1,1},0,10})

using_cmap_3(entity, int, entity_t);
extern cmap_entity entity_global_container;


int entity_spawn(entity_t entity_data);
void entity_destroy(int entity_id);
entity_t* entity_get(int entity_id);
void entity_global_update();

#endif // ENTITY_H