///////////////////////////////////////////////////////////
//  Worker.h
//  Implementation of the Class Processer
//  Created on:      02-Apr-2009 18:10:13
//  Original author: Tim Kuo
///////////////////////////////////////////////////////////

#if !defined(EA_20AED985_021D_47c7_AADB_219A004968B6__INCLUDED_)
#define EA_20AED985_021D_47c7_AADB_219A004968B6__INCLUDED_

#include "common.h"
#include "IActionHandler.h"
#include "IEventObserver.h"
#include <QObject>
#include "ThreadDatabase.h"
class QEvent;
class Staff;

class Worker : public QObject, public IActionHandler
{
	Q_OBJECT
public:
	Worker(QObject *parent = 0);
	virtual ~Worker();
	virtual void StartAction(Message& Action) ;
	bool event ( QEvent * e );
 
 private:
	IEventObserver *m_IEventObserver;
	ThreadDatabase* m_databaseThread;
	Staff* loggedStaff;

private slots:
	void IncomingEvent(Message& ev);
};
#endif // !defined(EA_20AED985_021D_47c7_AADB_219A004968B6__INCLUDED_)

