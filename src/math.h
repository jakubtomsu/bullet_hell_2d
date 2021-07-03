
#ifndef ENGINE_MATH_H
#define ENGINE_MATH_H

typedef struct m_v2{
    float x;
    float y;
}m_vector2;

typedef struct m_transform {
    m_v2 position;
    float rotation;
}

#endif