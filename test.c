#include <stdio.h>
#include <stdbool.h>

struct Person {
    int age;
    bool is_man;
};

void callee(char* msg, int num, struct Person person, int* arr) {
    printf("%s", msg);
    printf("passed num is %d\n", num);
    printf("passed person is man? %d\n", person.is_man);
    for (int i = 0; i < 3; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void callee2(long long num, int num2, long nm3, char ch){
    printf("callee 2 got called with %lld, %d, %d, %d\n", num, num2, nm3, ch);
}

int dump_pointed_val_of(int* ptr) {
    printf("pointed val : %d\n", *ptr);
}

int main() {
    callee2(5000000000000, 16, 52, 65);

    int hoge = 5;
    int *hoge_p = &hoge;
    dump_pointed_val_of(hoge_p);

    *hoge_p = 10;
    dump_pointed_val_of(hoge_p);
}