#include "chart1/chat_server.h"

int main(int argc, char* argv[])
{
	printf("main started\n");

	chat_server(argv[1]);

	printf("main end\n");

	return 0;

}