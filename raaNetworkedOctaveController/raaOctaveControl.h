#pragma once

#include <QtCore/QObject>

#include <raaNetwork/raaTcpMsg.h>
#include <raaNetwork/raaNetwork.h>

using namespace raaNet;

class raaOctaveControl: public QObject
{
	Q_OBJECT
public:
	raaOctaveControl();
	virtual ~raaOctaveControl();

	public slots:
	void readServer(raaTcpMsg* pMsg);

protected:
	raaNetwork *m_pServer;

};

