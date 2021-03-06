
#include "game.h"
#include "graphics.h"
#include "entity.h"
#include "input.h"
#include "engine.h"
#include "m_math.h"

unsigned int player_texture_run_horizontal;
unsigned int player_texture_run_up;
unsigned int player_texture_run_down;
unsigned int player_fist_texture;
unsigned int enemy_texture;
unsigned int projectile_texture;
unsigned int health_texture;
unsigned int explosion_texture;
int player_entity_id = -1;
entity_t* player_entity;


// id = unique number for each entity.
// entity = pointer to entity that we should update
void exaple_update_function(int id, entity_t* entity) {
    
    // all entity variables can be found in entity.cpp in struct entity_t
    entity->position.x += 0.1f;
    
}

/* PLAYER FIST */
m_v2 camera_shake;
float camera_shake_strength;
const float camera_shake_length = 6.0f;
const float camera_shake_freq = 2;
m_v2 player_cursor = {0.5,0.5};
float fist_time = 0;
const float mouse_speed = 0.01;

void player_fist_update(int id, entity_t* entity) {
    
    if(!player_entity) return;
    
    fist_time -= delta_time;
    entity->velocity = entity->velocity / (1.0f + (delta_time * 25));
    if(input_pressed(GLFW_MOUSE_BUTTON_1)) {
        entity->velocity += m_v2_normalize(player_cursor) * 150;
        camera_shake_strength += 1;
        entity->color *= 8;
        fist_time = 0.2f;
    }
    entity->color = m_v3_lerp(entity->color, {1,1,1}, delta_time * 10);
    
    /*
    entity->position = m_v2_lerp(entity->position, player_entity->position + player_cursor, delta_time* 15);*/
    
    entity->velocity += ((player_entity->position + player_cursor) - entity->position) * 3;
    entity->velocity += player_entity->velocity * 0.05f;
    
    entity->position += entity->velocity * delta_time;
}

void player_fist_on_collision(int id, entity_t* entity, int other_id, entity_t* other_entity) {
    if(other_id == player_entity_id || other_entity->flags.collision_static || fist_time < 0) return;
    
    other_entity->velocity += m_v2_normalize(player_cursor) * 5;
    other_entity->color *= 4;
}

/* PLAYER BULLET */

float player_bullet_timer = 0;

void player_bullet_update(int id, entity_t* entity) {
    entity->position += entity->velocity * delta_time;
    entity->time+=delta_time;
    const float lifetime = 0.25f;
    if(entity->time > lifetime) entity_destroy(id);
    entity->texture_offset.x = floorf((entity->time / lifetime) * 4);
}

void player_bullet_on_collision(int id, entity_t* entity, int other_id, entity_t* other_entity) {
    
}

/* PLAYER */

void player_update(int id, entity_t* entity) { 
    m_v2 dir = {};
    
#define  PLAYER_ON_RUN entity->texture_scale.x = 1.0f / 4.0f;
#define PLAYER_ON_STOP_RUNNING entity->texture_scale.x = 0.25f; entity->time = 0;
    
    
    // input
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
    
    
    
    // update velocity
    entity->velocity = entity->velocity / ( 1.0f + (delta_time * 18.0f));
    entity->velocity += dir * delta_time * 270.0f;
    
    if(input_pressed(GLFW_KEY_SPACE)) entity->velocity += dir * 80.0f;
    // apply velocity
    entity->position += entity->velocity * delta_time;
    
    // anim
    if(dir != m_v2{}) {
        if(entity->time > 1.0f){
            entity->texture_offset.x += 1.0f;
            entity->time = 0;
            entity->mode = 0;
        }
        entity->time += delta_time * 14;
    }else{
        entity->texture_offset.x = entity->texture == player_texture_run_horizontal ? 0.0 : 1.0f;
        entity->time = 0;
    }
    
    // cursor
    player_cursor += input_mouse_offset* mouse_speed * m_v2{1,-1};
    float len = m_v2_length(player_cursor);
    if(len > 8) player_cursor = (player_cursor / len) * 8;
    //player_cursor = m_v2_normalize(player_cursor) * 8;
    cursor_pos = entity->position + player_cursor;
    
    
    // bullets
    if(input_pressed(GLFW_MOUSE_BUTTON_1) && player_bullet_timer > 0.2f) {
        player_bullet_timer = 0;
        
        entity_t e = ENTITY_DEFAULT;
        e.position = entity->position + m_v2_normalize(player_cursor) * 2.5;
        //e.velocity = m_v2_normalize(player_cursor) * 30;
        e.update_func = player_bullet_update;
        e.on_collision_func = player_bullet_on_collision;
        e.texture = explosion_texture;
        e.texture_scale.x = 0.25;
        e.scale = {3,3};
        entity_spawn(e);
    }
    player_bullet_timer+= delta_time;
    
    
    
    camera_shake = m_v2{
        sinf(engine_time * 20.2345 * camera_shake_freq)*2 + sinf(engine_time *camera_shake_freq * 58.2342) + cosf(-engine_time * camera_shake_freq * 234.2312) + cosf(-engine_time*camera_shake_freq * 2344.2312)/2,
        sinf(engine_time*camera_shake_freq * 18.987)*2 + sinf(-engine_time*camera_shake_freq * 73.2342) + cosf(-engine_time*camera_shake_freq * 194.2312) + cosf(-engine_time*camera_shake_freq * 2784.2312)/2,
    };
    camera_shake_strength *= 1.0f - (camera_shake_length * delta_time);
    
    
    main_camera.position = m_v2_lerp(
                                     main_camera.position,
                                     entity->position +
                                     entity->velocity * 0.1f +
                                     player_cursor * 0.1f + 
                                     camera_shake * camera_shake_strength,
                                     delta_time * 6.0f
                                     );
    
}

