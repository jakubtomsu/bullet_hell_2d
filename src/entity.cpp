
#include "entity.h"
#include <stdio.h>

cmap_entity entity_global_container = cmap_entity_init();

int entity_counter;
struct entity_spawing_t {
    int id;
    entity_t entity;
};
entity_spawing_t entity_spawn_buffer[200];
unsigned int entity_spawn_buffer_counter;
int entity_destroy_buffer[200];
unsigned int entity_destroy_buffer_counter;



int entity_spawn(entity_t entity_data) {
    int id = entity_counter;
    entity_counter++;
    
    entity_spawing_t sp = {id, entity_data};
    entity_spawn_buffer[entity_spawn_buffer_counter] = sp;
    entity_spawn_buffer_counter++;
    
    return id;
}

void entity_destroy(int entity_id) {
    entity_destroy_buffer[entity_destroy_buffer_counter] = entity_id;
    entity_destroy_buffer_counter++;
}

entity_t* entity_get(int entity_id) {
    cmap_entity_mapped_t* mapped = cmap_entity_at(&entity_global_container, entity_id);
	return mapped;
}

void entity_global_update() {
    
    // spawn entities
    for(int i = 0; i < entity_spawn_buffer_counter; i++){
        cmap_entity_insert(
                           &entity_global_container,
                           entity_spawn_buffer[i].id, 
                           entity_spawn_buffer[i].entity
                           );
        printf("spawned entity %i\n", entity_spawn_buffer[i].id);
    }
    entity_spawn_buffer_counter = 0;
    
    //update entities
    c_foreach(i, cmap_entity, entity_global_container) {
        entity_t* e = &i.ref->second;
		if(e->update_func){
            e->update_func(i.ref->first, e);
        }
	}
    
    // destroy entities
    for(int i = 0; i < entity_destroy_buffer_counter; i++){
        cmap_entity_erase(
                          &entity_global_container,
                          entity_destroy_buffer[i]
                          );
        printf("destroyed entity %i\n", entity_destroy_buffer[i]);
    }
    entity_destroy_buffer_counter = 0;
    
    
    
    // terribly slow collision :(
    
    c_foreach(i, cmap_entity, entity_global_container) {
        entity_t* e = &i.ref->second;
        e->collision_count = 0;
    }
    
    c_foreach(i, cmap_entity, entity_global_container) {
        entity_t* e = &i.ref->second;
		
        c_foreach(j, cmap_entity, entity_global_container) {
            entity_t* e2 = &j.ref->second;
            
            if(i.ref->first >= j.ref->first) continue;
            
            m_v2 dir = e2->position - e->position;
            
            float l = m_v2_length(dir);
            float s12 = fabsf(e->scale.x) + fabsf(e2->scale.x);
            if(l > (s12 * 0.5f)) continue;
            m_v2 ndir = (dir / l) * 0.495f;
            float penetration = s12 - l;
            m_v2 mid = m_v2_lerp(e->position, e2->position, 0.5);
            
            if(!e->flags.collision_static) e->position = mid - (ndir * penetration);
            if(!e2->flags.collision_static)  e2->position = mid + (ndir * penetration);
            
            e->collision_count++;
            e2->collision_count++;
            if(e->on_collision_func) e->on_collision_func(i.ref->first, e, j.ref->first);
            if(e2->on_collision_func) e2->on_collision_func(j.ref->first, e2, i.ref->first);
        }
    }
    
}

int collision_query_circle(m_v2 pos, float radius, collision_callback callback) {
    
}