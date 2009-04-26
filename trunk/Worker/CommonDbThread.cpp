#include "CommonDbThread.h"
#include "common.h"
#include "messagedef.h"
#include "Message.h"
 #include <QSqlQuery>
#include <QVariant>
#include "dbquery.h"
#include <fstream>
#include "Staff.h"

CommonDbThread::CommonDbThread(QObject *parent , QThread::Priority priority)
:DbThread(parent, priority)
{
	m_loggedStaff = new Staff();
	m_loggedStaff->clear();
	m_loggedStaffImage = new QByteArray();
	m_loggedStaffImage->clear();
	setDbServer("QSQLITE", DBCONNECTION_COMMON, "", "", "", 0);
}

CommonDbThread::~CommonDbThread()
{

}

void CommonDbThread::WorkerThreadMain(Message& Action) {
	DBINFO("common db thread processing action: ", Action.type());
	switch(Action.type()) {
		case ACTION_SYSTEM_START:
		{
			if(checkDd()) {
				m_tempMsg = new Message(EVENT_SYSTEM_START);	
			}else{
				m_tempMsg = new Message(EVENT_WIZARD);
			}			
			break;
		}
		case ACTION_LOGIN:
		{
			Staff* staffIncome = static_cast<Staff*>(Action.data());
			string passwordInDb = getPassword(staffIncome->ID());
			int* r = new int(ERROR_NO_ERROR);
			if(!passwordInDb.empty() && staffIncome->Password() == passwordInDb) 
			{
				getLoggedStaff(staffIncome->ID());
				*r = ERROR_NO_ERROR;
			} else {
				*r = ERROR_PASSWORD_WRONG;
			}
			m_tempMsg = new Message(EVENT_LOGGEDIN, r);
			delete staffIncome;
			break;
		}
		case ACTION_LOGOFF:
		{
			m_loggedStaff->clear();
			m_loggedStaffImage->clear();
			m_tempMsg = new Message(EVENT_LOGGEDOFF);
			break;
		}
		case ACTION_SETSUPERUSER:
		{
			Staff* staff = static_cast<Staff*>(Action.data());
			addSupperStaff(staff);
			delete staff;
			break;
		}
		case ACTION_GETLOGGEDSTAFF:
		{
			m_tempMsg = new Message(EVENT_LOGGEDSTAFF, m_loggedStaff, m_loggedStaffImage);
			break;
		}
		default:
			break;	
	}
	//be sure the m_tempMsg is NOT NULL!!!
	if(NULL != m_tempMsg) {
		postEvent(m_tempMsg, EventDb);
		m_tempMsg = NULL;
	}
}

bool CommonDbThread::initDb()
{
	db.setDatabaseName(DBNAME);
	if(!db.open()) {
		return false;
	}
	DBINFO("initializing database...", "");
	DBINFO("initDb debug", 1);
	QSqlQuery q = QSqlQuery(db);
	DBINFO("initDb debug", 2);
	q.exec(CREATE_STAFF_TABLE);
		DBINFO("initDb debug", 3);
	q.exec(CREATE_JOB_TABLE);
		DBINFO("initDb debug", 4);
	q.exec(CREATE_LEVET_TABLE);
		DBINFO("initDb debug", 5);
	q.exec(CREATE_ORDERS_TABLE);
		DBINFO("initDb debug", 6);
	q.exec(CREATE_GOODS_TABLE);
		DBINFO("initDb debug", 7);
	q.exec(CREATE_SEX_TABLE);
		DBINFO("initDb debug", 8);
	q.exec(CREATE_STATUS_TABLE);
	q.exec(CREATE_TASKS_TABLE);
	q.exec(CREATE_GOOSTYPE_TABLE);
	q.exec(CREATE_IMAGE_TABLE);
	
	q.prepare(INSERTINTO_SEX_TABLE);
	q.bindValue(":id", 0);
	q.bindValue(":name", "δ�趨");
	q.exec();
	q.bindValue(":id", 1);
	q.bindValue(":name", "��");
	q.exec();
	q.bindValue(":id", 2);
	q.bindValue(":name", "Ů");
	q.exec();

	q.prepare("INSERT INTO job (id, name, profit, descrption) " "VALUES (:id, :name, :profit, :descrption)");
	q.bindValue(":id", 1);
	q.bindValue(":name", ("δ�趨"));
	q.bindValue(":profit", 0);
	q.bindValue(":descrption", ("ϵͳĬ�Ͽ�ְ��"));
	q.exec();

	q.prepare("INSERT INTO level (id, name, profit, descrption) " "VALUES (:id, :name, :profit, :descrption)");
	q.bindValue(":id", 1);
	q.bindValue(":name", ("δ�趨"));
	q.bindValue(":profit", 0);
	q.bindValue(":descrption", ("ϵͳĬ�Ͽռ���"));
	q.exec();

	q.prepare("INSERT INTO status (id, name, descrption) " "VALUES (:id, :name, :descrption)");
	q.bindValue(":id", 1);
	q.bindValue(":name", ("δ�趨"));
	q.bindValue(":descrption", ("ϵͳĬ�Ͽ�״̬"));
	q.exec();

	closeDb();
	DBINFO("databese initialized.", "");
	return true;
}

