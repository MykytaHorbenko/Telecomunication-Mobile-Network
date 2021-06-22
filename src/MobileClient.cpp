#pragma once

#include "MobileClient.hpp"

using namespace std;
using namespace Netconfagent;

namespace mobileclient
{
    //Helper variables
    vector <string> listNumbers;
    string outcomingNumber;
    string myOldState;
    string incNumOld;
    bool callState;
    bool iMadeCall = 0;

    //Helper functions
    string MobileClient::userName(string arg)
    {
        return "/mobile-network:core/subscribers[number='" + arg + "']/userName";
    }
    string MobileClient::number(string arg)
    {
        return "/mobile-network:core/subscribers[number='" + arg + "']/number";
    }
    string MobileClient::incNumber(string arg)
    {
        return "/mobile-network:core/subscribers[number='" + arg + "']/incomingNumber";
    }
    string MobileClient::state(string arg)
    {
        return "/mobile-network:core/subscribers[number='" + arg + "']/state";
    }


    bool MobileClient::registerClient(std::string number, std::string state)
    {
        _number = number;

        agent = make_unique<Netconfagent::NetConfAgent>();
        
        string xpathMyState = MobileClient::state(_number);
        string xpathMyNum = MobileClient::number(_number);
        string xpathForSubscribe = "/mobile-network:core/subscribers[number='" + _number + "']";
        string xpathForFetch = "/mobile-network:core/subscribers";

        agent->initSysrepo();

        if(_name == "")
        {
            cout << endl << "Name isn`t setted" << endl;
            return false;
        }
        if(_name != "")
        {
            agent->changeData(xpathMyState, state);
            agent->changeData(xpathMyNum, number);
            agent->registerOperData(*this, xpathForSubscribe);
        }

        agent->subscribeForModelChanges(*this, xpathForFetch);
        cout << endl << "Client " << _name << " registered with this number " << _number << endl;
    };

    void MobileClient::handleModuleChange(map < string, string >& mapFetchData) 
    {
        string xpathMyState = MobileClient::state(_number);
        string xpathMyIncNum = MobileClient::incNumber(_number);
        _incomingNumber = mapFetchData[xpathMyIncNum];
        string xpathOtherState = MobileClient::state(_incomingNumber);

        //string _xpathOtherStateOldNum = MobileClient::state(incNumOld);

        string xpathOtherStateOldNum;
         if(iMadeCall == 0)
         xpathOtherStateOldNum = MobileClient::state(incNumOld);
         else if(iMadeCall == 1)
            xpathOtherStateOldNum = MobileClient::state(outcomingNumber);



        if(mapFetchData[xpathMyIncNum] != "" && mapFetchData[xpathMyState] == "active")
        {
            
            cout<<endl;
            cout<<"======Incoming number " << _incomingNumber << " call!!!======"<<endl;
            cout<<endl;

        }

        if(mapFetchData[xpathMyState] == "busy" && mapFetchData[xpathOtherState] == "busy")
        {
            cout<<endl;
            cout<<"==============Call is active=============="<<endl;
            cout<<endl;
            callState = true;
        }

        if(mapFetchData[xpathMyState] == "idle" && mapFetchData[xpathOtherStateOldNum] == "idle" && mapFetchData[xpathMyIncNum] == "" && callState == false)//need to thinking
        {
            cout<<endl;
            cout<<"==============Call rejected=============="<<endl;
            cout<<endl;
            callState = false;
            incNumOld = "";
        }
        if(mapFetchData[xpathMyState] == "idle" && mapFetchData[xpathOtherStateOldNum] == "idle" && mapFetchData[xpathMyIncNum] == "" && callState == true)
        {
            cout<<endl;
            cout<<"==============Call ended=============="<<endl;
            cout<<endl;
            callState = false;
        }
        if(mapFetchData[xpathMyState] == "busy" && _name == "" && mapFetchData[xpathMyIncNum] == "")
        {
            cout<<endl;
            cout<<"==========Client unregistered========="<<endl;
            cout<<endl;

        }

    };

    void MobileClient::handleOperData(std::string& name, std::string& xPath) const
    {
        name = _name;
    
        xPath = "/mobile-network:core/subscribers[number='" + _number + "']/userName";
    }

    bool MobileClient::makeCall(std::string number)
    {
        string xpathOtherIncNum = MobileClient::incNumber(number);
        string xpathOtherState = MobileClient::state(number);
        string xpathOtherNumber = MobileClient::number(number);
        string xpathOtherClient = "/mobile-network:core/subscribers[number='" + number + "']";

        string xpathMyState = MobileClient::state(_number);

        map < string, string > mapCheckNumber;
        agent->fetchData(xpathOtherClient, mapCheckNumber);

        if(mapCheckNumber[xpathOtherState] != "idle")
        {
            cout<<"Client busy!!!"<<endl;
            return false;
        }

        if(mapCheckNumber[xpathOtherNumber] == _number)
        {
            cout<<"This is your number!!!"<<endl;
            return false;
        }

        else if(mapCheckNumber[xpathOtherNumber] == number)
        {
            agent->changeData(xpathOtherIncNum, _number);
            agent->changeData(xpathMyState, "active");
            agent->changeData(xpathOtherState, "active");
        }

        outcomingNumber = number;
        iMadeCall = 1;
    }

