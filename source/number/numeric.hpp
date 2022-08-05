////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2022  Evan Bowman
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of version 2 of the GNU General Public License as published by the
// Free Software Foundation.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 51
// Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
// GPL2 ONLY. No later versions permitted.
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "int.h"
#include <ciso646> // For MSVC. What an inept excuse for a compiler.

#include "fixnum.hpp"


#if defined(__GBA__) or defined(__NDS__)
template <typename T> using Atomic = T;
#else
#include <atomic>
template <typename T> using Atomic = std::atomic<T>;
#endif


enum class byte : u8 {};


inline u8 count_ones(u8 byte)
{
    static const u8 nibble_lut[16] = {
        0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};

    return nibble_lut[byte & 0x0F] + nibble_lut[byte >> 4];
}



inline int count_ones(u64 x)
{
    int sum = 0;

    for (int i = 0; i < 8; ++i) {
        sum += count_ones(((u8*)&x)[i]);
    }

    return sum;
}



template <typename T> struct Vec3
{
    T x = 0;
    T y = 0;
    T z = 0;
};



template <typename T> struct Vec2
{
    T x = 0;
    T y = 0;

    template <typename U> Vec2<U> cast() const
    {
        // Note: We could have used a uniform initializer here to
        // prevent narrowing, but there are cases of float->int cast
        // where one might not worry too much about a narrowing
        // conversion.
        Vec2<U> result;
        result.x = x;
        result.y = y;
        return result;
    }
};



template <typename T, typename U = T> struct Rect
{
    T x_off = 0;
    T y_off = 0;
    U w = 0;
    U h = 0;
};


template <typename T> T abs(const T& val)
{
    return (val > 0) ? val : val * -1;
}


// When you don't need an exact value, this works as a fast distance
// approximation.
template <typename T> T manhattan_length(const Vec2<T>& a, const Vec2<T>& b)
{
    return abs(a.x - b.x) + abs(a.y - b.y);
}


using Degree = u16;
using Angle = Degree;


// Note: In case I need to swap in a fixed-point class in the future.
using Float = float;



inline Vec2<Float> fvec(Vec2<Fixnum> fixed)
{
    return {fixed.x.as_float(), fixed.y.as_float()};
}



inline Vec2<s32> ivec(Vec2<Fixnum> fixed)
{
    return {fixed.x.as_integer(), fixed.y.as_integer()};
}



inline Float sqrt_approx(const Float x)
{
    constexpr Float magic = 0x5f3759df;
    const Float xhalf = 0.5f * x;

    union
    {
        float x;
        int i;
    } u;

    u.x = x;
    u.i = magic - (u.i >> 1);
    return x * u.x * (1.5f - xhalf * u.x * u.x);
}

