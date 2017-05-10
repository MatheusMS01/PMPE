#include "AlarmProtocol.h"

AlarmProtocol::AlarmProtocol()
   : IProtocol(IProtocol::Alarm, ParametersNumber)
{
}

std::string AlarmProtocol::fancy() const
{
   return "Alarm";
}