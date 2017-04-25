#include "SharedMemory.h"

#include <iostream>

int SharedMemory::m_id = 0;
key_t SharedMemory::m_key = 0;
int SharedMemory::m_size = 0;

SharedMemory::SharedMemory()
{
   m_key = 120053161;
   m_size = 2048;
   m_id = shmget(m_key, m_size, IPC_CREAT | 0666);
}

SharedMemory SharedMemory::getInstance()
{
   static SharedMemory instance;
   return instance; 
}

void SharedMemory::print()
{
   std::cout << "Shared";
}