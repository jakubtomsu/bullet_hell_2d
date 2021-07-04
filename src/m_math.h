

#ifndef M_MATH_H
#define M_MATH_H

#include <math.h>
#include <stdlib.h>

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

#define M_MATH_INLINE static inline

M_MATH_INLINE float m_maxf(float a, float b) {return (a < b ? a : b);}
M_MATH_INLINE float m_minf(float a, float b) { return (a > b ? a : b); }
M_MATH_INLINE float m_clampf(float a, float min, float max) { return (a > min ? (a < max ? a : max) : min); }
M_MATH_INLINE float m_rand01() { return ((float)rand() / (float)RAND_MAX); }
M_MATH_INLINE float m_randn() {return ((m_rand01() * 2) - 1.0f); }
M_MATH_INLINE m_v2 m_randv2() {return {m_randn(), m_randn()}; }

M_MATH_INLINE m_v2 operator+ (m_v2 left, m_v2 right) { return { left.x + right.x, left.y + right.y }; }
M_MATH_INLINE m_v3 operator+ (m_v3 left, m_v3 right) { return { left.x + right.x, left.y + right.y, left.z + right.z }; }
M_MATH_INLINE m_v2& operator+= (m_v2& left, m_v2 right) { return (left = left + right); }
M_MATH_INLINE m_v3& operator+= (m_v3& left, m_v3 right) { return (left = left + right); }

M_MATH_INLINE m_v2 operator- (m_v2 left, m_v2 right) { return { left.x - right.x, left.y -  right.y}; }
M_MATH_INLINE m_v3 operator- (m_v3 left, m_v3 right) { return { left.x - right.x, left.y - right.y, left.z - right.z }; }
M_MATH_INLINE m_v2& operator-= (m_v2& left, m_v2 right) { return (left = left - right); }
M_MATH_INLINE m_v3& operator-= (m_v3& left, m_v3 right) { return (left = left - right); }

M_MATH_INLINE m_v2 operator* (m_v2 left, m_v2 right) { return { left.x * right.x, left.y * right.y }; }
M_MATH_INLINE m_v3 operator* (m_v3 left, m_v3 right) { return { left.x * right.x, left.y * right.y, left.z * right.z }; }
M_MATH_INLINE m_v2 operator* (m_v2 left, float right) { return {  left.x * right, left.y * right }; }
M_MATH_INLINE m_v3 operator* (m_v3 left, float right) { return {  left.x * right, left.y * right, left.z * right }; }
M_MATH_INLINE m_v2& operator*= (m_v2& left, m_v2 right){ return { left = left * right }; }
M_MATH_INLINE m_v3& operator*= (m_v3& left, m_v3 right){ return { left = left * right }; }
M_MATH_INLINE m_v2& operator*= (m_v2& left, float right){ return { left = left * right }; }
M_MATH_INLINE m_v3& operator*= (m_v3& left, float right){ return { left = left * right }; }

M_MATH_INLINE m_v2 operator/ (m_v2 left, m_v2 right) { return { left.x / right.x, left.y / right.y };  }
M_MATH_INLINE m_v3 operator/ (m_v3 left, m_v3 right) { return { left.x / right.x, left.y / right.y, left.z / right.z  }; }
M_MATH_INLINE m_v2 operator/ (m_v2 left, float right) { return {  left.x / right, left.y / right }; }
M_MATH_INLINE m_v3 operator/ (m_v3 left, float right) { return {  left.x / right, left.y / right, left.z / right }; }

M_MATH_INLINE bool operator== (m_v2 left, m_v2 right) { return (left.x == right.x && left.y == right.y); }
M_MATH_INLINE bool operator== (m_v3 left, m_v3 right) { return  (left.x == right.x && left.y == right.y && left.z == right.z); }

M_MATH_INLINE bool operator!= (m_v2 left, m_v2 right) { return !(left == right); }
M_MATH_INLINE bool operator!= (m_v3 left, m_v3 right) { return !(left == right); }

M_MATH_INLINE m_v2 m_v2_lerp(m_v2 a, m_v2 b, float time) { return ((a * time) + (b * (1.0f - time))); }
M_MATH_INLINE float m_v2_lengthsq(m_v2 a) { return ((a.x * a.x) + (a.y * a.y)); }
M_MATH_INLINE float m_v2_length(m_v2 a) { return (sqrtf((a.x * a.x) + (a.y * a.y)));}



#endif // M_MATH_H