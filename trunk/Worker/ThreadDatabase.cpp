#include "ThreadDatabase.h"
#include "common.h"
#include "messagedef.h"
#include "Message.h"
#include <QCoreApplication>
#include "TEvent.h"
#include "Staff.h"
 #include <QSqlQuery>
#include <QVariant>
#include "dbquery.h"
#include <fstream>
#include <QSqlRecord>
#include "Job.h"
#include "Level.h"
#include "Status.h"


ThreadDatabase::ThreadDatabase(QObject *parent)
:QThread(parent)
{
	m_tempMsg = NULL;
	m_loggedstaff = new Staff();
	db = QSqlDatabase::addDatabase("QSQLITE");
}

ThreadDatabase::~ThreadDatabase()
{
	if(NULL != m_tempMsg)
		delete m_tempMsg;
}

void ThreadDatabase::run() {
	DBINFO("database thread start @", this);
	for(;;) {
		mutex.lock();
		if(m_listActionBuffer.empty())
			bufferEmpty.wait(&mutex);
		mutex.unlock();

		mutex.lock();
		Message Action = m_listActionBuffer.front();
		m_listActionBuffer.pop_front();
		mutex.unlock();

		switch(Action.type()) {
			case ACTION_SYSTEM_START:
			{
				if(checkDd()) {
					m_tempMsg = new Message(EVENT_SYSTEM_START);	
				}else{
					m_tempMsg = new Message(EVENT_WIZARD);
				}
				QEvent* ev= new TEvent((QEvent::Type)EventDb, m_tempMsg);
				QCoreApplication::postEvent(this->parent(), ev,Qt::HighEventPriority);
				break;
			}
			case ACTION_LOGIN:
			{
				Staff* staffIncome = static_cast<Staff*>(Action.data());
				Staff* staffDb = getStaff(staffIncome->ID());
				if(staffDb->ID() != 0 &&
					staffIncome->Password() == staffDb->Password() &&
					staffIncome->ID() == staffDb->ID()) {
					*m_loggedstaff = *staffDb;
					m_tempMsg = new Message(EVENT_LOGGEDIN, staffDb);
					QEvent* ev = new TEvent((QEvent::Type)EventDb, m_tempMsg);
					QCoreApplication::postEvent(this->parent(), ev,Qt::HighEventPriority);
				}
				delete staffIncome;
				break;
			}
			case ACTION_LOGOFF:
			{
				m_loggedstaff->clear();
				m_tempMsg = new Message(EVENT_LOGGEDOFF);
				QEvent* ev = new TEvent((QEvent::Type)EventDb, m_tempMsg);
				QCoreApplication::postEvent(this->parent(), ev,Qt::HighEventPriority);
				break;
			}
			case ACTION_GEALLSTAFF:
			{
				m_tempMsg = new Message(EVENT_ALLSTAFF, getAllStaffs());
				QEvent* ev = new TEvent((QEvent::Type)EventDb, m_tempMsg);
				QCoreApplication::postEvent(this->parent(), ev,Qt::HighEventPriority);
				break;
			}
			case ACTION_GETSTAFF:
			{
				uint32* id = static_cast<uint32*>(Action.data());
				m_tempMsg = new Message(EVENT_STAFF, getStaff(*id));
				QEvent* ev = new TEvent((QEvent::Type)EventDb, m_tempMsg);
				QCoreApplication::postEvent(this->parent(), ev,Qt::HighEventPriority);
				delete id;
				break;
			}
			case ACTION_ADDSTAFF:
			{
				Staff* staff = static_cast<Staff*>(Action.data());
				QByteArray* image = static_cast<QByteArray*>(Action.data2());
				Staff* addedStaff = NULL;
				if(addStaff(staff)) {
					addedStaff = getStaff(staff->ID());
				}
				if(NULL != image && !image->isEmpty() && addedStaff->ID() != 0) {
					setImage(addedStaff->ID(), *image);
				}
				delete staff;
				m_tempMsg = new Message(EVENT_STAFFADDED, addedStaff);
				QEvent* ev = new TEvent((QEvent::Type)EventDb, m_tempMsg);
				QCoreApplication::postEvent(this->parent(), ev,Qt::HighEventPriority);
				break;
			}
			case ACTION_SETSUPERUSER:
			{
				Staff* staff = static_cast<Staff*>(Action.data());
				addSupperStaff(staff);
				delete staff;
				break;
			}
			case ACTION_SETSTATUSTYPE:
			{
				list<Status>* statusList = static_cast<list<Status>*>(Action.data());
				list<Status>* result = new list<Status>;
				list<Status>::iterator it = statusList->begin();
				while(statusList->end() != it) {
					if(!addStatusType(&(*it)))
						result->push_back(*it);
					it++;
				}
				delete statusList;
				m_tempMsg = new Message(EVENT_SETSTATUSTYPE, result);
				QEvent* ev = new TEvent((QEvent::Type)EventDb, m_tempMsg);
				QCoreApplication::postEvent(this->parent(), ev,Qt::HighEventPriority);
				break;
			}
			case ACTION_SETJOBTYPE:
			{
				list<Job>* jobList = static_cast<list<Job>*>(Action.data());
				list<Job>* result = new list<Job>;
				list<Job>::iterator it = jobList->begin();
				while(jobList->end() != it) {
					if(!addJobType(&(*it)))
						result->push_back(*it);
					it++;
				}
				delete jobList;
				m_tempMsg = new Message(EVENT_SETJOBTYPE, result);
				QEvent* ev = new TEvent((QEvent::Type)EventDb, m_tempMsg);
				QCoreApplication::postEvent(this->parent(), ev,Qt::HighEventPriority);
				break;
			}
			case ACTION_REMOVEJOBTYPE:
			{
				list<Job>* jobList = static_cast<list<Job>*>(Action.data());
				list<Job>* result = new list<Job>;
				list<Job>::iterator it = jobList->begin();
				while(jobList->end() != it) {
					if(!removeJob(&(*it)))
						result->push_back(*it);
					it++;
				}
				delete jobList;
				m_tempMsg = new Message(EVENT_REMOVEJOBTYPE, result);
				QEvent* ev = new TEvent((QEvent::Type)EventDb, m_tempMsg);
				QCoreApplication::postEvent(this->parent(), ev,Qt::HighEventPriority);
				break;
			}
			case ACTION_SETLEVELTYPE:
			{
				list<Level>* levelList = static_cast<list<Level>*>(Action.data());
				list<Level>* result = new list<Level>;
				list<Level>::iterator it = levelList->begin();
				while(levelList->end() != it) {
					if(!addLevelType(&(*it)))
						result->push_back(*it);
					it++;
				}
				delete levelList;
				m_tempMsg = new Message(EVENT_SETLEVELTYPE, result);
				QEvent* ev = new TEvent((QEvent::Type)EventDb, m_tempMsg);
				QCoreApplication::postEvent(this->parent(), ev,Qt::HighEventPriority);
				break;
			}
			case ACTION_REMOVELEVELTYPE:
			{
				list<Level>* levelList = static_cast<list<Level>*>(Action.data());
				list<Level>* result = new list<Level>;
				list<Level>::iterator it = levelList->begin();
				while(levelList->end() != it) {
					if(!removeLevel(&(*it)))
						result->push_back(*it);
					it++;
				}
				delete levelList;
				m_tempMsg = new Message(EVENT_REMOVELEVELTYPE, result);
				QEvent* ev = new TEvent((QEvent::Type)EventDb, m_tempMsg);
				QCoreApplication::postEvent(this->parent(), ev,Qt::HighEventPriority);
				break;
			}
			case ACTION_CHANGEPASSWORD:
			{
				list<string>* pwList = static_cast<list<string>*>(Action.data());
				string oldpw = pwList->front();
				string newpw = pwList->back();
				changePassword(oldpw, newpw);
				delete pwList;
				break;
			}

			case ACTION_GETJOBTYPE:
			{
				m_tempMsg = new Message(EVENT_JOBTYPE, getJobs());
				QEvent* ev = new TEvent((QEvent::Type)EventDb, m_tempMsg);
				QCoreApplication::postEvent(this->parent(), ev,Qt::HighEventPriority);
				break;
			}
			case ACTION_GETLEVELTYPE:
			{
				m_tempMsg = new Message(EVENT_LEVELTYPE, getLevels());
				QEvent* ev = new TEvent((QEvent::Type)EventDb, m_tempMsg);
				QCoreApplication::postEvent(this->parent(), ev,Qt::HighEventPriority);
				break;
			}
			case ACTION_GETSTATUSTYPE:
			{
				m_tempMsg = new Message(EVENT_STATUSTYPE, getStatus());
				QEvent* ev = new TEvent((QEvent::Type)EventDb, m_tempMsg);
				QCoreApplication::postEvent(this->parent(), ev,Qt::HighEventPriority);
				break;
			}
			case ACTION_MODIFYSTAFF:
			{
				Staff* staff = static_cast<Staff*>(Action.data());
				QByteArray* image = static_cast<QByteArray*>(Action.data2());
				Staff* modifieddStaff = NULL;
				if(modifyStaff(staff)) {
					modifieddStaff = getStaff(staff->ID());
				}
				if(NULL != image && !image->isEmpty() && modifieddStaff->ID() != 0) {
					setImage(modifieddStaff->ID(), *image);
				}
				delete staff;
				m_tempMsg = new Message(EVENT_STAFFMODIFIED, modifieddStaff);
				QEvent* ev = new TEvent((QEvent::Type)EventDb, m_tempMsg);
				QCoreApplication::postEvent(this->parent(), ev,Qt::HighEventPriority);
				break;
			}
			case ACTION_REMOVESTAFF:
			{
				uint32* id = static_cast<uint32*>(Action.data());
				m_tempMsg = new Message(EVENT_STAFFREMOVED, removeStaff(*id));
				QEvent* ev = new TEvent((QEvent::Type)EventDb, m_tempMsg);
				QCoreApplication::postEvent(this->parent(), ev,Qt::HighEventPriority);
				delete id;
				break;
			}
			case ACTION_GETPICTURE:
			{
				uint32* id = static_cast<uint32*>(Action.data());
				m_tempMsg = new Message(EVENT_GETPICTURE, getImage(*id));
				QEvent* ev = new TEvent((QEvent::Type)EventDb, m_tempMsg);
				QCoreApplication::postEvent(this->parent(), ev,Qt::HighEventPriority);
				delete id;
				break;
			}

			default:
				break;	
		}
	}
}

