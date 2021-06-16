#pragma once

#include "MobileClient.hpp"

using namespace std;
using namespace Netconfagent;

namespace mobileclient
{
    bool MobileClient::registerClient(std::string number, std::string state)
    {
        _number = number;
        
        string _xpathForChangeState = "/mobile-network:core/subscribers[number='" + number + "']/state";
        string _xpathForChangeNum = "/mobile-network:core/subscribers[number='" + number + "']/number";

        agent.initSysrepo();
        agent.changeData(_xpathForChangeState, state);
        agent.changeData(_xpathForChangeNum, number);
        cout << endl << "Client with number " << _number << " is created " << endl;
    };

    void MobileClient::handleModuleChange()
    {
        agent.subscribeForModelChanges();
    };

    void MobileClient::handleOperData(std::string& name, std::string& xPath)
    {
        name = _name;
    
        xPath = "/mobile-network:core/subscribers[number='" + _number + "']/userName";
    }

    void handleRpc();

    void handleNotification();

    bool MobileClient::makeCall(std::string number)
    {
        string _xpathForChangeIncNum = "/mobile-network:core/subscribers[number='" + number + "']/incomingNumber";

        agent.changeData(_xpathForChangeIncNum, _number);
    }

    void MobileClient::setName(std::string name)
    {
        _name = name;
        agent.registerOperData(*this);

        cout << endl << "Client with number " << _number << " changed name " << endl;
    }

}
