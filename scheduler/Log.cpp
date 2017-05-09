#include "Log.h"

Log::Log(const std::string& name)
   : m_log("log/" + name + ".txt")
{
}

Log::~Log()
{
   m_log.close();
}

void Log::write(const std::string& message)
{
   const auto formattedMessage = message + "\n"; 
   m_log  << formattedMessage;
   m_log.flush();
}