void ThreadDatabase::QueueAction(Message& Action)
{
	if(!isRunning()) {
		start(HighPriority);
	}

	mutex.lock();
	m_listActionBuffer.push_back(Action);
	mutex.unlock();


	mutex.lock();
	if(!m_listActionBuffer.empty())
		bufferEmpty.wakeOne();
	mutex.unlock();

}

bool ThreadDatabase::initDb()
{
	
	db.setDatabaseName(DBNAME);
	if(!db.open()) {
		return false;
	}
	DBINFO("initializing database...", "");
	QSqlQuery q = QSqlQuery(db);
	q.exec(CREATE_STAFF_TABLE);
	q.exec(CREATE_JOB_TABLE);
	q.exec(CREATE_LEVET_TABLE);
	q.exec(CREATE_ORDERS_TABLE);
	q.exec(CREATE_GOODS_TABLE);
	q.exec(CREATE_SEX_TABLE);
	q.exec(CREATE_STATUS_TABLE);
	q.exec(CREATE_TASKS_TABLE);
	q.exec(CREATE_GOOSTYPE_TABLE);

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

	db.close();
	DBINFO("databese initialized.", "");
	return true;
}

bool ThreadDatabase::checkDd()
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
	  testfile.seekg (0, ios::end);
	  length = testfile.tellg();
	  testfile.seekg (0, ios::beg);

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
		QEvent* ev1 = new TEvent((QEvent::Type)EventDb, m_tempMsg);
		QCoreApplication::postEvent(this->parent(), ev1,Qt::HighEventPriority);
		initDb();
		m_tempMsg = new Message(EVENT_INIT_FINISHED);
		QEvent* ev2 = new TEvent((QEvent::Type)EventDb, m_tempMsg);
		QCoreApplication::postEvent(this->parent(), ev2,Qt::HighEventPriority);
		re = false;
	} else {//database exists. check super user
		testfile.close();
		db.setDatabaseName(DBNAME);
		db.open();
		QSqlQuery q = QSqlQuery(db);
		QString query = QString("SELECT * FROM staff WHERE id = %1").arg(SUPERUSERID);
		q.exec(query);
		if(!q.next())
			re = false;
		else
			re = true;
		db.close();
	}
	return re;
}

