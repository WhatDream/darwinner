 #include <QtGui>
#include "Guide.h"
#include "Message.h"
#include "messagedef.h"
#include "DUIHandler.h"
#include "Staff.h"
#include "JobTypeWidget.h"
#include "Status.h"
#include "LevelWidget.h"
#include <list>
#include "ProgressWidget.h"
#include "UiStrings.h"
using namespace std;

 Guide::Guide(QWidget *parent)
     : QWizard(parent)
 {
	setWizardStyle(QWizard::ModernStyle);
	QFont font = QFont("SimSun", 9);
	setFont(font);
	level = new LevelWidget(this, 0);
	job = new JobTypeWidget(this, 0);
	m_message = NULL;
	progressBar = NULL;
	totalStep = 0;
	isfinished = false;
	addPage(new IntroPage);
	addPage(new SupperUserPage);
	addPage(new StaffInfoPage(level, job, this));
	addPage(new StaffRightsPage);
	addPage(new ConclusionPage);
	setButtonText(QWizard::BackButton, LOCAL8BITSTR("��һ��"));
	setButtonText(QWizard::NextButton, LOCAL8BITSTR("��һ��"));
	setButtonText(QWizard::CommitButton, LOCAL8BITSTR("ȷ��"));
	setButtonText(QWizard::FinishButton, LOCAL8BITSTR("����"));
	setButtonText(QWizard::CancelButton, LOCAL8BITSTR("ȡ��"));
	setButtonText(QWizard::HelpButton, LOCAL8BITSTR("����"));

	setPixmap(QWizard::BannerPixmap, QPixmap(":/common/Resources/banner.png"));
	setPixmap(QWizard::LogoPixmap, QPixmap(":/common/Resources/cosmetic-text.png").scaled(100, 100,Qt::KeepAspectRatio ,Qt::SmoothTransformation));

	setWindowTitle(LOCAL8BITSTR("������"));
 }

 void Guide::OnEvent(Message& ev)
 {
	 switch(ev.type()) {
		case EVENT_WIZARD: 
		{
			show();
			break;
		}
		case EVENT_INIT:
		case EVENT_SETSUPERUSER:
		case EVENT_SETJOBTYPE:
		case EVENT_SETLEVELTYPE:
		{
			progressBar->setValue(++totalStep);
			break;
		}
		case EVENT_INIT_FINISHED:
		{
			progressBar->setValue(++totalStep);
			initData();
			break;
		}
		case EVENT_SETSTATUSTYPE://wait for this to indicate guide finished
		{
			progressBar->setValue(++totalStep);
			progressBar->close();
			if(!isfinished) {
				isfinished = true;
				m_message = new Message(ACTION_SYSTEM_START);
				m_uiHandler->StartAction(*m_message);
				delete m_message;
			}
			break;
		}
		case EVENT_SYSTEM_START:
		{
			 QDialog::accept();
			 break;
		}
	 }
 }

