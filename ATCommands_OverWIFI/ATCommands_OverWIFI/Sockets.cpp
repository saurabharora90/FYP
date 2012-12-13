#include<stdio.h>
#include<WinSock2.h>
#include<string>
#include<iostream>

using namespace std;

#define DRONE_IP	"192.168.1.1"
#define UDP_PORT	5556

SOCKET s;

bool connectToDrone()
{
	//Start up Winsock…
    WSADATA wsadata;

    int error = WSAStartup(0x0202, &wsadata);

    //Did something happen?
    if (error)
        return false;

    //Did we get the right Winsock version?
    if (wsadata.wVersion != 0x0202)
    {
        WSACleanup(); //Clean up Winsock
        return false;
    }

    //Fill out the information needed to initialize a socket…
    SOCKADDR_IN target; //Socket address information

    target.sin_family = AF_INET; // address family Internet
    target.sin_port = htons (UDP_PORT); //Port to connect on
    target.sin_addr.s_addr = inet_addr (DRONE_IP); //Target IP

    s = socket (AF_INET, SOCK_DGRAM , 0); //Create socket
    if (s == INVALID_SOCKET)
    {
        return false; //Couldn't create the socket
    }  

    //Try connecting...

    if (connect(s, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
    {
        return false; //Couldn't connect
    }
    else
        return true; //Success
}

void CloseConnection ()
{
    //Close the socket if it exists
    if (s)
        closesocket(s);

    WSACleanup(); //Clean up Winsock
}

int main()
{
	printf("Trying to Connect to Parrot AR Drone\n");
	if(connectToDrone())
		printf("Connection to Parrot AR Drone Successful\n");
	else
	{
		printf("An error has occured\n");
		return 0;
	}
	int command;
	
	printf("Please enter a command to be sent to the drone else press 0 to exit\n");
	printf("Press 1 for Green Led Blink\n");
	printf("Press 2 for Red Led Blink\n");
	printf("Press 5 for Fire\n");
	cin>>command;
	
	while(command!=0)
	{
		switch(command)
		{
			case 1:
				send(s,"AT*LED=1,1,1056964608,0\r",25,0);
				break;
			case 2:
				send(s,"AT*LED=1,2,1056964608,0\r",25,0);
				break;
			case 5:
				send(s,"AT*LED=1,5,1056964608,0\r",25,0);
				break;
		}
		
		printf("Command sent to Parrot AR Drone\n");

		printf("Enter the next command or press 0 to exit\n");
		cin>>command;
	}

	CloseConnection();
}