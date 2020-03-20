#include "fifo.hh"
#include "task.hh"

#include <deque>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <csignal>

namespace schedulers
{
namespace roundrobin
{
    // list of tasks
    std::deque<Task> tasks;

    std::mutex mutex;

    // index of the current process
    int current_process;

    // system-wide signal mask.
    // The only signal captured by the context switch is SIGUSR1, used for I/O
    sigset_t set;

    // "Kernel" stack
    void *signal_stack;

    // The interrupt context
    ucontext_t signal_context;

    // The current context
    ucontext_t* cur_context;

    static void io_interrupt(int j, siginfo_t* si, void* oldcontext);
    static void setup_signals();

    void add_task(Task&& task)
    {
       tasks.emplace_back(std::move(task));
    }

    void setup()
    {
        setup_signals();
        signal_stack = new char[Task::STACKSIZE];
    }

    void setup_signals()
    {
        struct sigaction act;

        act.sa_sigaction = io_interrupt;
        sigemptyset(&act.sa_mask);
        act.sa_flags = SA_RESTART | SA_SIGINFO;

        sigemptyset(&set);
        sigaddset(&set, SIGUSR1);

        if(sigaction(SIGUSR1, &act, NULL) != 0)
        {
            perror("Signal handler");
        }
    }

    [[noreturn]] static void next_process()
    {
#if VERBOSE
        printf("scheduling out process %d\n", tasks[current_process].pid);
#endif
        // Take the first job that is available
        do
            current_process = (current_process + 1) % tasks.size();
        while (tasks[current_process].state != Task::State::READY);
#if VERBOSE
        printf("scheduling in process %d\n", tasks[current_process].pid);
#endif
        cur_context = &tasks[current_process].context;
        tasks[current_process].run();
    }


    void io_interrupt()
    {
        // Save the current context in signal_context
        getcontext(&signal_context);
        signal_context.uc_stack.ss_sp = signal_stack;
        signal_context.uc_stack.ss_size = Task::STACKSIZE;
        signal_context.uc_stack.ss_flags = 0;
        // The scheduler should not be pre-empted
        sigemptyset(&signal_context.uc_sigmask);

        // Before switching to the new context, make sure to set the task status to "WAITING"
        tasks[current_process].state = Task::State::WAITING;

        makecontext(&signal_context, next_process, 1);

        swapcontext(cur_context, &signal_context);
    }

    void done_interrupt()
    {
        // 
    }
}
}
