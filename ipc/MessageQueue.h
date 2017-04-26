#pragma once

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <string>

class MessageQueue
{
public:
   MessageQueue(int key);
   ~MessageQueue();

   enum QueueKey : int
   {
      SchedulerQueueKey = 120053161
   };

   bool write(const std::string& message, const unsigned int type);
   bool read(std::string& message, const unsigned int type = 0);

   int getId();
private:
   struct Buffer
   {
      long type;
      char data[256];
   };

   key_t m_key;
   int m_id;
};