    void MobileClient::setName(std::string name)
    {
        _name = name;

        cout << endl << "Name " << _name << " setted " << endl;
    }

    bool MobileClient::answerCall()
    {
        string xpathMyState = MobileClient::state(_number);
        
        string xpathOtherState = MobileClient::state(_incomingNumber);

        if(_incomingNumber != "")
        {
        agent->changeData(xpathMyState, "busy");
        agent->changeData(xpathOtherState, "busy");
        }
        else
        {
            cout<<"You don`t have incoming call"<<endl;
            return false;
        }
    }

    bool MobileClient::rejectCall()
    {
        incNumOld = _incomingNumber;
        string xpathMyState = MobileClient::state(_number);
        string xpathMyIncNum = MobileClient::incNumber(_number);
        string xpathOtherIncNum = MobileClient::incNumber(outcomingNumber);

        string xpathCheckState = "/mobile-network:core/subscribers";

        string xpathOtherState;
        if(iMadeCall == 0)
         xpathOtherState = MobileClient::state(incNumOld);
        else if(iMadeCall == 1)
         xpathOtherState = MobileClient::state(outcomingNumber);

        map < string, string > mapCheckState;
        agent->fetchData(xpathCheckState, mapCheckState);

        if(mapCheckState[xpathMyState] == "idle")
        {
            cout<<"Call doesn't exist"<<endl;
            return false;
        }

        if(mapCheckState[xpathMyState] == "busy")
        {
            cout<<"You already answer the call!!!"<<endl;
            return false;
        }

        if(mapCheckState[xpathMyState] == "active" && iMadeCall == 0)
        {
        agent->changeData(xpathMyIncNum, "");
        agent->changeData(xpathMyState, "idle");
        agent->changeData(xpathOtherState, "idle");
        }

        if(mapCheckState[xpathMyState] == "active" && iMadeCall == 1)
        {
        agent->changeData(xpathOtherIncNum, "");
        agent->changeData(xpathMyState, "idle");
        agent->changeData(xpathOtherState, "idle");
        }
        iMadeCall = 0;
    }

    bool MobileClient::endCall()
    {
        incNumOld = _incomingNumber;
        string xpathMyState = MobileClient::state(_number);
        string xpathMyIncNum = MobileClient::incNumber(_number);
        string xpathOtherIncNum = MobileClient::incNumber(outcomingNumber);

        string xpathCheckState = "/mobile-network:core/subscribers";

        string xpathOtherState;
        if(iMadeCall == 0)
         xpathOtherState = MobileClient::state(incNumOld);
        else if(iMadeCall == 1)
         xpathOtherState = MobileClient::state(outcomingNumber);

        map < string, string > mapCheckState;
        agent->fetchData(xpathCheckState, mapCheckState);

        if(mapCheckState[xpathMyState] == "idle")
        {
            cout<<"Call doesn't exist"<<endl;
            return false;
        }

        if(mapCheckState[xpathMyState] == "active")
        {
            cout<<"You didn't answer the call!!!"<<endl;
            return false;
        }

        if(mapCheckState[xpathMyState] == "busy" && iMadeCall == 0)
        {
        agent->changeData(xpathMyState, "idle");
        agent->changeData(xpathOtherState, "idle");
        agent->changeData(xpathMyIncNum, "");
        }

        if(mapCheckState[xpathMyState] == "busy" && iMadeCall == 1)
        {
        agent->changeData(xpathMyState, "idle");
        agent->changeData(xpathOtherState, "idle");
        agent->changeData(xpathOtherIncNum, "");
        }
        iMadeCall = 0;
    }

    bool MobileClient::unRegisterClient()
    {
        map < string, string > mapCheck;
        string xpathToMe = "/mobile-network:core/subscribers";
        agent->fetchData(xpathToMe, mapCheck);

        string xpathMyState = MobileClient::state(_number);
        string xpathMyIncNum = MobileClient::incNumber(_number);
        string xpathMyNum = MobileClient::number(_number);
        string xpathMyName = MobileClient::userName(_number);

        string xpathForSubscribe = "/mobile-network:core/subscribers[number='" + _number + "']";

        if(mapCheck[xpathMyState] == "idle" && mapCheck[xpathMyIncNum] == "")
        {
            _name = "";
            agent->changeData(xpathMyState, "busy");
            _number = "";
            agent->changeData(xpathMyNum, _number);
            agent.~unique_ptr();
        }
        else
        {
            cout<<"Client busy!!!"<<endl;
            return false;
        }
    }
}
