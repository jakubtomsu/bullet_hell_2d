
#include "game.h"
#include "graphics.h"
#include "entity.h"
#include "input.h"
#include "engine.h"
#include "m_math.h"
#include <stdlib.h>





unsigned int player_texture_run_horizontal;
unsigned int player_texture_run_up;
unsigned int player_texture_run_down;
unsigned int enemy_texture;
unsigned int enemy_2_texture;
unsigned int health_texture;
unsigned int explosion_texture;
unsigned int explosion_lowres_texture;
unsigned int projectile_texture;
unsigned int pillar_texture;
unsigned int death_screen_texture;
unsigned int win_screen_texture;


int player_entity_id = -1;
entity_t* player_entity;


m_v2 camera_shake;
float camera_shake_strength;
const float camera_shake_length = 0.1f;
const float camera_shake_freq = 4;
m_v2 player_cursor = {0.5,0.5};
const float mouse_speed = 0.02;
const float player_speed = 280;
float player_speed_mul = 0;


int enemy_count = 0;





// id = unique number for each entity.
// entity = pointer to entity that we should update
void exaple_update_function(int id, entity_t* entity) {
    
    // all entity variables can be found in entity.cpp in struct entity_t
    entity->position.x += 0.1f;
    
}



/* EXPLOSION */

float player_bullet_timer = 0;

void explosion_update(int id, entity_t* entity) {
    entity->time+=delta_time;
    const float lifetime = 0.15f;
    if(entity->time > lifetime) entity_destroy(id);
    entity->texture_offset.x = m_clampf(floorf((entity->time / lifetime) * 4),0.0,3.0);
}

void explosion_on_collision(int id, entity_t* entity, int other_id, entity_t* other_entity) {
    other_entity->velocity += m_v2_normalize(other_entity->position - entity->position) * 25;
}

void explosion_spawn(m_v2 position, float scale, m_v3 color) {
    
    entity_t e = ENTITY_DEFAULT;
    e.position = position;
    //e.velocity = m_v2_normalize(player_cursor) * 30;
    e.update_func = explosion_update;
    e.on_collision_func = explosion_on_collision;
    e.texture = scale < 1.0f ? explosion_lowres_texture : explosion_texture;
    e.texture_scale.x = 0.25;
    e.scale = {scale, scale};
    e.flags.collision_static = 1;
    entity_spawn(e);
    camera_shake_strength += 0.1f;
}

/* PROJECTILES */

void projectile_update(int id, entity_t* entity) {
    entity->position += entity->velocity * delta_time;
    entity->time -= delta_time;
    if(entity->time < 0) entity_destroy(id);
}

void projectile_on_collision(int id, entity_t* entity, int other_id, entity_t* other_entity) {
    entity_destroy(id);
    explosion_spawn(entity->position, entity->scale.x, entity->color);
    other_entity->health--;
}

