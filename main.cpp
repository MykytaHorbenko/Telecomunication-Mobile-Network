#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <functional>
#include <thread>
//#include "header/NetConfAgent.hpp"
#include "MobileClient.hpp"


using namespace std;
using namespace Netconfagent;
using namespace mobileclient;

int main()
{
	string comand;
	string str;
	vector<string> arg;
	stringstream iss;

	MobileClient client;

	vector<string> comandList;
	comandList.push_back("register");
	comandList.push_back("unregister");
	comandList.push_back("call");
	comandList.push_back("name");
	comandList.push_back("answer");
	comandList.push_back("reject");
	comandList.push_back("callEnd");
	comandList.push_back("exit");

	map < string, string > mapFetchData;
	map < string, std::function <void()> > mapFunc;

	mapFunc.emplace("register", [&arg, &client]() {
		if (arg.size() < 1 || arg.size() > 1) {
			cout << "Wrong count of arguments";
			return;
		}
		const string number = arg[0];

		client.registerClient(number, "idle");
		client.handleModuleChange();

		});
	mapFunc.emplace("unregister", []() {
		cout << "func unregister called"<<endl;
		});
	mapFunc.emplace("call", [&arg, &client]() {
		if (arg.size() < 1 || arg.size() > 1) {
			cout << "Wrong count of arguments";
			return;
		}
		const string number = arg[0];

		client.makeCall(number);
		});
	mapFunc.emplace("name", [&arg, &client]() {
		if (arg.size() < 1 || arg.size() > 1) {
			cout << "Wrong count of arguments";
			return;
		}
		const string name = arg[0];
		client.setName(name);
		});
	mapFunc.emplace("answer", []() {
		cout << "func answer called"<<endl;
		});
	mapFunc.emplace("reject", []() {
		cout << "func reject called" << endl;
		});
	mapFunc.emplace("callEnd", []() {
		cout << "func callEnd called" << endl;
		});

	//thread th1(&NetConfAgent::subscribeForModelChanges, test, "mobile-network");

		//deleting empty pairs
		// map < string, string >::iterator it;
		// it = mapFetchData.begin();
		// mapFetchData.erase(it);
		// it = mapFetchData.begin();
		// mapFetchData.erase(it);
		//printing map
    	// for (auto i : mapFetchData){
    	// cout << i.first << " : " << i.second << endl;
    	// }
		
	do
	{
		
		cout << "Please, enter the comand" << endl;
		cin >> comand;

		getline(cin, str);

		stringstream iss(str);
		//Making vector with arguments
		while (iss)
		{
			string word;
			iss >> word;
			arg.push_back(word);
		} 
		vector<string>::iterator it;
		it = arg.end();
		arg.erase(it-1);
		//Checking entered comand
		bool mark = 0;
		for (auto i : comandList){
			if (comand == i) {
				mark = 1;
				break;
			}
		}
		//Calling comand
		if (mark != 0 && comand != "exit") {
			auto f = mapFunc.at(comand);
			f();
		}
		else if(comand != "exit")
			cout << "invalid comand" << endl;
		arg.clear();
	} while (comand != "exit");

	//th1.detach();
}


