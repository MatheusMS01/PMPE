#pragma once

#include <string>
#include <vector>

class Utils
{
public:
   static int getProtocolId(const std::string& pdu);

private:
   static std::vector<std::string> getPduContent(const std::string& pdu);
};