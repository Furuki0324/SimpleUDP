#include <stdio.h>
#include "simple_udp.h"

simple_udp udp0("0.0.0.0", 50001);

int main(int argc, char** argv)
{
	udp0.udp_bind();
	while (true)
	{
		std::string receivedData = udp0.udp_recv();
		printf("Received message is %s\n", receivedData.c_str());
	}
	return 0;
}