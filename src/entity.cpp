
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
    
    
}