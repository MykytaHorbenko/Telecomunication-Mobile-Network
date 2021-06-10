#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <functional>
#include "header/NetConfAgent.hpp"
using namespace std;

int main()
{
	string comand;
	string str;
	vector<string> arg;
	stringstream iss;

	vector<string> comandList;
	comandList.push_back("register");
	comandList.push_back("unregister");
	comandList.push_back("call");
	comandList.push_back("name");
	comandList.push_back("answer");
	comandList.push_back("reject");
	comandList.push_back("callEnd");
	comandList.push_back("exit");

	map < string, std::function <void()> > mapFunc;

	mapFunc.emplace("register", [&arg]() {
		if (arg.size() < 2 || arg.size() > 2) {
			cout << "Wrong count of arguments";
			return;
		}
		cout << "func register called with args: ";
		for (auto i : arg)
			cout << i << " ";
		cout << endl;
		});
	mapFunc.emplace("unregister", []() {
		cout << "func unregister called"<<endl;
		});
	mapFunc.emplace("call", [&arg]() {
		if (arg.size() < 1 || arg.size() > 1) {
			cout << "Wrong count of arguments";
			return;
		}
		cout << "func call called with arg: ";
		for (auto i : arg)
			cout << i << " ";
		cout << endl;
		});
	mapFunc.emplace("name", [&arg]() {
		if (arg.size() < 1 || arg.size() > 1) {
			cout << "Wrong count of arguments";
			return;
		}
		cout << "func name called with arg: ";
		for (auto i : arg)
			cout << i << " ";
		cout << endl;
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

//Test methods
		NetConfAgent test;
		test.initSysrepo();
		//test.subscribeForModelChanges();
		//test.changeData();
		//test.registerOperData();
		//test.fetchData();
		test.subscribeForRpc();
		

	// do
	// {
		
	// 	cout << "Please, enter the comand" << endl;
	// 	cin >> comand;

	// 	getline(cin, str);

	// 	stringstream iss(str);
	// 	//Making vector with arguments
	// 	while (iss)
	// 	{
	// 		string word;
	// 		iss >> word;
	// 		arg.push_back(word);
	// 	} 
	// 	vector<string>::iterator it;
	// 	it = arg.end();
	// 	arg.erase(it-1);
	// 	//Checking entered comand
	// 	bool mark = 0;
	// 	for (auto i : comandList){
	// 		if (comand == i) {
	// 			mark = 1;
	// 			break;
	// 		}
	// 	}
	// 	//Calling comand
	// 	if (mark != 0 && comand != "exit") {
	// 		auto f = mapFunc.at(comand);
	// 		f();
	// 	}
	// 	else if(comand != "exit")
	// 		cout << "invalid comand" << endl;
	// 	arg.clear();
	// } while (comand != "exit");
}


