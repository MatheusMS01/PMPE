#pragma once

#include "MessageQueue.h"
#include "ExecuteProgramPostponedProtocol.h"
#include "NotifySchedulerProtocol.h"
#include "TimestampProtocol.h"
#include <vector>

class Node
{
public:
   enum State : int
   {
      Free = 0,
      Busy = 1,
   };

   Node(int id);
   ~Node();

   void execute();
private:
   struct Neighbor
   {
      Neighbor(unsigned int x, unsigned int y)
         : x(x)
         , y(y)
      {
         id = x + (y * 4);
         processType = id + MessageQueue::SchedulerId + 1;
      }
      int id;
      unsigned int processType;
      unsigned int x;
      unsigned int y;
   };
   int m_id;
   unsigned int m_processType;
   MessageQueue m_messageQueue;
   unsigned int m_x;
   unsigned int m_y;
   std::vector<Neighbor> m_neighborList;
   bool m_waitingTimestamp;
   NotifySchedulerProtocol m_ns;

   void buildNeighborhood();
   void treat(ExecuteProgramPostponedProtocol epp);
   void treat(NotifySchedulerProtocol ns);
   void treat(TimestampProtocol ts);
   void route(const std::string& pdu, int destinationNode);
};