template <typename T> Vec2<T> operator+(const Vec2<T>& lhs, const Vec2<T>& rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

template <typename T> Vec2<T> operator-(const Vec2<T>& lhs, const Vec2<T>& rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

template <typename T> Vec2<T> operator*(const Vec2<T>& lhs, const Vec2<T>& rhs)
{
    return {lhs.x * rhs.x, lhs.y * rhs.y};
}

template <typename T> Vec2<T> operator/(const Vec2<T>& lhs, const Vec2<T>& rhs)
{
    return {lhs.x / rhs.x, lhs.y / rhs.y};
}

template <typename T> Vec2<T> operator+(const Vec2<T>& lhs, const T& rhs)
{
    return {lhs.x + rhs, lhs.y + rhs};
}

template <typename T> Vec2<T> operator-(const Vec2<T>& lhs, const T& rhs)
{
    return {lhs.x - rhs, lhs.y - rhs};
}

template <typename T> Vec2<T> operator*(const Vec2<T>& lhs, const T& rhs)
{
    return {lhs.x * rhs, lhs.y * rhs};
}

template <typename T> Vec2<T> operator*(const T& rhs, const Vec2<T>& lhs)
{
    return {lhs.x * rhs, lhs.y * rhs};
}

template <typename T> Vec2<T> operator/(const Vec2<T>& lhs, const T& rhs)
{
    return {lhs.x / rhs, lhs.y / rhs};
}

template <typename T> Vec2<T> operator/(const T& rhs, const Vec2<T>& lhs)
{
    return {lhs.x / rhs, lhs.y / rhs};
}

template <typename T> bool operator==(const Vec2<T>& rhs, const Vec2<T>& lhs)
{
    return lhs.x == rhs.x and lhs.y == rhs.y;
}

template <typename T> bool operator==(const Vec3<T>& rhs, const Vec3<T>& lhs)
{
    return lhs.x == rhs.x and lhs.y == rhs.y and lhs.z == rhs.z;
}

template <typename T>
bool operator not_eq(const Vec2<T>& rhs, const Vec2<T>& lhs)
{
    return lhs.x not_eq rhs.x or lhs.y not_eq rhs.y;
}

template <typename T> T clamp(T x, T floor, T ceil)
{
    if (x < floor) {
        return floor;
    } else if (x > ceil) {
        return ceil;
    } else {
        return x;
    }
}

inline Float smoothstep(Float edge0, Float edge1, Float x)
{
    x = clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
    return x * x * (3 - 2 * x);
}

template <typename T> T interpolate(const T& a, const T& b, Float t)
{
    return a * t + (1 - t) * b;
}


inline Float ease_out(Float time, Float b, Float c, Float duration)
{
    time = time / duration - 1;
    return c * (time * time * time + 1) + b;
}

inline Float ease_in(Float time, Float b, Float c, Float duration)
{
    time /= duration;
    return c * time * time * time + b;
}


inline u8 fast_interpolate(u8 a, u8 b, u8 t)
{
    return b + (u8)(((u16)(a - b) * t) >> 8);
}


using Microseconds = s32; // Therefore, a maximum of ~2147.5 seconds will fit in
                          // this data type.


constexpr Microseconds seconds(u32 count)
{
    return count * 1000000;
}


constexpr Microseconds minutes(u8 count)
{
    return count * seconds(60);
}


constexpr Microseconds milliseconds(u32 count)
{
    return count * 1000;
}




#define INT16_BITS (8 * sizeof(int16_t))
#ifndef INT16_MAX
#define INT16_MAX ((1 << (INT16_BITS - 1)) - 1)
#endif

#define TABLE_BITS (5)
#define TABLE_SIZE (1 << TABLE_BITS)
#define TABLE_MASK (TABLE_SIZE - 1)

#define LOOKUP_BITS (TABLE_BITS + 2)
#define LOOKUP_MASK ((1 << LOOKUP_BITS) - 1)
#define FLIP_BIT (1 << TABLE_BITS)
#define NEGATE_BIT (1 << (TABLE_BITS + 1))
#define INTERP_BITS (INT16_BITS - 1 - LOOKUP_BITS)
#define INTERP_MASK ((1 << INTERP_BITS) - 1)

static constexpr const int16_t sin90[TABLE_SIZE + 1] = {
    0x0000, 0x0647, 0x0c8b, 0x12c7, 0x18f8, 0x1f19, 0x2527, 0x2b1e, 0x30fb,
    0x36b9, 0x3c56, 0x41cd, 0x471c, 0x4c3f, 0x5133, 0x55f4, 0x5a81, 0x5ed6,
    0x62f1, 0x66ce, 0x6a6c, 0x6dc9, 0x70e1, 0x73b5, 0x7640, 0x7883, 0x7a7c,
    0x7c29, 0x7d89, 0x7e9c, 0x7f61, 0x7fd7, 0x7fff};

constexpr inline s16 sine(s16 angle)
{
    s16 v0 = 0;
    s16 v1 = 0;
    if (angle < 0) {
        angle += INT16_MAX;
        angle += 1;
    }
    v0 = (angle >> INTERP_BITS);
    if (v0 & FLIP_BIT) {
        v0 = ~v0;
        v1 = ~angle;
    } else {
        v1 = angle;
    }
    v0 &= TABLE_MASK;
    v1 = sin90[v0] +
         (s16)(((int32_t)(sin90[v0 + 1] - sin90[v0]) * (v1 & INTERP_MASK)) >>
               INTERP_BITS);
    if ((angle >> INTERP_BITS) & NEGATE_BIT)
        v1 = -v1;
    return v1;
}



constexpr inline s16 cosine(s16 angle)
{
    if (angle < 0) {
        angle += INT16_MAX;
        angle += 1;
    }
    return sine(angle - s16((270.f / 360.f) * INT16_MAX));
}



using UnitVec = Vec2<Float>;


inline UnitVec direction(const Vec2<Float>& origin, const Vec2<Float>& target)
{
    const auto vec = target - origin;

    const auto magnitude = sqrt_approx(vec.x * vec.x + vec.y * vec.y);

    return vec / magnitude;
}


constexpr inline Vec2<Float> rotate(const Vec2<Float>& input, Float angle)
{
    const s16 converted_angle = INT16_MAX * (angle / 360.f);
    const Float cos_theta = Float(cosine(converted_angle)) / INT16_MAX;
    const Float sin_theta = Float(sine(converted_angle)) / INT16_MAX;

    return {input.x * cos_theta - input.y * sin_theta,
            input.x * sin_theta + input.y * cos_theta};
}


// Given an angle in degrees, return the corresponding unit vector.
inline Vec2<Float> cartesian_angle(Float degree_angle)
{
    return rotate(Vec2<Float>{1, 0}, degree_angle);
}


inline Float distance(const Vec2<Float>& from, const Vec2<Float>& to)
{
    const Vec2<float> vec = {abs(from.x - to.x), abs(from.y - to.y)};
    return sqrt_approx(vec.x * vec.x + vec.y * vec.y);
}


enum class Cardinal : u8 { north, south, west, east };



inline s32 parse_int(const char* str, u32 len)
{
    s32 n = 0;
    for (u32 i = 0; i < len; ++i) {
        n = n * 10 + (str[i] - '0');
    }
    return n;
}



inline Float fast_atan_approx(Float x)
{
    return 57.2f * // degrees per radian
               (3.14f / 4) * x -
           x * (abs(x) - 1) * (0.2447f + 0.0663f * abs(x));
}


#include <algorithm>

// This is really inefficient. Ideally, I'd convert all these numbers to
// degrees, instead of multipying by 57.2f at the end. I think there's room for
// improvement here, but math theory has never been one of my strong suits.
inline Float fast_atan2_approx(Float x, Float y)
{
    auto a = std::min(abs(x), abs(y)) / std::max(abs(x), abs(y));
    auto s = a * a;
    auto r = ((-0.0464f * s + 0.1593f) * s - 0.3276f) * s * a + a;
    if (abs(y) > abs(x)) {
        r = 1.5707f - r;
    }
    if (x < 0) {
        r = 3.1415f - r;
    }
    if (y < 0) {
        r = -r;
    }

    auto result = 57.2f * r;
    if (result < 0) {
        result = 360 + result;
    }
    return result;
}



namespace detail
{
template <std::size_t... Is> struct seq
{
};
template <std::size_t N, std::size_t... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...>
{
};
template <std::size_t... Is> struct gen_seq<0, Is...> : seq<Is...>
{
};


template <class Generator, std::size_t... Is>
constexpr auto generate_array_helper(Generator g, seq<Is...>)
    -> std::array<decltype(g(std::size_t{}, sizeof...(Is))), sizeof...(Is)>
{
    return {{g(Is, sizeof...(Is))...}};
}

template <std::size_t tcount, class Generator>
constexpr auto generate_array(Generator g)
    -> decltype(generate_array_helper(g, gen_seq<tcount>{}))
{
    return generate_array_helper(g, gen_seq<tcount>{});
}
} // namespace detail


inline constexpr auto make_rotation_lut(float v)
{
    return detail::generate_array<360>(
        [](std::size_t curr, std::size_t total) -> Vec2<Fixnum> {
            auto off = rotate({1.f, 0}, curr);
            return Vec2<Fixnum>{Fixnum(off.x), Fixnum(off.y)};
        });
};
