#include "NotifySchedulerProtocol.h"

NotifySchedulerProtocol::NotifySchedulerProtocol()
   : IProtocol(IProtocol::NotifyScheduler, ParametersNumber)
{
}

void NotifySchedulerProtocol::setNodeId(int nodeId)
{
   m_parameterList.at(NodeId) = std::to_string(nodeId);
}

int NotifySchedulerProtocol::getNodeId()
{
   try
   {
      return std::stoi(m_parameterList.at(NodeId));
   }
   catch(...)
   {
      return -1;
   }
}

void NotifySchedulerProtocol::setBeginTime(const long int beginTime)
{
   m_parameterList.at(BeginTime) = std::to_string(beginTime);
}

long int NotifySchedulerProtocol::getBeginTime()
{
   try
   {
      return std::stol(m_parameterList.at(BeginTime));
   }
   catch(...)
   {
      return -1;
   }
}

void NotifySchedulerProtocol::setEndTime(const long int endTime)
{
   m_parameterList.at(EndTime) = std::to_string(endTime);
}

long int NotifySchedulerProtocol::getEndTime()
{
   try
   {
      return std::stol(m_parameterList.at(EndTime));
   }
   catch(...)
   {
      return -1;
   }
}

void NotifySchedulerProtocol::setDelay(int delay)
{
   m_parameterList.at(Delay) = std::to_string(delay);
}

int NotifySchedulerProtocol::getDelay()
{
   try
   {
      return std::stoi(m_parameterList.at(Delay));
   }
   catch(...)
   {
      return -1;
   }
}

void NotifySchedulerProtocol::setProgramName(const std::string& programName)
{
   m_parameterList.at(ProgramName) = programName;
}

std::string& NotifySchedulerProtocol::getProgramName()
{
   return m_parameterList.at(ProgramName);
}
