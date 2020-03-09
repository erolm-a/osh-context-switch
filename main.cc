#include <cstdio>
#include <poll.h>
#include "schedulers/roundrobin.hh"

using namespace schedulers;

void task1()
{
    while(true)
    {
        puts("Executing this task!");
        poll(NULL, 0, 100);

    }
}
int main()
{ 
    roundrobin::setup(100);
}
