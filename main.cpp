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
		if (comand != "answer" && comand != "reject" && comand != "callEnd" && comand != "exit")
			cin >> arg;

		if (comand == "register")
		{
			//registerUser(arg);
		}
		else if (comand == "unregister")
		{
			//unregisterUser(arg);
		}
		else if (comand == "call")
		{
			//makeCall(arg);
		}
		else if (comand == "name")
		{
			//changeUserName(arg);
		}
		else if (comand == "answer")
		{
			//answerCall();
		}
		else if (comand == "reject")
		{
			//rejectCall();
		}
		else if (comand == "callEnd")
		{
			//callEnd();
		}
		else
		{
			if (comand != "exit")
				cout << "Error comand" << endl;
		}
	} while (comand != "exit");
}


