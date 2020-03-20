#pragma once

namespace schedulers
{
namespace fifo
{
    void setup();
    void add_task(Task&& task);

    [[noreturn]] void start();
}
}
