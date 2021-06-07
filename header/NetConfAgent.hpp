#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "libsysrepocpp/headers/Session.hpp"
//#include <libsysrepocpp/headers/Session.hpp>
using namespace sysrepo;


class NetConfAgent
{
    private:
    S_Connection Connection;
    S_Session Session;
    S_Subscribe Subscribe;
    public:
    bool initSysrepo();
    bool closeSysrepo();
    bool fetchData();
    bool subscribeForModelChanges();
    bool registerOperData();
    bool subscribeForRpc();
    bool notifySysrepo();
    bool changeData();
    
};

