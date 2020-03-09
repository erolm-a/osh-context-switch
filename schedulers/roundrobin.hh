/*
 * Inspired by https://gist.github.com/DanGe42/7148946 .
 * Abridged to be C++-friendly
 */

#pragma once

#include "task.hh"

namespace schedulers {
namespace roundrobin {
    void setup(int milliseconds);
    void add_task(Task&& task);

    [[noreturn]] void start();
}
}