bool CommonDbThread::checkDd()
{
	bool re = false;
	bool exist;
	int length= 0;
	char* SQLiteMark;
	int isSQLite = -1;
	fstream testfile;
	testfile.open (DBFILE, fstream::in | fstream::out | fstream::app | fstream::binary);
	exist = testfile.is_open();
	if(exist) {
	  testfile.seekg(0, ios::end);
	  length = testfile.tellg();
	  testfile.seekg(0, ios::beg);
	  SQLiteMark = new char[SQLITEMARKLEN];
	  memset(SQLiteMark, 0, SQLITEMARKLEN);
	  testfile.read(SQLiteMark, SQLITEMARKLEN);
	  isSQLite = memcmp(SQLiteMark, SQLITEMARK, SQLITEMARKLEN);
	  testfile.close();
	}
	  
	if(!exist | 0 == length | 0 != isSQLite) {//first time running
		DBINFO("database file not exist or corrupt, creat new one.", "")
		remove(DBFILE);
		m_tempMsg = new Message(EVENT_INIT);
		if(NULL != m_tempMsg) {
			postEvent(m_tempMsg, EventDb);
			m_tempMsg = NULL;
		}
		initDb();
		m_tempMsg = new Message(EVENT_INIT_FINISHED);
		if(NULL != m_tempMsg) {
			postEvent(m_tempMsg, EventDb);
			m_tempMsg = NULL;
		}
		re = false;
	} else {//database exists. check super user
		testfile.close();
		openDb(DBNAME);
		DBINFO("debug 1", "");
		QSqlQuery q = QSqlQuery(getDb(DBCONNECTION_COMMON));
		DBINFO("debug 2", "");
		QString query = QString("SELECT * FROM staff WHERE id = %1").arg(SUPERUSERID);
		DBINFO("debug 3", "");
		q.exec(query);
		DBINFO("debug 4", "");
		if(!q.next()) {
					DBINFO("debug 5", "");
			re = false;
		}
		else {
					DBINFO("debug 6", "");
			re = true;
		}
		closeDb();
				DBINFO("debug 61", "");
	}
					DBINFO("debug 62", "");
	return re;
}

string CommonDbThread::getPassword(uint32 id) 
{
	string password;
	password.clear();
	if(!openDb(DBNAME)){
		return password;
	}
	DBINFO("getting password for: ", id);
	QSqlQuery q = QSqlQuery(getDb(DBCONNECTION_COMMON));
			DBINFO("debug 8", "");
	QString query = QString(GET_PASSWORD_BYID).arg(id);
			DBINFO("debug 9", "");
	if(q.exec(query)){
		if(q.next()) {
			password = string(q.value(0).toByteArray().data());
		}
	}
	closeDb();
	DBINFO("get password completed:", password);
	return password;
}

bool CommonDbThread::addSupperStaff(Staff* staff)
{
	bool r = false;
	if(!openDb(DBNAME)) {
		return r;
	}
	QSqlQuery q = QSqlQuery(getDb(DBCONNECTION_COMMON));
	
	DBINFO("creating super user...", "");
	q.prepare(INSERTINTO_STAFF_SUPER);
	q.bindValue(":id", SUPERUSERID);
	q.bindValue(":password", staff->Password().c_str());
	q.bindValue(":name", "��˼������Ա");
	q.bindValue(":jobId", 1);
	q.bindValue(":levelId", 1);
	q.bindValue(":sex", 1);
	q.bindValue(":status", 1);
	q.bindValue(":cell", "88888888");
	q.bindValue(":phone", "66666666");
	q.bindValue(":address", "�л����񹲺͹�");
	q.bindValue(":descrption", "���ǿ�˼��ϵͳ��������Ա");
	r = q.exec();
	DBINFO("create super user complete!", r);
	return r;
}

bool CommonDbThread::getLoggedStaff(uint32 id) 
{
	bool r = false;
	if(!openDb(DBNAME)){
		return r;
	}

	DBINFO("getting logged staff...", id);
	QSqlQuery q = QSqlQuery(getDb(DBCONNECTION_COMMON));
	QString query = QString(SELECT_STAFF_BYID_NOIMAGE).arg(id);
	if(q.exec(query)){
		if(q.next()) {
			m_loggedStaff->SetID(q.value(0).toUInt());
			m_loggedStaff->SetPassword(q.value(1).toByteArray().data());
			m_loggedStaff->SetName(q.value(2).toByteArray().data());
			m_loggedStaff->SetType(q.value(3).toUInt());
			m_loggedStaff->SetLevel(q.value(4).toUInt());
			m_loggedStaff->SetSex(q.value(5).toUInt());
			m_loggedStaff->SetBaseSalary(q.value(6).toUInt());
			m_loggedStaff->SetStatus(q.value(7).toUInt());
			m_loggedStaff->SetCell(q.value(8).toByteArray().data());
			m_loggedStaff->SetPhone(q.value(9).toByteArray().data());
			m_loggedStaff->SetAddress(q.value(10).toByteArray().data());
			m_loggedStaff->SetDescrp(q.value(11).toByteArray().data());
		}
	}

	QString get = QString(GET_IMAGE_BYID).arg(id);
	if(q.exec(get)) {
		if(q.next()) {
			*m_loggedStaffImage = q.value(0).toByteArray();
		}
	}
	
	closeDb();
	DBINFO("get logged staff completed:", id);
	return r;
}

