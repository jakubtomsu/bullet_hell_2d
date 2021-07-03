

#ifndef M_MATH_H
#define M_MATH_H

#include <math.h>

typedef struct m_v2{
    float x;
    float y;
}m_vector2_t;

typedef struct m_v3{
    float x;
    float y;
    float z;
}m_vector3_t;

typedef struct m_mat4 {
    union {
        float columns[4][4];
    };
} m_matrix4x4_t;

typedef struct m_transform {
    m_v2 position;
    float rotation;
} m_transform_t;



#endif // M_MATH_H