Staff* ThreadDatabase::getStaff(uint32 id) 
{
	db.setDatabaseName(DBNAME);
	db.open();
	QSqlQuery q = QSqlQuery(db);
	QString query = QString(SELECT_STAFF_BYID_NOIMAGE).arg(id);
	q.exec(query);
	Staff* temp = new Staff();
	if(q.next()) {
		temp->SetID(q.value(0).toUInt());
		temp->SetPassword(q.value(1).toByteArray().data());
		temp->SetName(q.value(2).toByteArray().data());
		temp->SetType(q.value(3).toUInt());
		temp->SetLevel(q.value(4).toUInt());
		temp->SetSex(q.value(5).toUInt());
		temp->SetBaseSalary(q.value(6).toUInt());
		temp->SetStatus(q.value(7).toUInt());
		temp->SetCell(q.value(8).toByteArray().data());
		temp->SetPhone(q.value(9).toByteArray().data());
		temp->SetAddress(q.value(10).toByteArray().data());
		temp->SetDescrp(q.value(11).toByteArray().data());
	}
	db.close();
	DBINFO("get one staff completed:", temp->Name())
	return temp;
}
bool ThreadDatabase::addStaff(Staff* staff)
{
	bool r =false;
	db.setDatabaseName(DBNAME);
	r = db.open();
	if(!r) {
		return r;
	}
	DBINFO("adding satff...", "");
	QSqlQuery q = QSqlQuery(db);

	q.prepare(INSERTINTO_STAFF);

	q.bindValue(":password", staff->Password().c_str());
	q.bindValue(":name", staff->Name().c_str());
	q.bindValue(":jobId", staff->Type());
	q.bindValue(":levelId", staff->Level());
	q.bindValue(":sex", staff->Sex());
	q.bindValue(":baseSalary", staff->baseSalary());
	q.bindValue(":status", staff->status());
	q.bindValue(":cell", staff->cell().c_str());
	q.bindValue(":phone", staff->phone().c_str());
	q.bindValue(":address", staff->address().c_str());
	q.bindValue(":descrption", staff->Descrp().c_str());
	r = q.exec();
	if(r) {
		q.exec("SELECT MAX(id) FROM staff");
		if(q.next()) staff->SetID(q.value(0).toUInt());
		QString setpw = QString("UPDATE staff SET password=%1 WHERE id = %2").arg(staff->ID()).arg(staff->ID());
		r = q.exec(setpw);
	}
	db.close();
	DBINFO("add satff complete", r);
	return r;
}

