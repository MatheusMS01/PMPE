#pragma once

#include "IProtocol.h"

class ShutdownProtocol : public IProtocol
{
public:
   enum ParameterId : unsigned int
   {
      ParametersNumber = 0, 
   };

   ShutdownProtocol();
};