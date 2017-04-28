#pragma once

#include "MessageQueue.h"

#include "ExecuteProgramPostponedProtocol.h"
#include "NotifySchedulerProtocol.h"
#include "ShutdownProtocol.h"

#include <map>
#include <list>

class Scheduler
{
public:
   Scheduler();
   ~Scheduler();

   int execute();

private:
   MessageQueue m_messageQueue;
   std::map<int, int> m_nodeMap;
   std::list<ExecuteProgramPostponedProtocol> m_pendingExecutionList;
   bool m_shutdown;

   bool createNodes();

   void treat(ExecuteProgramPostponedProtocol& eep);
   void treat(NotifySchedulerProtocol& ns);
   void treat(ShutdownProtocol& sd);

   void executeProgramPostponed(ExecuteProgramPostponedProtocol& eep);
   void printStatistics();
};