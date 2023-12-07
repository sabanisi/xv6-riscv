//
// Created by 佐久川泰輔 on 2023/12/05.
//

#include "kernel/types.h"
#include "user/user.h"
#include "user/uthreads.h"


#define STACK_SIZE 4096
#define MAX_THREADS 10000

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
    uint64 stack[STACK_SIZE];
    struct context my_context;
    void (*fun)();
};

void swtch(struct context*, struct context*);

struct uthread uthreads[MAX_THREADS];
struct uthread *current_thread;
int current_tid;

int make_uthread(void (*fun)()){
   struct uthread *t;
   printf("A");
    for(t = uthreads; t < uthreads + MAX_THREADS; t++){
        printf("B");
        if(t->state == UNUSED){
            t->tid = current_tid++;
            t->state = RUNNABLE;
            t->fun = fun;
            t->my_context.ra = (uint64)fun;
            t->my_context.sp = (uint64)(t->stack + STACK_SIZE);
            return t->tid;
        }
    }
    return -1;
}

void start_uthreads(){
    current_thread = uthreads;
    current_thread->state = RUNNING;
    current_thread->fun();
}

void yield(){
    struct uthread *t;
    current_thread->state = RUNNABLE;

    //scheduler
    for(t = current_thread + 1; t< current_thread + MAX_THREADS; t++){
        if(t->state == RUNNABLE){
            current_thread = t;
            current_thread->state = RUNNING;
            swtch(&t->my_context, &current_thread->my_context);
            return;
        }
    }
}

int mytid(){
    return current_thread->tid;
}


