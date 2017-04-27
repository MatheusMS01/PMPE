#include "ExecuteProgramPostponedProtocol.h"

ExecuteProgramPostponedProtocol::ExecuteProgramPostponedProtocol()
   : IProtocol(IProtocol::ExecuteProgramPostponed, ParametersNumber)
{
}

void ExecuteProgramPostponedProtocol::setSeconds(int seconds)
{
   m_contentList.at(Seconds) = std::to_string(seconds);
}

int ExecuteProgramPostponedProtocol::ExecuteProgramPostponedProtocol::getSeconds()
{
   try
   {
      return std::stoi(m_contentList.at(Seconds));
   }
   catch(...)
   {
      return -1;
   }
}

void ExecuteProgramPostponedProtocol::setProgramName(const std::string& programName)
{
   m_contentList.at(ProgramName) = programName;
}

std::string& ExecuteProgramPostponedProtocol::getProgramName()
{
   return m_contentList.at(ProgramName);
}

void ExecuteProgramPostponedProtocol::setDestinationNode(int destinationNode)
{
   m_contentList.at(DestinationNode) = std::to_string(destinationNode);
}

int ExecuteProgramPostponedProtocol::getDestinationNode()
{
   try
   {
      return std::stoi(m_contentList.at(DestinationNode));
   }
   catch(...)
   {
      return -1;
   }
}