#pragma once

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <string>

class MessageQueue
{
public:
   explicit MessageQueue(key_t key);
   ~MessageQueue() = default;

   enum QueueKey : key_t
   {
      MainQueueKey = 120053161
   };

   enum ProcessType : unsigned int
   {
      SchedulerId = 1,
      NodeZeroId  = SchedulerId + 1,
   };

   bool write(const std::string& message, const unsigned int type);
   bool read(std::string& message, const unsigned int type);
   bool remove();

   msglen_t getLenght(); /* Maximum number of bytes allowed in queue */
   unsigned long getBytes(); /* Current number of bytes in queue (nonstandard) */

private:
   struct Buffer
   {
      long type;
      char data[256];
   };

   key_t m_key;
   int m_id;
};