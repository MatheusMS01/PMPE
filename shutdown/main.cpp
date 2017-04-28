#include "ShutdownProtocol.h"
#include "MessageQueue.h"

int main()
{
   ShutdownProtocol sd;
   MessageQueue messageQueue(MessageQueue::MainQueueKey);

   messageQueue.write(sd.serialize(), MessageQueue::SchedulerId); 

   return 0;
}