bool ThreadDatabase::setImage(uint32 id, QByteArray& image)
{
	bool r =false;
	db.setDatabaseName(DBNAME);
	r = db.open();
	if(!r) {
		return r;
	}
	DBINFO("setting image for:", id);
	QSqlQuery q = QSqlQuery(db);

	QString set = QString(SET_PIC).arg(QString::fromLocal8Bit(image.data(), image.size())).arg(id);
//	r = q.prepare(set);
//	q.bindValue(":image", image);
	r = q.exec(set);
	db.close();
	DBINFO("set image complete", r);
	return r;

}
QByteArray* ThreadDatabase::getImage(uint32 id)
{
	QByteArray* image = new QByteArray;
	image->clear();
	db.setDatabaseName(DBNAME);
	if(!db.open()) {
		return image;
	}
	DBINFO("setting image for:", id);
	QSqlQuery q = QSqlQuery(db);

	QString get = QString(GET_PIC).arg(id);
	if(q.exec(get)) {
		if(q.next()) {
			*image = q.value(0).toByteArray();
		}
	}
	db.close();
	DBINFO("set image complete", "");
	return image;
}

uint32* ThreadDatabase::removeStaff(uint32 id)
{
	uint32* r = new uint32(id);
	db.setDatabaseName(DBNAME);
	if(!db.open()) {
		*r = 0;
		return r;
	}
	DBINFO("removing satff...", "");
	QSqlQuery q = QSqlQuery(db);
	QString remove = QString(DELETE_STAFF_BYID).arg(id);
	if(!q.exec(remove)) 
	{
		*r = 0;
	}
	db.close();
	DBINFO("remove satff complete", *r);
	return r;
}


