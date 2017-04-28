#include "ExecuteProgramPostponedProtocol.h"

#include "Utils.h"

ExecuteProgramPostponedProtocol::ExecuteProgramPostponedProtocol()
   : IProtocol(IProtocol::ExecuteProgramPostponed, ParametersNumber)
{
}

bool ExecuteProgramPostponedProtocol::parse(const std::string& pdu)
{
   auto parameterList = Utils::getPduContent(pdu);

   if(parameterList.size() < 3)
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
      case DestinationNode:
      {
         m_parameterList.at(DestinationNode) = parameterList.at(DestinationNode);
      }
      // no break

      // Mandatory
      default:
      {
         m_parameterList.at(Delay) = parameterList.at(Delay);
         m_parameterList.at(ProgramName) = parameterList.at(ProgramName);
      }
      break;
   }

   return true;
}

void ExecuteProgramPostponedProtocol::setDelay(int delay)
{
   m_parameterList.at(Delay) = std::to_string(delay);
}

int ExecuteProgramPostponedProtocol::ExecuteProgramPostponedProtocol::getDelay()
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

std::string& ExecuteProgramPostponedProtocol::getProgramName()
{
   return m_parameterList.at(ProgramName);
}

void ExecuteProgramPostponedProtocol::setDestinationNode(int destinationNode)
{
   m_parameterList.at(DestinationNode) = std::to_string(destinationNode);
}

int ExecuteProgramPostponedProtocol::getDestinationNode()
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