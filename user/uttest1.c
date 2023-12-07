//
// Created by 佐久川泰輔 on 2023/12/05.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void foo() {
    int c = 0;
    for (;;) {
        printf("foo_(tid=%d):␣%d\n", mytid(), c);
        c += 1;
        yield();
    }
}

void bar() {
    int c = 0;
    for (;;) {
        printf("bar_(tid=%d):␣%d\n", mytid(), c);
        yield();
        c += 2;
    }
}

void baz_sub(int *cp) {
    printf("baz_(tid=%d):␣%d\n", mytid(), *cp);
    yield();
    *cp += 3;
}

void baz() {
    int c = 0;
    for (;;) {
        baz_sub(&c);
        baz_sub(&c);
    }
}

int main() {
    make_uthread(foo);
    make_uthread(bar);
    make_uthread(baz);
    start_uthreads();
    exit(0);
}