bool ThreadDatabase::modifyStaff(Staff* staff)
{
	bool r =false;
	db.setDatabaseName(DBNAME);
	r = db.open();
	if(!r) {
		return r;
	}
	DBINFO("modifying satff...", "");
	QSqlQuery q = QSqlQuery(db);

	QString modifstr = QString(UPDATA_STAFF_BASIC).arg(staff->Name().c_str()).arg(staff->Type()).arg(staff->Level()).arg(staff->Sex()).arg(staff->baseSalary()).arg(staff->status()).arg(staff->cell().c_str()).arg(staff->phone().c_str()).arg(staff->address().c_str()).arg(staff->Descrp().c_str()).arg(staff->ID());
/*
	q.prepare(modifstr);

	q.bindValue(":name", staff->Name().c_str());
	q.bindValue(":jobId", staff->Type());
	q.bindValue(":levelId", staff->Level());
	q.bindValue(":sex", staff->Sex());
	q.bindValue(":baseSalary", staff->baseSalary());
	q.bindValue(":status", staff->status());
	q.bindValue(":cell", staff->cell().c_str());
	q.bindValue(":phone", staff->phone().c_str());
	q.bindValue(":address", staff->address().c_str());
	q.bindValue(":descrption", staff->Descrp().c_str());
	*/
	r = q.exec(modifstr);
	db.close();
	DBINFO("modify satff complete", r);
	return r;
}

bool ThreadDatabase::addSupperStaff(Staff* staff)
{
	db.setDatabaseName(DBNAME);
	if(!db.open()) {
		return false;
	}
	QSqlQuery q = QSqlQuery(db);
	
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
	q.bindValue(":image", "");
	q.exec();
	DBINFO("create super user complete!", "");
	return true;
}

bool ThreadDatabase::addJobType(Job* job)
{
	bool r = false;
	db.setDatabaseName(DBNAME);
	if(!db.open()) {
		return false;
	}
	QSqlQuery q = QSqlQuery(db);
	QString qstring = QString(SELECT_JOB_BYID).arg(job->id());
	q.exec(qstring);
	if (q.next()) 
	{
		QString update = QString(UPDATA_JOB).arg(job->name().c_str()).arg(job->profit()).arg(job->description().c_str()).arg(job->id());
		r = q.exec(update);
	}
	else
	{
		q.prepare(INSERTINTO_JOB_TABLE);
		q.bindValue(":name", job->name().c_str());
		q.bindValue(":profit", job->profit());
		q.bindValue(":descrption", job->description().c_str());
		r = q.exec();
	}
	db.close();
	return r;
}

bool ThreadDatabase::addLevelType(Level* level)
{
	bool r = false;
	db.setDatabaseName(DBNAME);
	if(!db.open()) {
		return false;
	}
	QSqlQuery q = QSqlQuery(db);
	QString qstring = QString(SELECT_LEVEL_BYID).arg(level->id());
	q.exec(qstring);
	if (q.next()) 
	{
		QString update = QString(UPDATA_LEVEL).arg(level->name().c_str()).arg(level->profit()).arg(level->description().c_str()).arg(level->id());
		r = q.exec(update);
	}
	else
	{
		q.prepare(INSERTINTO_LEVEL_TABLE);
		q.bindValue(":name", level->name().c_str());
		q.bindValue(":profit", level->profit());
		q.bindValue(":descrption", level->description().c_str());
		r = q.exec();
	}
	db.close();
	return r;
}

bool ThreadDatabase::addStatusType(Status* status)
{
	bool r = false;
	db.setDatabaseName(DBNAME);
	if(!db.open()) {
		return false;
	}
	QSqlQuery q = QSqlQuery(db);
	QString qstring = QString(SELECT_STATUS_BYID).arg(status->id());
	q.exec(qstring);
	if (q.next()) 
	{
		QString update = QString(UPDATA_STATUS).arg(status->name().c_str()).arg(status->description().c_str()).arg(status->id());
		r = q.exec(update);
	}
	else
	{
		q.prepare(INSERTINTO_STATUS_TABLE);
		q.bindValue(":name", status->name().c_str());
		q.bindValue(":descrption", status->description().c_str());
		r = q.exec();
	}
	db.close();
	return r;
}


list<Staff>* ThreadDatabase::getAllStaffs()
{
	db.setDatabaseName(DBNAME);
	db.open();
	QSqlQuery q = QSqlQuery(db);
	q.exec(SELECT_STAFF_NOIMAGE);
	Staff temp;
	list<Staff>* r = new list<Staff>;
	r->clear();
	while (q.next()) {
		temp.SetID(q.value(0).toUInt());
		temp.SetPassword(q.value(1).toByteArray().data());
		temp.SetName(q.value(2).toByteArray().data());
		temp.SetType(q.value(3).toUInt());
		temp.SetLevel(q.value(4).toUInt());
		temp.SetSex((byte)(q.value(5).toUInt()));
		temp.SetBaseSalary(q.value(6).toInt());
		temp.SetStatus(q.value(7).toUInt());
		temp.SetCell(q.value(8).toByteArray().data());
		temp.SetPhone(q.value(9).toByteArray().data());
		temp.SetAddress(q.value(10).toByteArray().data());
		temp.SetDescrp(q.value(11).toByteArray().data());
		r->push_back(temp);
	}
	db.close();
	DBINFO("get all staffs. amount: ", r->size());
	return r;
}

