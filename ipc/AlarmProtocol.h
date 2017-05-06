#pragma once

#include "IProtocol.h"

class AlarmProtocol : public IProtocol
{
public:
   enum ParameterId : unsigned int
   {
      ParametersNumber = 0, 
   };

   AlarmProtocol();
};