void projectile_spawn(m_v2 pos, m_v2 vel, float scale, float duration, m_v3 color) {
    entity_t e = ENTITY_DEFAULT;
    e.position = pos;
    e.velocity = vel;
    e.scale = {scale, scale};
    e.color = color;
    e.update_func = projectile_update;
    e.on_collision_func = projectile_on_collision;
    e.texture_scale.x = 0.25;
    e.texture = explosion_texture;
    e.time = duration;
    entity_spawn(e);
    camera_shake_strength += 0.05f;
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
    entity->velocity += dir * delta_time * player_speed * player_speed_mul;
    player_speed_mul += (1.0f - player_speed_mul) * (delta_time * 4);
    
    if(input_pressed(GLFW_KEY_SPACE)){
        entity->velocity += dir * 50.0f;
        explosion_spawn(entity->position, 3.0f, {1,1,1});
        camera_shake_strength += 4.0f;
    }
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
    if(len > 6) player_cursor = (player_cursor / len) * 6;
    main_camera.distance = M_LERP(main_camera.distance, 19 + len, delta_time * 2);
    //player_cursor = m_v2_normalize(player_cursor) * 8;
    cursor_pos = entity->position + player_cursor;
    
    
    // bullets
    if(input_down(GLFW_MOUSE_BUTTON_1) && player_bullet_timer > 0.22f) {
        player_bullet_timer = 0;
        camera_shake_strength += 1.0f;
        projectile_spawn(
                         entity->position + m_v2_normalize(player_cursor) * 1.5f,
                         m_v2_normalize(player_cursor) * 45,
                         1,
                         1.8,
                         {1,0.9,0.2}
                         );
    }
    player_bullet_timer+= delta_time;
    
    
    if(input_pressed(GLFW_MOUSE_BUTTON_2)) {
        explosion_spawn(
                        entity->position + m_v2_normalize(player_cursor) * 3.0f,
                        4.0f,
                        {0.9,0.9,0.9}
                        );
    }
    
    
    camera_shake = m_v2{
        sinf(engine_time * 20.2345 * camera_shake_freq)*2 + sinf(engine_time *camera_shake_freq * 58.2342) + cosf(-engine_time * camera_shake_freq * 234.2312) + cosf(-engine_time*camera_shake_freq * 2344.2312)/2,
        sinf(engine_time*camera_shake_freq * 18.987)*2 + sinf(-engine_time*camera_shake_freq * 73.2342) + cosf(-engine_time*camera_shake_freq * 194.2312) + cosf(-engine_time*camera_shake_freq * 2784.2312)/2,
    };
    camera_shake_strength /= 1.0f + (delta_time / camera_shake_length);
    
    
    main_camera.position = m_v2_lerp(
                                     main_camera.position,
                                     entity->position +
                                     entity->velocity * 0.25f +
                                     player_cursor * 0.8f+ 
                                     camera_shake * camera_shake_strength,
                                     delta_time * 4
                                     );
    
}

/* PICK UPS */
void pickup_on_collision(int id, entity_t* entity, int other_id, entity_t* other_entity)
{
    if (other_entity == player_entity){
        player_entity->health++;
        entity_destroy(id);
    }
}

/* ENEMIES */

void projectile_spawn_circle(m_v2 pos, float speed, float scale, float duration, m_v3 color, float radius, unsigned int count) {
    for(int i = 0; i < count; i++) {
        float t = (float)i / (count) * M_PI_F * 2;
        m_v2 dir =  m_v2{sinf(t), cosf(t)};
        projectile_spawn(
                         pos + dir * radius,
                         dir * speed,
                         scale,
                         duration,
                         color
                         );
    }
}


//enemy
void enemy_default_update(int id, entity_t* entity) {
    //destroy entity on 0 health
    if (entity->health <= 0){
        //health pick up
        if( m_rand01() < 0.3f) {
            entity_t e = ENTITY_DEFAULT;
            e.position = entity->position;
            e.texture = health_texture;
            e.color = {0.8,0.1,0.2};
            e.scale = {1,1};
            e.on_collision_func = pickup_on_collision;
            entity_spawn(e);
        }
        
        enemy_count--;
        entity_destroy(id);
        //spawn_explosion(entity->position,1);
    }
    
    entity->time += delta_time;
    
    entity->texture_offset.x = (int)roundf(entity->time * 4) % 2 == 0? 1.0f : 0.0f;
    entity->color =m_v3_lerp(
                             m_v3{1,1,1} * (1.0f + m_clampf(entity->time * entity->time * entity->time, 0.0, 0.8f)),
                             m_v3{0.9,0.1,0.2},
                             1.0f / (float)entity->health
                             );
}

void enemy_circler_update(int id, entity_t* entity) {
    enemy_default_update(id, entity);
    
    if(entity->time > 1.0f){
        const float bullet_speed = 19;
        projectile_spawn_circle(
                                entity->position,
                                bullet_speed,
                                2,
                                6,
                                {0.7,0.2,0.3},
                                4,
                                6
                                );
        entity->time = 0;
    }
    
}

void enemy_sniper_update(int id, entity_t* entity) {
    enemy_default_update(id, entity);
    /*if(entity->texture_offset.x != 0 && entity->time >= 0.25){
        entity->texture_offset.x -= -0.2;
        entity->time = 0;
    }
*/
    if(entity->time > 0.6f) {
        const float bullet_speed = 16;
        m_v2 dir = m_v2_normalize(player_entity->position - entity->position);
        projectile_spawn(
                         entity->position + dir * 2.5,
                         dir * bullet_speed,
                         1.8,
                         3,
                         {0.9,0.5,0.1}
                         );
        entity->time = 0;
    }
    
}

