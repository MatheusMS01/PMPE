#include "AlarmProtocol.h"

AlarmProtocol::AlarmProtocol()
   : IProtocol(IProtocol::Alarm, ParametersNumber)
{
}

std::string AlarmProtocol::pretty() const
{
   return "Alarm";
}