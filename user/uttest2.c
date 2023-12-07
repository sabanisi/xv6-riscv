//
// Created by 佐久川泰輔 on 2023/12/08.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/uthreads.h"

void foo() {
    int c = 0;
    for (;;) {
        printf("foo (tid=%d): %d\n", mytid(), c);
        c += 1;
        yield();
        if(c > 6)uthread_exit();
    }
}
void bar() {
    int c = 0;
    for (;;) {
        printf("bar (tid=%d): %d\n", mytid(), c);
        yield();
        c += 2;
        if(c > 8)uthread_exit();
    }
}

void baz_sub(int *cp) {
    printf("baz (tid=%d): %d\n", mytid(), *cp);
    yield();
    *cp += 3;
}

void baz() {
    int c = 0;
    for (;;) {
        baz_sub(&c);
        baz_sub(&c);
        if(c > 10) uthread_exit();
    }
}

int main() {
    make_uthread(foo);
    make_uthread(bar);
    make_uthread(baz);
    start_uthreads();
    printf("main (tid=%d): end\n", mytid());
    exit(0);
}