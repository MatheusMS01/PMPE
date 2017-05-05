#include "Scheduler.h"

#include "Node.h"
#include "Utils.h"

#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <signal.h>
#include <ctime>
#include <iomanip>

Scheduler::Scheduler()
   : m_messageQueue(MessageQueue::MainQueueKey)
   , m_nodeMap()
   , m_pendingExecutionList()
   , m_childPIDList()
   , m_executionLogList()
   , m_shutdown(false)
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
         m_childPIDList.push_back(pid);
         m_nodeMap.insert(std::make_pair(nodeId, Node::Free));
         // @FIXME: Not working on OSX
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

         case IProtocol::Shutdown:
         {
            ShutdownProtocol sd;
            treat(sd);
         }
         break;

         default:
         {
         }
         break;
      }

      if(m_shutdown)
      {
         const auto busyNode = find_if(m_nodeMap.begin(), m_nodeMap.end(), [] (const std::pair<int, int>& node)
         {
            return node.second == Node::Busy;
         });

         const auto canShutdown = (busyNode == m_nodeMap.end());

         if(canShutdown)
         {
            // Kill all children processes
            for(const auto childPID : m_childPIDList)
            {
               kill(childPID, SIGTERM);
            }

            // @TODO: Show programs that were not executed

            printStatistics();

            m_messageQueue.remove();

            break; // while
         }
      }
   }

   return 0;
}

void Scheduler::treat(ExecuteProgramPostponedProtocol& epp)
{
   if(m_shutdown)
   {
      // m_pendingExecutionList.push_back(epp);
      return;
   }

   epp.setSubmittalTime(time(NULL));
   sleep(epp.getDelay());

   for(auto& node : m_nodeMap)
   {
      epp.setDestinationNode(node.first);

      if(node.second == Node::Busy)
      {
         m_pendingExecutionList.push_back(epp);
         continue;
      }

      executeProgramPostponed(epp);
   }
}


void Scheduler::treat(const NotifySchedulerProtocol& ns)
{
   m_nodeMap[ns.getNodeId()] = Node::Free;

   m_executionLogList.push_back(ns);

   const auto makespan = ns.getEndTime() - ns.getBeginTime();

   std::string message;
   message.append("job=" + std::to_string(ns.getNodeId()) + ", ");
   message.append("arquivo=" + ns.getProgramName() + ", ");
   message.append("delay=" + std::to_string(ns.getDelay()) +
                  " segundo" + (ns.getDelay() > 1 ? "s" : "") +  ", ");

   message.append("makespan=" + std::to_string(makespan) + 
                  " segundo" + (makespan > 1 ? "s" : ""));

   std::cout << message << "\n";

   for(size_t it = 0; it < m_pendingExecutionList.size(); ++it)
   {
      if(m_pendingExecutionList.at(it).getDestinationNode() == ns.getNodeId())
      {
         executeProgramPostponed(m_pendingExecutionList.at(it));
         m_pendingExecutionList.erase(m_pendingExecutionList.begin() + it);
         break;
      }
   }
}

void Scheduler::treat(const ShutdownProtocol& sd)
{
   m_shutdown = true;

}

void Scheduler::executeProgramPostponed(const ExecuteProgramPostponedProtocol& epp)
{
   if(m_shutdown)
   {
      return;
   }

   // Write to node zero
   if(m_messageQueue.write(epp.serialize(), MessageQueue::SchedulerId + 1))
   {
      m_nodeMap[epp.getDestinationNode()] = Node::Busy;
   }
}

void Scheduler::printStatistics()
{
   std::string statistics;
   statistics.append("\n\n-----------------------------Statistics-------------------------\n");
   statistics.append("PID\tProgram Name\tSubmittal Time\tBegin Time\tEnd Time\n");
   statistics.append("----------------------------------------------------------------\n");
   for(const auto& executionLog : m_executionLogList)
   {
      statistics.append(std::to_string(executionLog.getPID()));
      statistics.append("\t");

      statistics.append(executionLog.getProgramName());
      statistics.append("\t");

      const auto submittalTime = executionLog.getSubmittalTime();
      const auto *submittalTimeInfo = localtime(&submittalTime);
      statistics.append((submittalTimeInfo->tm_hour < 10 ? "0" : "") + std::to_string(submittalTimeInfo->tm_hour) + ":");
      statistics.append((submittalTimeInfo->tm_min < 10 ? "0" : "") + std::to_string(submittalTimeInfo->tm_min) + ":");
      statistics.append((submittalTimeInfo->tm_sec < 10 ? "0" : "") + std::to_string(submittalTimeInfo->tm_sec) + "\t");

      const auto beginTime = executionLog.getBeginTime();
      const auto *beginTimeInfo = localtime(&beginTime);
      statistics.append((beginTimeInfo->tm_hour < 10 ? "0" : "") + std::to_string(beginTimeInfo->tm_hour) + ":");
      statistics.append((beginTimeInfo->tm_min < 10 ? "0" : "") + std::to_string(beginTimeInfo->tm_min) + ":");
      statistics.append((beginTimeInfo->tm_sec < 10 ? "0" : "") + std::to_string(beginTimeInfo->tm_sec) + "\t");

      const auto endTime = executionLog.getEndTime();
      const auto *endTimeInfo = localtime(&endTime);
      statistics.append((endTimeInfo->tm_hour < 10 ? "0" : "") + std::to_string(endTimeInfo->tm_hour) + ":");
      statistics.append((endTimeInfo->tm_min < 10 ? "0" : "") + std::to_string(endTimeInfo->tm_min) + ":");
      statistics.append((endTimeInfo->tm_sec < 10 ? "0" : "") + std::to_string(endTimeInfo->tm_sec) + "\n");
   }

   std::cout << statistics << "\n";
}
