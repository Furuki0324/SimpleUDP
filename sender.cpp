#include "simple_udp.h"
#include <stdio.h>

simple_udp udp0("127.0.0.1", 50001);

int main(int argc, char** argv)
{
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	while (true)
	{
		printf("Send\n");
		udp0.udp_send("Hello receiver!!");
	}
	udp0.udp_send("Good bye, receiver!");
	return 0;
}