/* ENEMIES */
void projectile_update_1(int id, entity_t* entity) {
    entity->position += entity->velocity*delta_time;
    entity->color -= m_v3{0.8,1,1} * delta_time * 0.05;
    if (entity->collision_count > 0){
        entity_destroy(id);
    }
    
}
void projectile_on_collision(int id, entity_t* entity, int other_id, entity_t* other_entity) {
    other_entity->health--;
    printf("au---------------------------------------");
}

void enemy_update(int id, entity_t* entity) {
    if(!player_entity) return;
    
    entity->time += delta_time;
    if(entity->time > 1.0f){
        for(int i = 0; i < 1; i++) {
            entity_t e = ENTITY_DEFAULT;
            const float bullet_speed = 5;
            e.velocity = m_v2_normalize(player_entity->position - entity->position)*bullet_speed;
            e.position =  entity->position+e.velocity/bullet_speed;
            e.update_func = projectile_update_1;
            e.texture = projectile_texture;
            e.on_collision_func = projectile_on_collision;
            
            entity_spawn(e);
            entity->time = 0;
        }
    }
    entity->texture_offset.x = (int)roundf(entity->time * 4) % 2 == 0? 1.0f : 0.0f;
    entity->color = m_v3{1,1,1} * (1.0f + (entity->time * entity->time * entity->time ));
}


/* GAME */

void game_initialize() {
    
#define TEX_INTERPOLATION GL_NEAREST
    
    player_texture_run_down = texture_import("player_run_down.png", TEX_INTERPOLATION, GL_REPEAT);
    player_texture_run_up = texture_import("player_run_up.png", TEX_INTERPOLATION, GL_REPEAT);
    player_texture_run_horizontal = texture_import("player_run_horizontal.png", TEX_INTERPOLATION, GL_REPEAT);
    
    player_fist_texture = texture_import("fist.png", TEX_INTERPOLATION, GL_REPEAT);
    
    enemy_texture = texture_import("enemy1.png", TEX_INTERPOLATION, GL_REPEAT);
    
    projectile_texture = texture_import("projectile.png", TEX_INTERPOLATION, GL_REPEAT);
    health_texture = texture_import("cross.png", TEX_INTERPOLATION, GL_REPEAT);
    explosion_texture = texture_import("explosion.png", TEX_INTERPOLATION, GL_REPEAT);
    
}

void vel_update(int id, entity_t* entity) {
    entity->velocity = entity->velocity / (1.0f + (delta_time * 4));
    entity->position += entity->velocity * delta_time;
}

void game_load_level() {
    
    // player
    entity_t e = ENTITY_DEFAULT;
    e.position = { 0,-4 };
    e.texture = player_texture_run_up;
    e.update_func = player_update;
    e.color = {1,1,1};
    e.texture_scale = {0.25,1};
    e.health = 4;
    player_entity_id = entity_spawn(e);
    
    // player fist
    /*
    entity_t player_f = ENTITY_DEFAULT;
    player_f.position = e.position + m_v2{0,1};
    player_f.texture = player_fist_texture;
    player_f.update_func = player_fist_update;
    player_f.on_collision_func = player_fist_on_collision;
    entity_spawn(player_f);
    */
    
    // enemies
    for(int i = 0; i < 10; i++) {
        entity_t e = ENTITY_DEFAULT;
        e.position = m_randv2() * 10;
        e.update_func = enemy_update;
        e.texture = enemy_texture;
        e.texture_scale = {0.5,1.0};
        e.scale = {3,3};
        entity_spawn(e);
    }
    
    
    
    for(int i = 0; i < 10; i++) {
        entity_t e = ENTITY_DEFAULT;
        e.position = m_randv2() * 10;
        e.scale = {2,2};
        e.texture = 0;
        //e.flags.collision_static = 1;
        e.update_func = vel_update;
        //entity_spawn(e);
    }
    
};

void game_early_update() {
    
}

void game_late_update() {
    if(player_entity_id >= 0) {
        player_entity = entity_get(player_entity_id);
    }
    
    printf("player ptr = 0x%x\n",(int)player_entity);
    
}

void game_on_render_update() {
    
    for(int i = 0; i < player_entity->health; i++) {
        draw_quad(
                  camera_ndc_to_world(&main_camera, { -0.95f + (float)i / 15.0f, 0.95}),
                  {(float)main_camera.distance * 0.04f,(float)main_camera.distance * 0.04f},
                  health_texture,
                  {1,1},
                  {},
                  {0.8,0.1,0.2}
                  );
    }
}