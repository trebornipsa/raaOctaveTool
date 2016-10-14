#pragma once

#include <map>
#include <list>
#include <QtCore/QString>

namespace raaNet
{
	class raaTcpThread;
	class raaUdpThread;

	typedef std::map<QString, raaTcpThread*>raaStrngTcpThreadMap;
	typedef std::list<raaTcpThread*>raaTcpThreadList;
	typedef std::map<QString, raaUdpThread*>raaStrngUdpThreadMap;
	typedef std::list<raaUdpThread*>raaUdpThreadList;

	const static unsigned int csm_uiUnconnectedState = 0;
	const static unsigned int csm_uiHostLookupState = 1;
	const static unsigned int csm_uiConnectingState = 2;
	const static unsigned int csm_uiConnectedState = 3;
	const static unsigned int csm_uiBoundState = 4;
	const static unsigned int csm_uiClosingState = 5;
	const static unsigned int csm_uiListeningState = 6;
	const static unsigned int csm_uiNameConnectedState = 7;

	const static unsigned short csm_usTcpMsgSystem = 0;
	const static unsigned short csm_usTcpMsgInfo = 1;
	const static unsigned short csm_usTcpMsgRequest = 2;
	const static unsigned short csm_usTcpMsgReply = 3;

	const static unsigned short csm_usUdpMsgSystem = 0;
	const static unsigned short csm_usUdpMsgInfo = 1;

	const static char csm_pcSepperator[] = "<raa>";
	const static char csm_pcInnerSepperator[] = "<rii>";

	static QString tcpMsgTypeToString(unsigned short usType)
	{
		switch(usType)
		{
			case csm_usTcpMsgSystem:
				return QString("tcpMsgSystem");
			case csm_usTcpMsgInfo:
				return QString("tcpMsgInfo");
			case csm_usTcpMsgRequest:
				return QString("tcpMsgRequest");
			case csm_usTcpMsgReply:
				return QString("tcpMsgReply");
			default:
				return QString("tcpMsgUnknown");
		}
	}
}
