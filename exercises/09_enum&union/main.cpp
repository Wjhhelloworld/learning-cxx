#include "../exercise.h"

enum ColorEnum : unsigned char {
    COLOR_RED = 31,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
};

enum class Color : int {
    Red = COLOR_RED,
    Green,
    Yellow,
    Blue,
};

ColorEnum convert_by_pun(Color c) {
    union TypePun {
        ColorEnum e;
        Color c;
    };
    
    TypePun pun;
    pun.c = c;
    return pun.e;
}

int main(int argc, char **argv) {
    ASSERT(convert_by_pun(Color::Red) == COLOR_RED, "Type punning conversion");
    ASSERT(convert_by_pun(Color::Green) == COLOR_GREEN, "Type punning conversion");
    ASSERT(convert_by_pun(Color::Yellow) == COLOR_YELLOW, "Type punning conversion");
    ASSERT(convert_by_pun(Color::Blue) == COLOR_BLUE, "Type punning conversion");
    return 0;
}