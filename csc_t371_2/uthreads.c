//
// Created by 佐久川泰輔 on 2023/12/05.
//

#include "kernel/types.h"
#include "user/user.h"
#include "user/uthreads.h"


#define STACK_DEPTH 512
#define MAX_THREADS 4

#define UNUSED 0
#define RUNNABLE 1
#define RUNNING 2

struct context {
    uint64 ra;
    uint64 sp;

    // callee-saved
    uint64 s0;
    uint64 s1;
    uint64 s2;
    uint64 s3;
    uint64 s4;
    uint64 s5;
    uint64 s6;
    uint64 s7;
    uint64 s8;
    uint64 s9;
    uint64 s10;
    uint64 s11;
};

struct uthread {
    int tid;
    int state;
    uint64 stack[STACK_DEPTH];
    struct context my_context;

    void (*fun)();
};

void swtch(struct context *, struct context *);

struct uthread uthreads[MAX_THREADS];
struct uthread *current_thread;
struct context start_context;

int make_uthread(void (*fun)()) {
    int empty = -1;
    for (int i = 0; i < MAX_THREADS; i++) {
        if (uthreads[i].state == UNUSED) {
            empty = i;
            break;
        }
    }

    if(empty == -1) {
        return -1;
    }

    struct uthread *thread = malloc(sizeof(struct uthread));
    thread->tid = empty;
    thread->state = RUNNABLE;
    thread->fun = fun;
    thread->my_context.ra = (uint64) fun;
    thread->my_context.sp = (uint64)(thread->stack + STACK_DEPTH);
    uthreads[empty] = *thread;

    return thread->tid;
}

void start_uthreads() {
    current_thread = &uthreads[0];
    current_thread->state = RUNNING;

    start_context.ra = (uint64) start_uthreads;
    start_context.sp = (uint64)(current_thread->stack + STACK_DEPTH);
    swtch(&start_context, &current_thread->my_context);
}

void scheduler() {
    struct uthread *t = 0;
    struct uthread *prev = current_thread;
    int i;

    for (i = current_thread->tid + 1; i <= current_thread->tid + MAX_THREADS; i++) {
        if (i >= MAX_THREADS) i -= MAX_THREADS;
        if (uthreads[i].state == RUNNABLE) {
            t = &uthreads[i];
            break;
        }

        if (i == current_thread->tid) {
            swtch(&prev->my_context, &start_context);
            return;
        }
    }

    t->state = RUNNING;
    if (t != current_thread) {
        current_thread = t;
        swtch(&prev->my_context, &current_thread->my_context);
    }
}

void yield() {
    current_thread->state = RUNNABLE;
    scheduler();
}

int mytid() {
    return current_thread->tid;
}

void uthread_exit() {
    current_thread->state = UNUSED;
    free(current_thread);
    scheduler();
}

