#include "ShutdownProtocol.h"

ShutdownProtocol::ShutdownProtocol()
   : IProtocol(IProtocol::Shutdown, ParametersNumber)
{
}

std::string ShutdownProtocol::fancy() const
{
   return "Shutdown";
}