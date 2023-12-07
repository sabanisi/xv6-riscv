//
// Created by 佐久川泰輔 on 2023/12/08.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/uthreads.h"

static int hoge = 0;

void foo() {
    for (;;) {
        printf("foo (tid=%d): %d\n", mytid(), hoge);
        hoge++;
        yield();
    }
}

void bar() {
    int c = 0;
    for (;;) {
        printf("bar (tid=%d): %d\n", mytid(), hoge);
        yield();
        c++;
        hoge += c;
    }
}

int main() {
    make_uthread(foo);
    make_uthread(bar);
    start_uthreads();
    printf("main (tid=%d): end\n", mytid());
    exit(0);
}