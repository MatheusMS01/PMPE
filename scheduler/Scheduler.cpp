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

         _exit(0);
      }
      else
      {
         m_nodeMap.insert(std::make_pair(nodeId, Node::Free));
         // @TODO: Not working on OSX
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
            ExecuteProgramPostponedProtocol epp;
            epp.parse(message);
            treat(epp);
         }
         break;

         case IProtocol::NotifyScheduler:
         {
            NotifySchedulerProtocol ns;
            ns.parse(message);
            treat(ns);
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
void Scheduler::treat(ExecuteProgramPostponedProtocol& epp)
{
   sleep(epp.getDelay());
   for(auto& node : m_nodeMap)
   {
      if(node.second == Node::Busy)
      {
         // @TODO: Add to list
         std::cout << "Node " << node.first << " is busy";
         continue;
      }

      epp.setDestinationNode(node.first);

      // Write to node zero
      m_messageQueue.write(epp.serialize(), MessageQueue::SchedulerId + 1);

      node.second = Node::Busy;
   }
}


void Scheduler::treat(NotifySchedulerProtocol& ns)
{
   m_nodeMap[ns.getNodeId()] = Node::Free;

   const auto makespan = ns.getEndTime() - ns.getBeginTime();

   std::string message;
   message.append("job=" + std::to_string(ns.getNodeId()) + ", ");
   message.append("arquivo=" + ns.getProgramName() + ", ");
   message.append("delay=" + std::to_string(ns.getDelay()) + ", ");
   message.append("makespan=" + std::to_string(makespan));

   std::cout << message << "\n";
}
