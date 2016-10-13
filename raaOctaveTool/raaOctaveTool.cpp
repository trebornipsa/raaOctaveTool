// raaOctaveTool.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <windows.h>

#include <QtWidgets\QApplication>
#include "raaOctaveToolInterface.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	raaOctaveToolInterface *pInterface = new raaOctaveToolInterface();

	pInterface->show();

	return a.exec();
}

