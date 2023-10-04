#include "pch.h"
#include "Server.h"

int main()
{
	Server server;
	server.Initialize();
	
	server.Run();

	server.CleanUp();
}