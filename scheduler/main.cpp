#include "MessageQueue.h"
#include "Protocol.h"

#include <iostream>
#include <string>

int main()
{
   MessageQueue messageQueue(MessageQueue::MainQueueKey);

   while(true)
   {
      std::string message;
      if(messageQueue.read(message, MessageQueue::SchedulerId))
      {
         Protocol::ParameterList parameterList;
         Protocol::parsePdu(message, parameterList);

         for(auto test : parameterList)
         {
            std::cout << test << "\n"; 
         }
      }
   }

   return 0;
}