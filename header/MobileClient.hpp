#pragma once

#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "NetConfAgent.hpp"
#include "libsysrepocpp/headers/Session.hpp"

using namespace sysrepo;
using namespace Netconfagent;

namespace mobileclient
{

class MobileClient
{
    public:
/*
*@brief initSysrepo. 
*This method initializes a session with the server
*
*@return true if ok, otherwise false
*/
    bool registerClient(std::string number, std::string state);

    bool unRegisterClient();

    void handleModuleChange(std::map < std::string, std::string >& mapFetchData) ;

    void handleOperData(std::string& name, std::string& xPath) const;

    void handleRpc();

    void handleNotification();

    bool makeCall(std::string number);

    bool answerCall();

    bool rejectCall();

    bool endCall();

    void setName(std::string name);

    std::string userName(std::string arg);

    std::string number(std::string arg);

    std::string incNumber(std::string arg);

    std::string state(std::string arg);

    
    private:
    std::unique_ptr<Netconfagent::NetConfAgent> agent;
    std::string _name;
    std::string _number;
    std::string _incomingNumber;
    


};

}
