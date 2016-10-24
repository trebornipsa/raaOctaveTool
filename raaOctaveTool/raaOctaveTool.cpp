// raaOctaveTool.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <windows.h>

#include <QtWidgets\QApplication>
#include "raaOctaveToolInterface.h"

int main(int argc, char* argv[])
{
	std::string sConfig;
	std::string sIp = "localhost";
	std::string sName = "raaOctaveTool";
	unsigned short int usiPort = 65204;

	for (int i = 0; i<argc; i++)
	{
		if (!strcmp(argv[i], "-config")) sConfig = argv[++i];
		if (!strcmp(argv[i], "-server")) sIp = argv[++i];
		if (!strcmp(argv[i], "-name")) sName = argv[++i];
		if (!strcmp(argv[i], "-port")) usiPort = QString(argv[++i]).toUInt();
	}

	QApplication a(argc, argv);

//	Sleep(20000);

	raaOctaveToolInterface *pInterface = new raaOctaveToolInterface(sConfig, sName, sIp, usiPort);

	pInterface->show();

	return a.exec();
}

