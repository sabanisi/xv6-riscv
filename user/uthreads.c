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
#define SLEEP 3

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

struct uthread{
    int tid;
    int state;
    uint64 stack[STACK_DEPTH];
    struct context my_context;
    void (*fun)();
};

void swtch(struct context*, struct context*);

struct uthread uthreads[MAX_THREADS];
struct uthread *current_thread;
int current_tid;

int make_uthread(void (*fun)()){
    if(current_tid >= MAX_THREADS) return -1;

    struct uthread *thread = malloc(sizeof(struct uthread));
    thread->tid = current_tid;
    thread->state = RUNNABLE;
    thread->fun = fun;
    thread->my_context.ra = (uint64)fun;
    thread->my_context.sp = (uint64)(thread->stack + STACK_DEPTH);
    uthreads[current_tid] = *thread;

    current_tid++;
    return thread->tid;
}

void start_uthreads(){
    current_thread = &uthreads[0];
    current_thread->state = RUNNING;
    current_thread->fun();
}

void yield(){
    struct uthread *t = 0;
    struct uthread *prev = current_thread;
    int i;
    current_thread->state = RUNNABLE;

    //scheduler
    for(i = current_thread->tid+1; i <= current_tid + MAX_THREADS; i++){
        if(i >= MAX_THREADS) i -= MAX_THREADS;
        if(uthreads[i].state == RUNNABLE) {
            t = &uthreads[i];
            break;
        }
    }

    t->state = RUNNING;
    if(t!=current_thread){
        current_thread = t;
        swtch(&prev->my_context, &current_thread->my_context);
    }
}

int mytid(){
    return current_thread->tid;
}


