#include "Scheduler.h"

#include "Node.h"
#include "Utils.h"

#include <iostream>

#include <unistd.h>

Scheduler::Scheduler()
   : m_messageQueue(MessageQueue::MainQueueKey)
   , m_nodeMap()
{
}

Scheduler::~Scheduler()
{
}

bool Scheduler::createNodes()
{
   pid_t pid;
   for(int nodeId = 0; nodeId < 16; ++nodeId)
   {
      pid = fork();

      if(pid == -1)
      {
         return false;
      }

      if(pid == 0)
      {
         Node node(nodeId);
         node.execute();

         exit(0);
      }
      else
      {
         m_nodeMap.insert(std::make_pair(nodeId, Node::Free));
         wait();
      }
   }

   return true;
}

int Scheduler::execute()
{
   if(!createNodes())
   {
      return 1;
   }

   while(true)
   {
      std::string message;
      if(!m_messageQueue.read(message, MessageQueue::SchedulerId))
      {
         continue;
      }

      switch(Utils::getProtocolId(message))
      {
         case IProtocol::ExecuteProgramPostponed:
         {
            ExecuteProgramPostponedProtocol eep;
            std::cout << message << std::endl;
            eep.parse(message);

            std::cout << "Seconds: " << eep.getSeconds() << "\n";
            std::cout << "ProgramName: " << eep.getProgramName() << "\n";
            std::cout << "DestinationNode: " << eep.getDestinationNode() << "\n";
            std::cout << "Serialized: " << eep.serialize() << "\n";
            treat(eep);
         }
         break;

         default:
         {
         }
         break;
      }
   }

   return 0;
}

// Send message to all nodes via node zero
void Scheduler::treat(ExecuteProgramPostponedProtocol& eep)
{
   for(auto& node : m_nodeMap)
   {
      if(node.second == Node::Busy)
      {
         // @TODO: Add to list
         continue;
      }

      eep.setDestinationNode(node.first);

      // Write to node zero
      m_messageQueue.write(eep.serialize(), MessageQueue::SchedulerId + 1);

      node.second = Node::Busy;
   }
}