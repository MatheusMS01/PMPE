#pragma once

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <string>

class MessageQueue
{
public:
   MessageQueue(key_t key);
   ~MessageQueue();

   enum QueueKey : key_t
   {
      MainQueueKey = 120053161
   };

   enum ProcessType : unsigned int
   {
      SchedulerId = 1
   };

   bool write(const std::string& message, const unsigned int type);
   bool read(std::string& message, const unsigned int type);

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