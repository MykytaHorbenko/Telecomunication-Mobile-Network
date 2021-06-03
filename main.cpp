#include <iostream>
#include <string>
using namespace std;

int main()
{
	string comand, arg;

	do
	{
		cout << "Please, enter the comand" << endl;

		cin >> comand;
		if (comand != "answer" && 
			comand != "reject" && 
			comand != "callEnd" && 
			comand != "exit")
			cin >> arg;

		if (comand == "register")
		{
			//registerUser(arg);
			cout << "Func 'registerUser' was called" << endl;
		}
		else if (comand == "unregister")
		{
			//unregisterUser(arg);
			cout << "Func 'unregisterUser' was called" << endl;
		}
		else if (comand == "call")
		{
			//makeCall(arg);
			cout << "Func 'makeCall' was called" << endl;
		}
		else if (comand == "name")
		{
			//changeUserName(arg);
			cout << "Func 'changeUserName' was called" << endl;
		}
		else if (comand == "answer")
		{
			//answerCall();
			cout << "Func 'answerCall' was called" << endl;
		}
		else if (comand == "reject")
		{
			//rejectCall();
			cout << "Func 'rejectCall' was called" << endl;
		}
		else if (comand == "callEnd")
		{
			//callEnd();
			cout << "Func 'callEnd' was called" << endl;
		}
		else
		{
			if (comand != "exit")
				cout << "Error comand" << endl;
		}
	} while (comand != "exit");
}


