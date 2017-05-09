#pragma once

#include <string>
#include <fstream>

class Log
{
public:
   Log(const std::string& name);
   ~Log();

   void write(const std::string& message);

private:
   std::ofstream m_log;
};