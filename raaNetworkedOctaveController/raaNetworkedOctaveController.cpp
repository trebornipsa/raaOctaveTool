// raaNetworkedOctaveController.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <QtCore/QCoreApplication>


#include "raaOctaveControl.h"

int main(int argc, char** argv)
{
	std::string sTracker;
	for(int i=0;i<argc;i++)
	{
		if (!strcmp(argv[i], "-tracker")) sTracker = argv[++i];
	}


	QCoreApplication a(argc, argv);

	raaOctaveControl *pController = new raaOctaveControl(sTracker);



//	raaDataEngineController *pController = new raaDataEngineController();

	a.exec();


	return 0;
}


