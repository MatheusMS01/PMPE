#include "ShutdownProtocol.h"

ShutdownProtocol::ShutdownProtocol()
   : IProtocol(IProtocol::Shutdown, ParametersNumber)
{
}

std::string ShutdownProtocol::pretty() const
{
   return "Shutdown";
}