list<Job>* ThreadDatabase::getJobs()
{
	db.setDatabaseName(DBNAME);
	db.open();
	QSqlQuery q = QSqlQuery(db);
	q.exec(SELECT_JOB_ALL);
	Job temp;
	list<Job>* r = new list<Job>;
	r->clear();
	while (q.next()) {
		temp.setId(q.value(0).toUInt());
		temp.setName(q.value(1).toByteArray().data());
		temp.setProfit(q.value(2).toUInt());
		temp.setDescription(q.value(3).toByteArray().data());
		r->push_back(temp);
	}
	db.close();
	DBINFO("get all jobs. amount: ", r->size());
	return r;
}

list<Level>* ThreadDatabase::getLevels()
{
	db.setDatabaseName(DBNAME);
	db.open();
	QSqlQuery q = QSqlQuery(db);
	q.exec(SELECT_LEVEL_ALL);
	Level temp;
	list<Level>* r = new list<Level>;
	r->clear();
	while (q.next()) {
		temp.setId(q.value(0).toUInt());
		temp.setName(q.value(1).toByteArray().data());
		temp.setProfit(q.value(2).toUInt());
		temp.setDescription(q.value(3).toByteArray().data());
		r->push_back(temp);
	}
	db.close();
	DBINFO("get all levels. amount: ", r->size());
	return r;
}

list<Status>* ThreadDatabase::getStatus()
{
	db.setDatabaseName(DBNAME);
	db.open();
	QSqlQuery q = QSqlQuery(db);
	q.exec(SELECT_STATUS_ALL);
	Status temp;
	list<Status>* r = new list<Status>;
	r->clear();
	while (q.next()) {
		temp.setId(q.value(0).toUInt());
		temp.setName(q.value(1).toByteArray().data());
		temp.setDescription(q.value(2).toByteArray().data());
		r->push_back(temp);
	}
	db.close();
	DBINFO("get all status. amount: ", r->size());
	return r;
}

bool ThreadDatabase::changePassword(string oldpw, string newpw)
{
	bool r =false;
	db.setDatabaseName(DBNAME);
	r = db.open();
	if(!r) {
		return r;
	}
	DBINFO("changing password...", "");
	QSqlQuery q = QSqlQuery(db);

	QString getold = QString("SELECT password FROM staff WHERE id = %1").arg(m_loggedstaff->ID());
	if(r = q.exec(getold)) {
		if(r = q.next()) {
			if(oldpw != string(q.value(0).toByteArray().data())) return false;
		}else{
			return r;
		}
	} else {
		return r;
	}

	QString setnew = QString("UPDATE staff SET password = '%1' WHERE id = %2").arg(newpw.c_str()).arg(m_loggedstaff->ID());
	r = q.exec(setnew);
	db.close();
	DBINFO("change password complete", r);
	return r;
}

bool ThreadDatabase::removeJob(Job* id)
{
	bool r =false;
	db.setDatabaseName(DBNAME);
	r = db.open();
	if(!r) {
		return r;
	}
	DBINFO("removing job...", "");
	QSqlQuery q = QSqlQuery(db);

	QString check = QString(CHECK_JOB_BYID).arg(id->id());
	if(q.exec(check)) {
		if(q.next())
			return false;
	}

	QString remove = QString(DELETE_JOB_BYID).arg(id->id());
	r = q.exec(remove);
	db.close();
	DBINFO("remove job complete", r);
	return r;
}

bool ThreadDatabase::removeLevel(Level* id)
{
	bool r =false;
	db.setDatabaseName(DBNAME);
	r = db.open();
	if(!r) {
		return r;
	}
	DBINFO("removing level...", "");
	QSqlQuery q = QSqlQuery(db);

	QString check = QString(CHECK_LEVEL_BYID).arg(id->id());
	if(q.exec(check)) {
		if(q.next())
			return false;
	}

	QString remove = QString(DELETE_LEVEL_BYID).arg(id->id());
	r = q.exec(remove);
	db.close();
	DBINFO("remove job complete", r);
	return r;
}

