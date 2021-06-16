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

    void handleModuleChange();

    void handleOperData(std::string& name, std::string& xPath);

    void handleRpc();

    void handleNotification();

    bool makeCall(std::string number);

    void setName(std::string name);
    
    private:
    NetConfAgent agent;
    std::string _name;
    std::string _number;

};

}
