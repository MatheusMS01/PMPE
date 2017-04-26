#include "MessageQueue.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
   if(argc < 2)
   {
      std::cout << "Missing params!";
      return 1;
   }
   MessageQueue messageQueue(MessageQueue::SchedulerQueueKey);


   if(!messageQueue.write(argv[1]))
   {
      std::cout << "Failed to write." << std::endl;
   }

   return 0;
}