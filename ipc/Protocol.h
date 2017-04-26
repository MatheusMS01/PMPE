#pragma once

#include <string>
#include <vector>

class Protocol
{
public:
   using ParameterList = std::vector<std::string>;

   enum ProtocolId : int
   {
      ExecuteProgramPostponed = 0,
      UpdateAvailability      = 1,
   };

   enum ParameterId : int
   {
      // EEP
      SecondsId             = 0,
      FileNameId            = 1,
      DestinationNodeId     = 2,
      EEPExpectedParameters = DestinationNodeId + 1, 
      // UA
      BeginTimeId           = 0,
      EndTimeId             = 1,
      UAExpectedParameters  = EndTimeId + 1, 
   };

   static void buildPdu(const int protocolId,
                        std::string& pdu,
                        const ParameterList& parameterList);

   static void parsePdu(const std::string& pdu, ParameterList& parameterList);

private:
   static std::vector<std::string> getPduContent(const std::string& pdu);
};