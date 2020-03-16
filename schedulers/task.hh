#pragma once

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <ucontext.h>
#include <utility>

namespace schedulers {
struct Task {
    // process pid
    int pid;
    static int max_pid;

    // priority used internally by the scheduler
    int priority;

    // The entry point of the task.
    void (*entry_point)(void);

    // The context of the process.
    ucontext_t context;

    constexpr static size_t STACKSIZE = 4096;

    Task(void (*entry_point)(void))
        : entry_point { entry_point }
    {
        this->pid = max_pid++;
        printf("Created task %d\n", pid);
        void* stack = new char[STACKSIZE];
        getcontext(&context);

        context.uc_stack.ss_sp = stack;
        context.uc_stack.ss_size = STACKSIZE;
        context.uc_stack.ss_flags = 0;
        if (sigemptyset(&context.uc_sigmask) < 0) {
            perror("sigemptyset");
            exit(EXIT_FAILURE);
        }

        makecontext(&context, entry_point, 1);
    }

    Task(Task&& o) = default;

    /*
     * Start or resume a task.
     */
    [[noreturn]] void run()
    {
        while (true) {
            setcontext(&context);
        }
    }

    Task(const Task&) = default;
    // Avoid dangerous context sharing.
    // Task(const Task&) = delete;
    // Task& operator=(const Task&) = delete;
};
}
