#pragma once

#include "MessageQueue.h"
#include "ExecuteProgramPostponedProtocol.h"
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
         : m_x(x)
         , m_y(y)
      {
         m_id = m_x + (m_y * 4);
         m_processType = m_id + MessageQueue::SchedulerId + 1;
      }
      int m_id;
      unsigned int m_processType;
      unsigned int m_x;
      unsigned int m_y;
   };
   int m_id;
   unsigned int m_processType;
   MessageQueue m_messageQueue;
   unsigned int m_x;
   unsigned int m_y;
   std::vector<Neighbor> m_neighborList;

   void buildNeighborhood();
   void treat(ExecuteProgramPostponedProtocol epp);
   void route(const std::string& pdu, int destinationNode);
};