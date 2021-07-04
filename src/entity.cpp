
#include "entity.h"


cmap_entity entity_global_container = cmap_entity_init();



int entity_counter;
entity_t entity_spawn_buffer[200];
unsigned int entity_spawn_buffer_counter;
int entity_destroy_buffer[200];
unsigned int entity_destroy_buffer_counter;

int entity_spawn(entity_t entity_data) {
    int id = entity_counter;
    entity_counter++;
    
    cmap_entity_result_t inserted = cmap_entity_insert(&entity_global_container, id, entity_data);
    
    
    return id;
}

void entity_destroy(int entity_id) {
    
}

entity_t* entity_get(int entity_id) {
    cmap_entity_mapped_t* mapped = cmap_entity_at(&entity_global_container, entity_id);
	return mapped;
}

void entity_global_update() {
    
    
    c_foreach(i, cmap_entity, entity_global_container) {
        entity_t* e = &i.ref->second;
		if(e->update_func){
            e->update_func(i.ref->first, e);
        }
	}
    
    
}