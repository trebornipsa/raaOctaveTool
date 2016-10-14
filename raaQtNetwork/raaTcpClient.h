#pragma once

#include <QtCore/QObject>
#include "raaNetworkDefs.h"

class RAANETWORK_DLL_DEF raaTcpClient: public QObject
{
    Q_OBJECT
public:
	raaTcpClient();
	virtual ~raaTcpClient();
};

