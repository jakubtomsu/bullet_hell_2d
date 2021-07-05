
#ifndef ENTITY_H
#define ENTITY_H

#include "m_math.h"
#include "cmap.h"

struct entity_t;

typedef void (*entity_update_func)(int, entity_t*);
typedef void (*entity_on_collision_func)(int, entity_t*, int, entity_t*);

struct entity_flags_t {
    unsigned char collision_static : 1;
};

struct entity_t {
    
    m_v2 position;
    m_v2 scale;
    m_v3 color;
    unsigned int texture;
    entity_flags_t flags;
    m_v2 texture_scale;
    m_v2 texture_offset;
    
    // collision
    int collision_count;
    
    //functions
    entity_update_func update_func;
    entity_on_collision_func on_collision_func;
    
    // gameplay data
    int health;
    m_v2 velocity;
    float time;
    int step;
    int mode;
    
};

#define ENTITY_DEFAULT (entity_t{{},{1,1},{1,1,1},0,{},{1,1}})
#define ENTITY_ID_NULL -1

using_cmap_3(entity, int, entity_t);
extern cmap_entity entity_global_container;


int entity_spawn(entity_t entity_data);
void entity_destroy(int entity_id);
entity_t* entity_get(int entity_id);
void entity_global_update();
typedef void (*collision_callback)(int id, entity_t* entity);
int collision_query_circle(m_v2 pos, float radius, collision_callback callback);

#endif // ENTITY_H