void Guide::accept()
{
	button(QWizard::BackButton)->setDisabled(true);
	button(QWizard::FinishButton)->setDisabled(true);
	button(QWizard::CancelButton)->setDisabled(true);
	progressBar = new ProgressWidget(LOCAL8BITSTR("���ڳ�ʼ�����ݿ⣬���Ժ򡭡�"), 0, 6, this);
	progressBar->show();
	totalStep = 0;
	initDb();
 }

 void Guide::initDb()
 {
	m_message = new Message(ACTION_INIT_DB);
	m_uiHandler->StartAction(*m_message);
	delete m_message;
 }

 void Guide::initData()
 {
 //*********************init super staff*************************//
	Staff* staff = new Staff();
	staff->SetPassword(field("password").toString().toLocal8Bit().data());	
	m_message = new Message(ACTION_SETSUPERUSER, staff);
	m_uiHandler->StartAction(*m_message);
	delete m_message;

//*********************init staff job*************************//
	list<Job>* jobList = job->getJobList();
//	if(jobList->empty())
	{
		Job temp;
		temp.setName("δ�趨");
		temp.setDescription("ϵͳĬ��ְ��");
		jobList->push_front(temp);
	}
	m_message = new Message(ACTION_SETJOBTYPE, (void*)jobList);
	m_uiHandler->StartAction(*m_message);
	delete m_message;

//*********************init staff level*************************//
	list<Level>* levelList = level->getLevelList();
//	if(levelList->empty())
	{
		Level temp;
		temp.setName("δ�趨");
		temp.setDescription("ϵͳĬ�ϵȼ�");
		levelList->push_front(temp);
	}
	m_message = new Message(ACTION_SETLEVELTYPE, (void*)levelList);
	m_uiHandler->StartAction(*m_message);
	delete m_message;

//*********************init staff status*************************//
	list<Status>* statusList = new list<Status>;
	Status temp;
	temp.setName("��ְ");
	statusList->push_back(temp);
	temp.setName("�ݼ�");
	statusList->push_back(temp);
	temp.setName("����");
	statusList->push_back(temp);
	temp.setName("æµ");
	statusList->push_back(temp);
	temp.setName("��ְ");
	statusList->push_back(temp);

	m_message = new Message(ACTION_SETSTATUSTYPE, (void*)statusList);
	m_uiHandler->StartAction(*m_message);
	delete m_message;
 }



 IntroPage::IntroPage(QWidget *parent)
     : QWizardPage(parent)
 {
	 setTitle(LOCAL8BITSTR("������"));
	 	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/common/Resources/watermark.png"));


	 label = new QLabel(LOCAL8BITSTR("��������һ������Cosmetic����˼������������ҵ��Ӫϵͳ�����򵼽������������ϵͳ����������Ļ������á����ˡ������û�����ص����ݣ��������þ��������պ��ٴ��޸ġ�"));
     label->setWordWrap(true);

     QVBoxLayout *layout = new QVBoxLayout;
     layout->addWidget(label);
     setLayout(layout);
 }

 SupperUserPage::SupperUserPage(QWidget *parent)
     : QWizardPage(parent)
 {
	setTitle(LOCAL8BITSTR("�����û�"));
	setSubTitle(LOCAL8BITSTR("�����û��ڱ�ϵͳ�о������Ȩ�ޣ���������һ�ε�¼ϵͳ�Լ����и��ָ߼���������ʹ�õ�Ψһ�ʺš��乤��Ϊ<font color=\"#ff0000\"; size=+1><b>10000</b></font>����Ϊ���������룬���μ�����Ϣ��"));

	passwordLabel = new QLabel(LOCAL8BITSTR("������"));
	passwordLineEdit = new QLineEdit;
	passwordLineEdit->setEchoMode(QLineEdit::Password);
	passwordLineEdit->setMaximumWidth(200);
	passwordLineEdit->setMaxLength(PW_MAX_LEN);
	passwordLabel->setBuddy(passwordLineEdit);

	confirmLabel = new QLabel(LOCAL8BITSTR("ȷ������"));
	confirmLineEdit = new QLineEdit;
	confirmLineEdit->setEchoMode(QLineEdit::Password);
	confirmLineEdit->setMaxLength(PW_MAX_LEN);
	confirmLineEdit->setMaximumWidth(200);
	confirmLabel->setBuddy(confirmLineEdit);

	registerField("password*", passwordLineEdit);
	registerField("confirm*", confirmLineEdit);


	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(passwordLabel, 0, 0);
	layout->addWidget(passwordLineEdit, 0, 1);
	layout->addWidget(confirmLabel, 1, 0);
	layout->addWidget(confirmLineEdit, 1, 1);
	layout->setContentsMargins(0, 20, 200, 30);
	setLayout(layout);
 }

 bool SupperUserPage::isComplete() const{
	 return !field("password").toString().isEmpty() && field("password").toString() == field("confirm").toString();
 }

