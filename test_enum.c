#include <stdio.h>

// 列挙型の定義
typedef enum {
    RED,
    GREEN,
    BLUE
} Color;

// 列挙型を引数にとる関数の定義
void printColor(Color color) {
    switch (color) {
        case RED:
            printf("Color is Red\n");
            break;
        case GREEN:
            printf("Color is Green\n");
            break;
        case BLUE:
            printf("Color is Blue\n");
            break;
        default:
            printf("Unknown Color\n");
    }
}

int main() {
    Color color = RED;
    printColor(color);  // "Color is Red" を出力

    return 0;
}