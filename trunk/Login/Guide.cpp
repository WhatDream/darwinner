 #include <QtGui>

#include "Guide.h"
#include "Message.h"
#include "messagedef.h"
#include "DUIHandler.h"
#include "Staff.h"
#include "JobTypeWidget.h"
#include "LevelWidget.h"
#include <list>
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
	addPage(new IntroPage);
	addPage(new SupperUserPage);
	addPage(new StaffInfoPage(level, job, this));
	addPage(new StaffRightsPage);
	addPage(new ConclusionPage);
	setButtonText(QWizard::BackButton, QString::fromLocal8Bit("��һ��"));
	setButtonText(QWizard::NextButton, QString::fromLocal8Bit("��һ��"));
	setButtonText(QWizard::CommitButton, QString::fromLocal8Bit("ȷ��"));
	setButtonText(QWizard::FinishButton, QString::fromLocal8Bit("����"));
	setButtonText(QWizard::CancelButton, QString::fromLocal8Bit("ȡ��"));
	setButtonText(QWizard::HelpButton, QString::fromLocal8Bit("����"));

	setPixmap(QWizard::BannerPixmap, QPixmap(":/icons/Resources/banner.png"));
	setPixmap(QWizard::LogoPixmap, QPixmap(":/icons/Resources/cosmetic-text.png").scaled(100, 100,Qt::KeepAspectRatio ,Qt::SmoothTransformation));

	setWindowTitle(QString::fromLocal8Bit("������"));
 }

 void Guide::OnEvent(Message& ev)
 {
	 switch(ev.type()) {
		case EVENT_WIZARD: 
		{
			show();
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
/*add super staff by Tim Kuo 2009-04-17*/
	Staff* staff = new Staff();
	staff->SetPassword(field("password").toByteArray().data());	
	m_message = new Message(ACTION_SETSUPERUSER, staff);
	m_uiHandler->StartAction(*m_message);
	delete m_message;

/*add job type by Tim Kuo 2009-04-17*/
	list<Job>* jobList = job->getJobList();
	m_message = new Message(ACTION_SETJOBTYPE, (void*)jobList);
	m_uiHandler->StartAction(*m_message);
	delete m_message;

/*add level type by Tim Kuo 2009-04-17*/
	list<Level>* levelList = level->getLevelList();
	m_message = new Message(ACTION_SETLEVELTYPE, (void*)levelList);
	m_uiHandler->StartAction(*m_message);
	delete m_message;

/*system start up by Tim Kuo 2009-04-17*/
	m_message = new Message(ACTION_SYSTEM_START);
	m_uiHandler->StartAction(*m_message);
	delete m_message;
 }

 IntroPage::IntroPage(QWidget *parent)
     : QWizardPage(parent)
 {
	 setTitle(QString::fromLocal8Bit("������"));
	 	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/icons/Resources/watermark.png"));


	 label = new QLabel(QString::fromLocal8Bit("��������һ������Cosmetic����˼������������ҵ��Ӫϵͳ�����򵼽������������ϵͳ����������Ļ������á����ˡ������û�����ص����ݣ��������þ��������պ��ٴ��޸ġ�"));
     label->setWordWrap(true);

     QVBoxLayout *layout = new QVBoxLayout;
     layout->addWidget(label);
     setLayout(layout);
 }

 SupperUserPage::SupperUserPage(QWidget *parent)
     : QWizardPage(parent)
 {
	setTitle(QString::fromLocal8Bit("�����û�"));
	setSubTitle(QString::fromLocal8Bit("�����û��ڱ�ϵͳ�о������Ȩ�ޣ���������һ�ε�¼ϵͳ�Լ����и��ָ߼���������ʹ�õ�Ψһ�ʺš��乤��Ϊ<font color=\"#ff0000\"; size=+1><b>10000</b></font>����Ϊ���������룬���μ�����Ϣ��"));

	passwordLabel = new QLabel(QString::fromLocal8Bit("������"));
	passwordLineEdit = new QLineEdit;
	passwordLineEdit->setEchoMode(QLineEdit::Password);
	passwordLineEdit->setMaximumWidth(200);
	passwordLineEdit->setMaxLength(PW_MAX_LEN);
	passwordLabel->setBuddy(passwordLineEdit);

	confirmLabel = new QLabel(QString::fromLocal8Bit("ȷ������"));
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
	setTitle(QString::fromLocal8Bit("ְ��Ա������"));
	setSubTitle(QString::fromLocal8Bit("��¼���˾��Ա��ְ��Ա���ȼ����Լ����Ե�ҵ����ɱ�����"));

	level = levelWidget;
	job = joblWidget;

	jobLabel = new QLabel(QString::fromLocal8Bit("ְ�����ã�"));
	levelLabel = new QLabel(QString::fromLocal8Bit("�ȼ����ã�"));
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
	 /*
     QString className = field("className").toString();
     macroNameLineEdit->setText(className.toUpper() + "_H");

     QString baseClass = field("baseClass").toString();

     includeBaseCheckBox->setChecked(!baseClass.isEmpty());
     includeBaseCheckBox->setEnabled(!baseClass.isEmpty());
     baseIncludeLabel->setEnabled(!baseClass.isEmpty());
     baseIncludeLineEdit->setEnabled(!baseClass.isEmpty());

     if (baseClass.isEmpty()) {
         baseIncludeLineEdit->clear();
     } else if (QRegExp("Q[A-Z].*").exactMatch(baseClass)) {
         baseIncludeLineEdit->setText("<" + baseClass + ">");
     } else {
         baseIncludeLineEdit->setText("\"" + baseClass.toLower() + ".h\"");
     }
	 */
 }

 StaffRightsPage::StaffRightsPage(QWidget *parent)
     : QWizardPage(parent)
 {
	 setTitle(QString::fromLocal8Bit("ְ��Ȩ��"));
     setSubTitle(QString::fromLocal8Bit("��Ϊ��ְͬ���Ա���������ڱ�ϵͳ�е�Ȩ�ޡ������ѡ��������ɲ���Ҫ����Ϣй¶��"));

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
     setTitle(QString::fromLocal8Bit("�������"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/icons/Resources/watermark.png"));

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
	 label->setText(QString::fromLocal8Bit("������%1����������򵼣������¼���档")
                    .arg(finishText));
 }
