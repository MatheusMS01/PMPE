#include "NotifySchedulerProtocol.h"

NotifySchedulerProtocol::NotifySchedulerProtocol()
   : IProtocol(IProtocol::NotifyScheduler, ParametersNumber)
{
}

void NotifySchedulerProtocol::setNodeId(int nodeId)
{
   m_parameterList.at(NodeId) = std::to_string(nodeId);
}

int NotifySchedulerProtocol::getNodeId() const
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

void NotifySchedulerProtocol::setSubmittalTime(const long int submittalTime)
{
   m_parameterList.at(SubmittalTime) = std::to_string(submittalTime);
}

long int NotifySchedulerProtocol::getSubmittalTime() const
{
   try
   {
      return std::stol(m_parameterList.at(SubmittalTime));
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

long int NotifySchedulerProtocol::getBeginTime() const
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

long int NotifySchedulerProtocol::getEndTime() const
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

int NotifySchedulerProtocol::getDelay() const
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

std::string NotifySchedulerProtocol::getProgramName() const
{
   return m_parameterList.at(ProgramName);
}

void NotifySchedulerProtocol::setPID(const pid_t pid)
{
    m_parameterList.at(PID) = std::to_string(pid);
}

pid_t NotifySchedulerProtocol::getPID() const
{
   try
   {
      return std::stoi(m_parameterList.at(PID));
   }
   catch(...)
   {
      return -1;
   }
}

std::string NotifySchedulerProtocol::fancy() const
{
   std::string message;

   message.append("NotifyScheduler\n");
   message.append("\tNodeId: " + m_parameterList.at(NodeId) + "\n");
   message.append("\tSubmittalTime: " + m_parameterList.at(SubmittalTime) + "\n");
   message.append("\tBeginTime: " + m_parameterList.at(BeginTime) + "\n");
   message.append("\tEndTime: " + m_parameterList.at(EndTime) + "\n");
   message.append("\tDelay: " + m_parameterList.at(Delay) + "\n");
   message.append("\tProgramName: " + m_parameterList.at(ProgramName) + "\n");
   message.append("\tPID: " + m_parameterList.at(PID));

   return message;
}