//****************************StaffInfoPage************************//
 StaffInfoPage::StaffInfoPage(LevelWidget* levelWidget, JobTypeWidget* joblWidget, QWidget *parent)
     : QWizardPage(parent)
 {
	setTitle(LOCAL8BITSTR("ְ�񼰼���"));
	setSubTitle(LOCAL8BITSTR("��¼���˾��Ա��ְ��ͼ��𣬼����������Ϣ��ϵͳ�Ѿ�Ԥ����һЩ��Ŀ�������Լ�����ӡ��޸ġ�ɾ�����ڱ�ϵͳ�У�Ա��нˮ�ǰ��չ�ʽ��<center><font color=\"#336699\"><b>ְ����+������+ҵ����(ְ�����+�������)</b></font></center>������ġ�"));

	level = levelWidget;
	job = joblWidget;

	jobLabel = new QLabel(LOCAL8BITSTR("ְ�����ã�"));
	levelLabel = new QLabel(LOCAL8BITSTR("�������ã�"));
	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(jobLabel, 0, 0, 1, 1);
	layout->addWidget(levelLabel, 0, 1, 1, 1);
	layout->addWidget(job, 1, 0, 1, 1);
	layout->addWidget(level, 1, 1, 1, 1);
	layout->setMargin(0);
	setLayout(layout);
	}

 void StaffInfoPage::initializePage()
 {
	 {
	 list<Job> jobList;
	 Job temp;
	 temp.setName("�ϰ�");
	 jobList.push_back(temp);
	 temp.setName("�곤");
	 jobList.push_back(temp);
	 temp.setName("����Ա");
	 jobList.push_back(temp);
	 temp.setName("����ʦ");
	 jobList.push_back(temp);
	 temp.setName("��������");
	 jobList.push_back(temp);
	 temp.setName("����ʦ");
	 jobList.push_back(temp);
	 temp.setName("��������");
	 jobList.push_back(temp);
	 temp.setName("��Ħʦ");
	 jobList.push_back(temp);
	 temp.setName("��Ħ����");
	 jobList.push_back(temp);

	 job->pushjobs(&jobList);
	 }

	{
	list<Level> levelList;
	Level temp;
	temp.setName("��ʱԱ��");
	levelList.push_back(temp);
	temp.setName("����Ա��");
	levelList.push_back(temp);
	temp.setName("��ͨԱ��");
	levelList.push_back(temp);
	temp.setName("����Ա��");
	levelList.push_back(temp);
	temp.setName("����Ա��");
	levelList.push_back(temp);
	
	level->pushLevels(&levelList);
	}

 }

 StaffRightsPage::StaffRightsPage(QWidget *parent)
     : QWizardPage(parent)
 {
	 setTitle(LOCAL8BITSTR("ְ��Ȩ��"));
     setSubTitle(LOCAL8BITSTR("��Ϊ��ְͬ���Ա���������ڱ�ϵͳ�е�Ȩ�ޡ������ѡ��������ɲ���Ҫ����Ϣй¶��"));

/*
     outputDirLabel = new QLabel(tr("&Output directory:"));
     outputDirLineEdit = new QLineEdit;
     outputDirLabel->setBuddy(outputDirLineEdit);

     headerLabel = new QLabel(tr("&Header file name:"));
     headerLineEdit = new QLineEdit;
     headerLabel->setBuddy(headerLineEdit);

     implementationLabel = new QLabel(tr("&Implementation file name:"));
     implementationLineEdit = new QLineEdit;
     implementationLabel->setBuddy(implementationLineEdit);

     registerField("outputDir*", outputDirLineEdit);
     registerField("header*", headerLineEdit);
     registerField("implementation*", implementationLineEdit);

     QGridLayout *layout = new QGridLayout;
     layout->addWidget(outputDirLabel, 0, 0);
     layout->addWidget(outputDirLineEdit, 0, 1);
     layout->addWidget(headerLabel, 1, 0);
     layout->addWidget(headerLineEdit, 1, 1);
     layout->addWidget(implementationLabel, 2, 0);
     layout->addWidget(implementationLineEdit, 2, 1);
     setLayout(layout);
	 */
 }

 void StaffRightsPage::initializePage()
 {
	 /*
     QString className = field("className").toString();
     headerLineEdit->setText(className.toLower() + ".h");
     implementationLineEdit->setText(className.toLower() + ".cpp");
     outputDirLineEdit->setText(QDir::convertSeparators(QDir::tempPath()));
	 */
 }

 ConclusionPage::ConclusionPage(QWidget *parent)
     : QWizardPage(parent)
 {
     setTitle(LOCAL8BITSTR("�������"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/common/Resources/watermark.png"));

     label = new QLabel;
     label->setWordWrap(true);

     QVBoxLayout *layout = new QVBoxLayout;
     layout->addWidget(label);
     setLayout(layout);

 }

 void ConclusionPage::initializePage()
 {
     QString finishText = wizard()->buttonText(QWizard::FinishButton);
     finishText.remove('&');
	 label->setText(LOCAL8BITSTR("������%1����������򵼣������¼���档")
                    .arg(finishText));
 }
