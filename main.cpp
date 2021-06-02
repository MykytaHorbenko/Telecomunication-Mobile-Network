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

		if(comand == "register")
		{
			system("cls");
			//registerUser(arg);
			system("pause");
		}
		if (comand == "unregister")
		{
			system("cls");
			//unregisterUser(arg);
			system("pause");
		}
		if (comand == "call")
		{
			system("cls");
			//makeCall(arg);
			system("pause");
		}
		if (comand == "name")
		{
			system("cls");
			//changeUserName(arg);
			system("pause");
		}
		if (comand == "answer")
		{
			system("cls");
			//answerCall();
			system("pause");
		}
		if (comand == "reject")
		{
			system("cls");
			//rejectCall();
			system("pause");
		}
		if (comand == "callEnd")
		{
			system("cls");
			//callEnd();
			system("pause");
		}
		
	} while (comand != "exit");
}

 
