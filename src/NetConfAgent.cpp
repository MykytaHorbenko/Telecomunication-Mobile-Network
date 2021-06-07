#include "NetConfAgent.hpp"

using namespace std;

bool NetConfAgent::initSysrepo() 
{
    Connection = std::make_shared<sysrepo::Connection>();
    Session = std::make_shared<sysrepo::Session>(Connection);
    Subscribe = std::make_shared<sysrepo::Subscribe>(Session);
    std::cout<<"Session began"<<std::endl;
    return 1;
}

bool NetConfAgent::closeSysrepo(){
    Subscribe.reset();
    Connection.reset();
    Session.reset();
    std::cout<<"Session closed"<<std::endl;
    return 1;
}

bool NetConfAgent::fetchData(){
    try {
        cout<<"fetchData started"<<endl;
        const char *xpath = "/mobile-network:core/subscribers[number='001']/incomingNumber";
        auto values = Session->get_items(xpath);
        if (values == nullptr)
        cout<<"values == nullptr"<<endl;
            return 0;
    cout<<"Values on xpath: "<<endl;  
        for(unsigned int i = 0; i < values->val_cnt(); i++)
            cout << values->val(i)->xpath() << endl;
    } catch( const std::exception& e ) {
        cout << e.what() << endl;
    }
    return 1;
}
