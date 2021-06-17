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
        string _xpathForSubscribe = "/mobile-network:core/subscribers[number='" + number + "']";
        string _xpathForFetch = "/mobile-network:core/subscribers[number='" + number + "']";


        agent.initSysrepo();
        agent.changeData(_xpathForChangeState, state);
        agent.changeData(_xpathForChangeNum, number);
        if(_name != "")
            agent.registerOperData(*this, _xpathForSubscribe);
        else
        {
            cout << endl << "Name isn`t setted" << endl;
            return false;
        }
        agent.subscribeForModelChanges(*this, _xpathForFetch);
        cout << endl << "Client with number " << _number << " is created " << endl;
    };

    void MobileClient::handleModuleChange(map < string, string >& mapFetchData) const
    {

        //deleting empty pairs
		map < string, string >::iterator it;
		it = mapFetchData.begin();
		mapFetchData.erase(it);
		it = mapFetchData.begin();
		mapFetchData.erase(it);
		//printing map
    	for (auto i : mapFetchData){
    	cout << i.first << " : " << i.second << endl;
    	}
    };

    void MobileClient::handleOperData(std::string& name, std::string& xPath) const
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
        //agent.registerOperData(*this);

        cout << endl << "Client with number " << _number << " changed name " << endl;
    }

}
