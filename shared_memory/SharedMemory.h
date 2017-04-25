#pragma once

#include <sys/types.h>
#include <sys/shm.h>

class SharedMemory
{
public:
   static SharedMemory getInstance();
   static bool initialize();
   static void print();

private:
   SharedMemory();

   static int m_id;
   static key_t m_key;
   static int m_size;
};