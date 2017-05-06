#pragma once

#include "MessageQueue.h"

#include "ExecuteProgramPostponedProtocol.h"
#include "NotifySchedulerProtocol.h"
#include "ShutdownProtocol.h"
#include "AlarmProtocol.h"

#include <map>
#include <vector>

class Scheduler
{
public:
   Scheduler();
   ~Scheduler() = default;

   int execute();

private:
   MessageQueue m_messageQueue;
   std::map<int, int> m_nodeMap;
   std::vector<ExecuteProgramPostponedProtocol> m_pendingExecutionList;
   std::vector<pid_t> m_childPIDList;
   std::vector<NotifySchedulerProtocol> m_executionLogList;
   bool m_shutdown;
   int m_sequentialNumber;

   bool createNodes();

   void treat(ExecuteProgramPostponedProtocol& eep);
   void treat(const NotifySchedulerProtocol& ns);
   void treat(const ShutdownProtocol& sd);
   void treat(const AlarmProtocol& al);

   void executeProgramPostponed(const ExecuteProgramPostponedProtocol& eep);
   void printStatistics();
};