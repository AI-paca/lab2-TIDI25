#ifndef VEC_H
#define VEC_H

#include <cmath>

struct vec { // вектор
    float x, y;

    // Конструкторы
    vec() : x(0), y(0) {} // x, y
    vec(float x, float y) : x(x), y(y) {} 

    vec operator+(const vec& other) const { return vec(x + other.x, y + other.y); }
    vec operator-(const vec& other) const { return vec(x - other.x, y - other.y); }
    
    vec operator*(float scalar) const { return vec(x * scalar, y * scalar); }
    vec operator/(float scalar) const { return vec(x / scalar, y / scalar); }

    // Операторы присваивания
    vec& operator+=(const vec& other) { x += other.x; y += other.y; return *this; }
    vec& operator-=(const vec& other) { x -= other.x; y -= other.y; return *this; }
    vec& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }

    float length() const { return std::sqrt(x*x + y*y); }
    float lengthSq() const { return x*x + y*y; } // Быстрее, если корень не нужен

    vec normalized() const { //нормаль
        float len = length();
        if (len < 0.0001f) return vec(0, 0);
        return vec(x / len, y / len);
    }
    
    // Скалярное произведение (новый результрующий вектор)
    float dot(const vec& other) const { return x * other.x + y * other.y; }
};

// Глобальный оператор для float * Vector
inline vec operator*(float scalar, const vec& vec) { //inline позволят определить функцию в .h
    return vec * scalar;
}
#endif // VEC_H