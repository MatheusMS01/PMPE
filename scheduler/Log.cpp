#include "Log.h"
#include <ctime>

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
   const auto now = std::time(nullptr);
   const auto *nowTimeInfo = localtime(&now);

   std::string formattedMessage;
   formattedMessage.append("[");
   formattedMessage.append((nowTimeInfo->tm_hour < 10 ? "0" : "") + std::to_string(nowTimeInfo->tm_hour) + ":");
   formattedMessage.append((nowTimeInfo->tm_min < 10 ? "0" : "") + std::to_string(nowTimeInfo->tm_min) + ":");
   formattedMessage.append((nowTimeInfo->tm_sec < 10 ? "0" : "") + std::to_string(nowTimeInfo->tm_sec));
   formattedMessage.append("] ");
   formattedMessage.append(message + "\n"); 
   m_log  << formattedMessage;
   m_log.flush();
}