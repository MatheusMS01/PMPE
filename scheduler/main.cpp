#include "MessageQueue.h"

#include <iostream>
#include <string>

int main()
{
   MessageQueue messageQueue(MessageQueue::SchedulerQueueKey);

   while(true)
   {
      std::string message;
      if(messageQueue.read(message))
      {
         std::cout << "Message read: " << message << "\n";
      }
      else
      {
         std::cout << "Failed to read" << "\n";
      }
   }

   return 0;
}