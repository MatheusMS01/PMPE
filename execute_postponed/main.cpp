#include "MessageQueue.h"
#include "Protocol.h"

#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char* argv[])
{
   if(argc < 3)
   {
      std::cout << "Missing params!";
      return 1;
   }

   MessageQueue messageQueue(MessageQueue::MainQueueKey);

   std::string pdu;
   {
      Protocol::ParameterList parameterList;
      parameterList.resize(Protocol::EEPExpectedParameters);
      parameterList[Protocol::SecondsId] = argv[1];
      parameterList[Protocol::FileNameId] = argv[2];
      parameterList[Protocol::DestinationNodeId] = "0";

      Protocol::buildPdu(Protocol::ExecuteProgramPostponed, pdu, parameterList);
   }

   if(pdu.empty())
   {
      std::cout << "Failed to build pdu" << std::endl;
   }

   if(!messageQueue.write(pdu, MessageQueue::SchedulerId))
   {
      std::cout << "Failed to write." << std::endl;
   }

   return 0;
}