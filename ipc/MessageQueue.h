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

   bool write(const std::string& message);
   bool read(std::string& message);

   int getId();
private:
   struct Buffer
   {
      char data[256];
   };

   key_t m_key;
   int m_id;
   Buffer m_buffer;
};