#pragma once

#include <sys/types.h>
#include <sys/shm.h>

class Scheduler
{
public:
   Scheduler();
   ~Scheduler();

   int execute();
};