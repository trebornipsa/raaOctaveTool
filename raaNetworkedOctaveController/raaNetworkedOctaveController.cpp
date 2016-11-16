// raaNetworkedOctaveController.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <QtCore/QCoreApplication>


#include "raaOctaveControl.h"

int main(int argc, char** argv)
{
	std::string sTracker, sConfDir;
	for(int i=0;i<argc;i++)
	{
		if (!strcmp(argv[i], "-tracker")) sTracker = argv[++i];
		if (!strcmp(argv[i], "-confDir")) sConfDir = argv[++i];
	}


	QCoreApplication a(argc, argv);

	raaOctaveControl *pController = new raaOctaveControl(sTracker, sConfDir);



//	raaDataEngineController *pController = new raaDataEngineController();

	a.exec();


	return 0;
}


