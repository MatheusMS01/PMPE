#include "MessageQueue.h"
#include "ExecuteProgramPostponedProtocol.h"

#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char* argv[])
{
   if(argc != 3)
   {
      std::cout << "Wrong number of params!";
      return 1;
   }

   MessageQueue messageQueue(MessageQueue::MainQueueKey);

   ExecuteProgramPostponedProtocol epp;
   epp.setSeconds(std::stoi(argv[1]));
   epp.setProgramName(argv[2]);

   if(!messageQueue.write(epp.serialize(), MessageQueue::SchedulerId))
   {
      std::cout << "Failed to write." << std::endl;
   }

   return 0;
}