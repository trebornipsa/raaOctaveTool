// raaNetworkedOctaveController.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <QtCore/QCoreApplication>

#include "raaOctaveControl.h"

int main(int argc, char** argv)
{
	QCoreApplication a(argc, argv);

	raaOctaveControl *pController = new raaOctaveControl();

//	raaDataEngineController *pController = new raaDataEngineController();

	a.exec();


	return 0;
}


