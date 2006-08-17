#ifndef __XMMSSOCKET_H__
#define __XMMSSOCKET_H__

#include <xmmsclient/xmmsclient++.h>

#include "XmmsQT4.h"

#include <QObject>
#include <QTimer>

class XMMSSocket 
{
	public:
		XMMSSocket (const char *name = "promoe");
		~XMMSSocket () { qDebug("destroy"); };
		bool connect (const char *path);
		Xmms::Client m_client;
};

#endif
