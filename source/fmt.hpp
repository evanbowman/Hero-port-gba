#pragma once

#include "string.hpp"



void english__to_string(int num, char* buffer, int base);



template <u32 length> StringBuffer<length> to_string(int num)
{
    char temp[length];
    english__to_string(num, temp, 10);

    return temp;
}



inline StringBuffer<12> stringify(s32 num)
{
    return to_string<12>(num);
}



// passthrough stringify just defined for convenience in function templates.
inline const char* stringify(const char* arg)
{
    return arg;
}



template <s32 precision, typename T, s32 buffer_size = 24>
StringBuffer<buffer_size> stringify(const FixedPoint<precision, T>& fp)
{
    StringBuffer<buffer_size> result;

    result += stringify((s32)fp.numerator());

    auto fraction = (s32)fp.denominator();
    if (fraction) {
        result += ".";
        result += stringify(fraction);
    }

    return result;
}



template <u32 size, typename Arg, typename... Args>
void make_format(StringBuffer<size>& output,
                 const char* fmt_str,
                 Arg&& arg,
                 Args&&... args)
{
    while (*fmt_str not_eq '\0') {
        if (*fmt_str == '%') {
            output += stringify(arg);
            if constexpr (sizeof...(args)) {
                make_format(output, ++fmt_str, std::forward<Args>(args)...);
                break;
            }
        } else {
            output.push_back(*fmt_str);
        }
        ++fmt_str;
    }
}



template <u32 size = 128, typename... Args>
StringBuffer<size> format(const char* fmt_str, Args&&... args)
{
    StringBuffer<size> result;
    make_format(result, fmt_str, std::forward<Args>(args)...);

    return result;
}
