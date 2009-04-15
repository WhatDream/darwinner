#include <string>
#include "messagedef.h"
#include "Staff.h"
#include "Message.h"
#include "DUIHandler.h"
#include "SSLoginWindow.h"


SSLoginWindow::SSLoginWindow(QWidget *parent, DUIHandler* handler)
	: QDialog(parent)
{
	setupUi();
	QFont font = QFont("SimSun", 9);
	SettingFont(font);
	m_uiHandler = handler;
}

SSLoginWindow::~SSLoginWindow()
{

}

void SSLoginWindow::OnEvent(Message& ev) 
{
	switch(ev.type()) {
		case EVENT_LOGGEDIN:
		{
			hide();
			break;
		}
		case EVENT_SYSTEM_START:
		case EVENT_LOGGEDOFF:
		{
			show();
			break;
		}
		case EVENT_EXIT:
		{
			close();
			break;
		}
		default:
		{
			hide();
			break;
		}
	}
}

void SSLoginWindow::Login()
{
	Staff* stu = new Staff();
	stu->SetID(idLineEdit->text().toUInt());
	stu->SetPassword(pwLineEdit->text().toStdString());
	Message* action = new Message(stu);
	action->setType(ACTION_LOGIN);
	m_uiHandler->StartAction(*action);
	delete action;
}

void SSLoginWindow::setupUi()
{
	const QIcon icon = QIcon(QString::fromUtf8(":/icons/Resources/Cosmetic.png"));
	logoLabel = new QLabel(this);
	logoLabel->setAlignment(Qt::AlignCenter);
	logoLabel->setFrameShape(QFrame::StyledPanel);
	logoLabel->setFrameShadow(QFrame::Raised);
	logoLabel->setFixedSize(126, 126);
	logoLabel->setPixmap(QPixmap(":/icons/Resources/cosmetic-text.png").scaled(120, 120,Qt::KeepAspectRatio ,Qt::SmoothTransformation));
	setWindowTitle(QString::fromLocal8Bit("Cosmetic����˼������������ҵ��Ӫϵͳ"));

	loginGroupBox = new QGroupBox(QString::fromLocal8Bit("Ա�����"), this);

	idLabel = new QLabel(QString::fromLocal8Bit("����(&N)"), loginGroupBox);
	idLineEdit = new QLineEdit(loginGroupBox);
	idLabel->setBuddy(idLineEdit);
	idLineEdit->setValidator(new QIntValidator(idLineEdit));

	pwLabel = new QLabel(QString::fromLocal8Bit("����(&P)"), loginGroupBox);
	pwLineEdit = new QLineEdit(loginGroupBox);
	pwLineEdit->setEchoMode(QLineEdit::Password);
	pwLabel->setBuddy(pwLineEdit);

	pushButtonLogin = new QPushButton(this);
	pushButtonLogin->setText(QString::fromLocal8Bit("��¼(&L)"));
	pushButtonLogin->setMinimumHeight(30);
	pushButtonExit = new QPushButton(this);
	pushButtonExit->setText(QString::fromLocal8Bit("�˳�(&X)"));
	pushButtonExit->setMinimumHeight(30);
	QHBoxLayout *buttonLayout = new QHBoxLayout();
	buttonLayout->addWidget(pushButtonLogin);
	buttonLayout->addWidget(pushButtonExit);

	QGridLayout *loginLayout = new QGridLayout(loginGroupBox);
	loginLayout->addWidget(idLabel, 0, 0, 1, 1);
	loginLayout->addWidget(idLineEdit, 0, 1, 1, 3);
	loginLayout->addWidget(pwLabel, 1, 0, 1, 1);
	loginLayout->addWidget(pwLineEdit, 1, 1, 1, 3);
	loginLayout->addLayout(buttonLayout, 2, 0, 1, 4);
	loginGroupBox->setLayout(loginLayout);

	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	mainLayout->addWidget(logoLabel, 1);
	mainLayout->addWidget(loginGroupBox, 10);
	setLayout(mainLayout);

	setFixedSize(400, 150);

	connect(pushButtonExit, SIGNAL(clicked()), this, SLOT(close()));
	connect(pushButtonLogin, SIGNAL(clicked()), this, SLOT(Login()));
}

void SSLoginWindow::SettingFont(QFont& font)
 {
	 font.setBold(true);
	 loginGroupBox->setFont(font);

	 font.setBold(false);
	 pushButtonLogin->setFont(font);
	 pushButtonExit->setFont(font);
	 idLabel->setFont(font);
	 pwLabel->setFont(font);
	 pwLineEdit->setFont(font);
	 idLineEdit->setFont(font);
 }