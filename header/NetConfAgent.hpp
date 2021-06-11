#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "libsysrepocpp/headers/Session.hpp"
//#include <libsysrepocpp/headers/Session.hpp>
using namespace sysrepo;

namespace Netconfagent
{

class NetConfAgent
{
    public:
    bool initSysrepo();
    bool fetchData(string xpath);
    bool subscribeForModelChanges(string _module_name);
    bool registerOperData(string _module_name);
    bool subscribeForRpc(string _module_name, string _rpc_xpath);
    bool notifySysrepo(string _module_name);
    bool changeData(string _xpath, string newValue);
    private:
    S_Connection Connection;
    S_Session Session;
    S_Subscribe Subscribe;
    
};

}
