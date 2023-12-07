//
// Created by 佐久川泰輔 on 2023/12/07.
//

#ifndef XV6_RISCV_UTHREADS_H
#define XV6_RISCV_UTHREADS_H

int make_uthread(void (*fun)());
void start_uthreads();
void yield();
int mytid();

#endif //XV6_RISCV_UTHREADS_H