void enemy_spawner_run(){
    if (enemy_count <= 0){
        // enemies
        
        for(int i = 0; i < 6; i++) {
            entity_t e = ENTITY_DEFAULT;
            e.position = m_randv2() * 40;
            e.update_func = enemy_circler_update;
            e.texture = enemy_2_texture;
            e.health = 6;
            enemy_count++;
            e.texture_scale = {0.5,1.0};
            e.scale = {2,2};
            e.time = -m_randn() - 2;
            entity_spawn(e);
        }
        
        for(int i = 0; i < 4; i++) {
            entity_t e = ENTITY_DEFAULT;
            e.position = m_randv2() * 40;
            e.update_func = enemy_sniper_update;
            e.texture = enemy_texture;
            e.health = 3;
            enemy_count++;
            e.texture_offset.x = 0.8;
            e.texture_scale = {0.2,1.0};
            e.scale = {2,2};
            e.time = -m_randn() - 2;
            entity_spawn(e);
        }
        
    }
}

/* GAME */

void game_initialize() {
    
#define TEX_INTERPOLATION GL_NEAREST
    
    player_texture_run_down = texture_import("player_run_down.png", TEX_INTERPOLATION, GL_REPEAT);
    player_texture_run_up = texture_import("player_run_up.png", TEX_INTERPOLATION, GL_REPEAT);
    player_texture_run_horizontal = texture_import("player_run_horizontal.png", TEX_INTERPOLATION, GL_REPEAT);
    
    enemy_texture = texture_import("enemy1.png", TEX_INTERPOLATION, GL_REPEAT);
    enemy_2_texture = texture_import("enemy2.png", TEX_INTERPOLATION, GL_REPEAT);
    
    projectile_texture = texture_import("projectile.png", TEX_INTERPOLATION, GL_REPEAT);
    health_texture = texture_import("cross.png", TEX_INTERPOLATION, GL_REPEAT);
    explosion_texture = texture_import("explosion.png", TEX_INTERPOLATION, GL_REPEAT);
    explosion_lowres_texture = texture_import("explosion_lowres.png", TEX_INTERPOLATION, GL_REPEAT);
    pillar_texture= texture_import("pillar.png", TEX_INTERPOLATION, GL_REPEAT);
    death_screen_texture = texture_import("death_screen.png", TEX_INTERPOLATION, GL_REPEAT);
    win_screen_texture = texture_import("win_screen.png", TEX_INTERPOLATION, GL_REPEAT);
    
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
    e.health = 3;
    player_entity_id = entity_spawn(e);
    /*
    // player fist
    entity_t player_f = ENTITY_DEFAULT;
    player_f.position = e.position + m_v2{0,1};
    player_f.texture = player_fist_texture;
    player_f.update_func = player_fist_update;
    player_f.on_collision_func = player_fist_on_collision;
    entity_spawn(player_f);
    */
    
    
    for(int i = 0; i < 20; i++) {
        entity_t e = ENTITY_DEFAULT;
        e.position = m_randv2() * 30;
        e.scale = {4,4};
        e.texture = pillar_texture;
        e.flags.collision_static = 1;
        entity_spawn(e);
    }
    
    
    // enemy spawner
    enemy_spawner_run();
    
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
    
    
    if(player_entity != NULL && player_entity->health <= 0){
        draw_quad(
                  main_camera.position,
                  {(float)main_camera.distance,0.5f * (float)main_camera.distance},
                  death_screen_texture,
                  {1,1},
                  {},
                  {0.8,0.1,0.2}
                  );
        
        engine_should_quit = true;
    }
    
    
    if(enemy_count <= 0){
        draw_quad(
                  main_camera.position,
                  {(float)main_camera.distance,0.5f * (float)main_camera.distance},
                  win_screen_texture,
                  {1,1},
                  {},
                  {0.1,0.7,0.3}
                  );
        
        engine_should_quit = true;
    }
}