
#ifndef ENGINE_MATH_H
#define ENGINE_MATH_H

#include <math.h>

typedef struct m_v2{
    float x;
    float y;
}m_vector2_t;

typedef struct m_transform {
    m_v2 position;
    float rotation;
} m_transform_t;

#endif