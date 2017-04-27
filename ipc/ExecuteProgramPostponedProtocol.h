#pragma once

#include "IProtocol.h"

class ExecuteProgramPostponedProtocol : public IProtocol
{
public:
   enum ContentId : int
   {
      Seconds          = 0,
      ProgramName      = 1,      
      DestinationNode  = 2,
      ParametersNumber = DestinationNode + 1, 
   };

   ExecuteProgramPostponedProtocol();

   void setSeconds(int seconds);
   int getSeconds();

   void setProgramName(const std::string& programName);
   std::string& getProgramName();

   void setDestinationNode(int destinationNode);
   int getDestinationNode();
};