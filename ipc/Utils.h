#pragma once

#include <string>
#include <vector>

class Utils
{
public:
   static int getProtocolId(const std::string& pdu);
   static unsigned int distanceBetweenNodes(int nodeId1, int nodeId2);
   static std::vector<std::string> getPduContent(const std::string& pdu);
};