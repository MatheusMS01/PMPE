#include "ExecuteProgramPostponedProtocol.h"

#include "Utils.h"

ExecuteProgramPostponedProtocol::ExecuteProgramPostponedProtocol()
   : IProtocol(IProtocol::ExecuteProgramPostponed, ParametersNumber)
{
}

bool ExecuteProgramPostponedProtocol::parse(const std::string& pdu)
{
   auto parameterList = Utils::getPduContent(pdu);

   if(parameterList.size() < 4)
   {
      return false;
   }

   if(parameterList.at(0) != std::to_string(m_id))
   {
      return false;
   }

   parameterList.erase(parameterList.begin());

   switch(parameterList.size() - 1)
   {
      case SubmissionTime:
      {
         m_parameterList.at(SubmissionTime) = parameterList.at(SubmissionTime);
      }
      // no break
      
      case DestinationNode:
      {
         m_parameterList.at(DestinationNode) = parameterList.at(DestinationNode);
      }
      // no break

      // Mandatory
      default:
      {
         m_parameterList.at(SequentialNumber) = parameterList.at(SequentialNumber);
         m_parameterList.at(Delay) = parameterList.at(Delay);
         m_parameterList.at(ProgramName) = parameterList.at(ProgramName);
      }
      break;
   }

   return true;
}

void ExecuteProgramPostponedProtocol::setDelay(const int delay)
{
   m_parameterList.at(Delay) = std::to_string(delay);
}

int ExecuteProgramPostponedProtocol::ExecuteProgramPostponedProtocol::getDelay() const
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

void ExecuteProgramPostponedProtocol::setProgramName(const std::string& programName)
{
   m_parameterList.at(ProgramName) = programName;
}

std::string ExecuteProgramPostponedProtocol::getProgramName() const
{
   return m_parameterList.at(ProgramName);
}

void ExecuteProgramPostponedProtocol::setDestinationNode(const int destinationNode)
{
   m_parameterList.at(DestinationNode) = std::to_string(destinationNode);
}

int ExecuteProgramPostponedProtocol::getDestinationNode() const
{
   try
   {
      return std::stoi(m_parameterList.at(DestinationNode));
   }
   catch(...)
   {
      return -1;
   }
}

void ExecuteProgramPostponedProtocol::setSubmissionTime(const long int submissionTime)
{
   m_parameterList.at(SubmissionTime) = std::to_string(submissionTime);
}

long int ExecuteProgramPostponedProtocol::getSubmissionTime() const
{
   try
   {
      return std::stol(m_parameterList.at(SubmissionTime));
   }
   catch(...)
   {
      return -1;
   }
}

void ExecuteProgramPostponedProtocol::setSequentialNumber(const int sequentialNumber)
{
   m_parameterList.at(SequentialNumber) = std::to_string(sequentialNumber);
}

int ExecuteProgramPostponedProtocol::getSequentialNumber() const
{
   try
   {
      return std::stol(m_parameterList.at(SequentialNumber));
   }
   catch(...)
   {
      return -1;
   }

}

std::string ExecuteProgramPostponedProtocol::fancy() const
{
   std::string message;

   message.append("ExecuteProgramPostponed\n");
   message.append("\tDelay: " + m_parameterList.at(Delay) + "\n");
   message.append("\tProgramName: " + m_parameterList.at(ProgramName) + "\n");
   message.append("\tSubmissionTime: " + m_parameterList.at(SubmissionTime) + "\n");
   message.append("\tSequentialNumber: " + m_parameterList.at(SequentialNumber) + "\n");
   message.append("\tDestinationNode: " + m_parameterList.at(DestinationNode));

   return message;
}