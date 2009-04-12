#include "StaffManagementUI.h"
#include "messagedef.h"
#include "Message.h"
#include "DUIHandler.h"
#include <QFont>
#include <QPoint>

const char* StaffType[] = {"�����û�",  "�ϰ�", "�곤", "����Ա", "����ʦ", "ϴ��ʦ", "����ʦ", "��Ħʦ"};
const char* StaffLevel[] = {"����","������","���鹤","�߼���","��ʦ"};
const char* StaffSex[] = {"δ֪","��","Ů"};

StaffManagementUI::StaffManagementUI()
{
	setupUi();
	started = false;
	font = QFont("SimSun", 9);
	SettingFont(font);

	m_stuffDataModel = new QStandardItemModel(0, 6, this);
	m_stuffDataModel->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("Ա�����"));
	m_stuffDataModel->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("����"));
	m_stuffDataModel->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("�Ա�"));
	m_stuffDataModel->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("ְ��"));
	m_stuffDataModel->setHeaderData(4, Qt::Horizontal, QString::fromLocal8Bit("����"));
	m_stuffDataModel->setHeaderData(5, Qt::Horizontal, QString::fromLocal8Bit("����"));

	m_sortProxyModel = new QSortFilterProxyModel;
	m_sortProxyModel->setSourceModel(m_stuffDataModel);
	m_sortProxyModel->setDynamicSortFilter(true);
	m_sortProxyModel->setFilterKeyColumn(-1);
	
	treeViewStaff->setModel(m_sortProxyModel);
	treeViewStaff->sortByColumn(0, Qt::AscendingOrder);
    comboBoxItem->setCurrentIndex(0);
    checkBoxSort->setChecked(true);
    checkBoxSearch->setChecked(true);

	connect(lineEditKeyword, SIGNAL(textChanged(const QString &)), this, SLOT(filterRegExpChanged()));
	connect(comboBoxPattern, SIGNAL(currentIndexChanged(int)),this, SLOT(filterRegExpChanged()));
	connect(comboBoxItem, SIGNAL(currentIndexChanged(int)), this, SLOT(filterColumnChanged()));
	connect(checkBoxSearch, SIGNAL(toggled(bool)), this, SLOT(filterRegExpChanged()));
	connect(checkBoxSort, SIGNAL(toggled(bool)), this, SLOT(sortChanged()));
	//connect(actionExport, SIGNAL(actionExport->triggered)
	connect(actionMenu, SIGNAL(triggered(bool)), this, SLOT(Menu()));
	connect(actionLogOff, SIGNAL(triggered(bool)), this, SLOT(Logoff()));
	connect(actionExit, SIGNAL(triggered(bool)), this, SLOT(Exit()));
	connect(actionStaffDetail, SIGNAL(triggered(bool)), this, SLOT(staffDetail()));
	connect(actionDeleteStaff, SIGNAL(triggered(bool)), this, SLOT(removeStaff()));
	connect(actionCasher4Client, SIGNAL(triggered(bool)), this, SLOT(cash4Client()));
}

StaffManagementUI::~StaffManagementUI()
{

}

void StaffManagementUI::OnEvent(Message & Msg){
	switch(Msg.type()) {
		case EVENT_STAFFMGNT:
		{
			if(isHidden())
				show();
			else
				this->setFocus();
			break;
		}
		case EVENT_LOGGEDOFF:
		{
			close();
			break;
		}
		case EVENT_EXIT:
		{
			close();
			break;
		}
		case EVENT_STAFFS:
		{
			addStaff((vector<Staff>*)(Msg.data()));
		}
		default:
			break;
	}
}

void StaffManagementUI::Menu()
{
	Message* action = new Message();
	action->setType(ACTION_MAINMENU);
	m_uiHandler->StartAction(*action);
	delete action;
}

void StaffManagementUI::Logoff()
{
	Message* action = new Message();
	action->setType(ACTION_LOGOFF);
	m_uiHandler->StartAction(*action);
	delete action;
}

void StaffManagementUI::Exit()
{
	Message* action = new Message();
	action->setType(ACTION_EXIT);
	m_uiHandler->StartAction(*action);
	delete action;
}

bool StaffManagementUI::eventFilter(QObject * obj, QEvent * ev)
{	
	if(treeViewStaff == obj) {
		switch(ev->type()) {
			case QEvent::ContextMenu:
			{
				QMenu menu(treeViewStaff);
				menu.addAction(actionStaffDetail);
				menu.addAction(actionDeleteStaff);
				menu.addAction(actionCasher4Client);
				menu.setFont(font);
				menu.exec(dynamic_cast<QContextMenuEvent*>(ev)->globalPos());
				return true;
				break;
			}
		}
	}
	return QMainWindow::eventFilter(obj, ev);
}

bool StaffManagementUI::event(QEvent * ev)
{

	switch(ev->type()) {
		case QEvent::Show:
		{
			if(!started) {
				started = true;
				Message* action = new Message();
				action->setType(ACTION_GETSTAFF);
				m_uiHandler->StartAction(*action);
				delete action;
			}
			break;
		}
		case QEvent::ContextMenu:
		{
			QMenu menu(this);
			menu.addAction(actionMenu);
			menu.addAction(actionLogOff);
			menu.addAction(actionExit);
			menu.setFont(font);
			QPoint pos = dynamic_cast<QContextMenuEvent*>(ev)->globalPos();
			menu.exec(pos);
			break;
		}
	}
	return QMainWindow::event(ev);
}

void StaffManagementUI::addStaff(vector<Staff>* staff) 
{
	vector<Staff>::iterator it = staff->begin();
	while(staff->end() != it) {
		m_stuffDataModel->insertRow(0);
		m_stuffDataModel->setData(m_stuffDataModel->index(0, 0), it->ID());
		m_stuffDataModel->setData(m_stuffDataModel->index(0, 1), QString::fromLocal8Bit(it->Name().c_str()));
		m_stuffDataModel->setData(m_stuffDataModel->index(0, 2), QString::fromLocal8Bit(StaffSex[it->Sex()]));
		m_stuffDataModel->setData(m_stuffDataModel->index(0, 3), QString::fromLocal8Bit(StaffType[it->Type()]));
		m_stuffDataModel->setData(m_stuffDataModel->index(0, 4), QString::fromLocal8Bit(StaffLevel[it->Level()]));
		m_stuffDataModel->setData(m_stuffDataModel->index(0, 5), QString::fromLocal8Bit(it->Descrp().c_str()));
		it++;
	}
}

 void StaffManagementUI::filterRegExpChanged()
 {
     QRegExp::PatternSyntax syntax =
		 QRegExp::PatternSyntax(comboBoxPattern->itemData(comboBoxPattern->currentIndex()).toInt());
     Qt::CaseSensitivity caseSensitivity =
		 checkBoxSearch->isChecked() ? Qt::CaseInsensitive : Qt::CaseSensitive;

	 QRegExp regExp(lineEditKeyword->text(), caseSensitivity, syntax);
     m_sortProxyModel->setFilterRegExp(regExp);
 }

 void StaffManagementUI::filterColumnChanged()
 {
	 m_sortProxyModel->setFilterKeyColumn(comboBoxItem->currentIndex()-1);
 }

 void StaffManagementUI::sortChanged()
 {
     m_sortProxyModel->setSortCaseSensitivity( checkBoxSort->isChecked() ? Qt::CaseInsensitive : Qt::CaseSensitive);
 }

void StaffManagementUI::staffDetail()
{
	QModelIndex currentIndex = treeViewStaff->currentIndex();
	QModelIndex dataIndex = currentIndex.sibling(currentIndex.row(), 0);
	QString id = dataIndex.data().toString();
}

void StaffManagementUI::removeStaff()	
{
	
}

void StaffManagementUI::cash4Client()
{

}


 void StaffManagementUI::SettingFont(QFont& font)
 {
	 font.setBold(true);
	 treeViewStaff->header()->setFont(font);
	 staffGroupBox->setFont(font);
	 optionGroupBox->setFont(font);
	 font.setBold(false);
	 checkBoxSearch->setFont(font);
	 checkBoxSort->setFont(font);
	 comboBoxItem->setFont(font);
	 comboBoxPattern->setFont(font);
	 labelSearchItem->setFont(font);
	 labelSearchPattern->setFont(font);
	 labelKeyword->setFont(font);
	 lineEditKeyword->setFont(font);
	 menu_File->setFont(font);
	 menubar->setFont(font);
	 statusbar->setFont(font);
	 treeViewStaff->setFont(font);

 }

 void StaffManagementUI::setupUi() {
	customCentralWidget = new QWidget(this);

	staffGroupBox = new QGroupBox(QString::fromLocal8Bit("Ա�����"), customCentralWidget);
	treeViewStaff = new QTreeView(staffGroupBox);
	treeViewStaff->setRootIsDecorated(false);
	treeViewStaff->setAlternatingRowColors(true);
	treeViewStaff->setSortingEnabled(true);
	treeViewStaff->setEditTriggers(QAbstractItemView::NoEditTriggers);
	treeViewStaff->setContextMenuPolicy(Qt::DefaultContextMenu);
	treeViewStaff->installEventFilter(this);

	optionGroupBox = new QGroupBox(QString::fromLocal8Bit("�����ʽ"), customCentralWidget);
	comboBoxPattern = new QComboBox(optionGroupBox);
	labelSearchPattern = new QLabel(QString::fromLocal8Bit("ƥ�䷽ʽ(&P)"), optionGroupBox);
	labelSearchPattern->setBuddy(comboBoxPattern);
	comboBoxPattern->addItem(QString::fromLocal8Bit("������ʽ"), QRegExp::RegExp);
	comboBoxPattern->addItem(QString::fromLocal8Bit("ģ��ƥ��"), QRegExp::Wildcard);
	comboBoxPattern->addItem(QString::fromLocal8Bit("��ȷƥ��"), QRegExp::FixedString);

	lineEditKeyword = new QLineEdit(optionGroupBox);
	labelKeyword = new QLabel(QString::fromLocal8Bit("�ؼ���(&K)"), optionGroupBox);
	labelKeyword->setBuddy(lineEditKeyword);

	comboBoxItem = new QComboBox(optionGroupBox);
	labelSearchItem = new QLabel(QString::fromLocal8Bit("������(&I)"), optionGroupBox);
	labelSearchItem->setBuddy(comboBoxItem);
	comboBoxItem->addItem(QString::fromLocal8Bit("ȫ����Ŀ"));
	comboBoxItem->addItem(QString::fromLocal8Bit("Ա�����"));
	comboBoxItem->addItem(QString::fromLocal8Bit("����"));
	comboBoxItem->addItem(QString::fromLocal8Bit("�Ա�"));
	comboBoxItem->addItem(QString::fromLocal8Bit("ְ��"));
	comboBoxItem->addItem(QString::fromLocal8Bit("����"));
	comboBoxItem->addItem(QString::fromLocal8Bit("����"));

	checkBoxSort = new QCheckBox(QString::fromLocal8Bit("������Դ�Сд(&F)"), optionGroupBox);
	checkBoxSearch = new QCheckBox(QString::fromLocal8Bit("�������Դ�Сд(&S)"), optionGroupBox);

	actionExport = new QAction(this);
	actionMenu = new QAction(this);
	actionLogOff = new QAction(this);
	actionExit = new QAction(this);
	actionStaffDetail = new QAction(this); 
	actionDeleteStaff = new QAction(this);
	actionCasher4Client = new QAction(this);

	menubar = new QMenuBar(this);
	menu_File = new QMenu(QString::fromLocal8Bit("�ļ�(&F)"), menubar);
	menu_Display = new QMenu(QString::fromLocal8Bit("����(&S)"), menubar);
	menu_Setting = new QMenu(QString::fromLocal8Bit("��ʾ(&D)"), menubar);
	menu_About = new QMenu(QString::fromLocal8Bit("����(&A)"), menubar);
	setMenuBar(menubar);


	menubar->addAction(menu_File->menuAction());
	menu_File->addSeparator();
	menu_File->addAction(actionExport);
	menu_File->addSeparator();
	menu_File->addAction(actionMenu);
	menu_File->addAction(actionLogOff);
	menu_File->addAction(actionExit);
	actionExport->setText(QString::fromLocal8Bit("����(&E)"));
	actionMenu->setText(QString::fromLocal8Bit("���˵�(&M)"));
	actionLogOff->setText(QString::fromLocal8Bit("ע��(&L)"));
	actionExit->setText(QString::fromLocal8Bit("�˳�(&X)"));
	menu_File->setTitle(QString::fromLocal8Bit("�ļ�(&F)"));
	actionStaffDetail->setText(QString::fromLocal8Bit("���鼰ҵ��(&D)"));
	actionDeleteStaff->setText(QString::fromLocal8Bit("ɾ��Ա��(&R)"));
	actionCasher4Client->setText(QString::fromLocal8Bit("Ϊ�ͻ���ֵ(&C)"));

	menubar->addAction(menu_Display->menuAction());
	menubar->addAction(menu_Setting->menuAction());
	menubar->addAction(menu_About->menuAction());


	statusbar = new QStatusBar(this);
	setStatusBar(statusbar);

	QHBoxLayout *staffLayout = new QHBoxLayout(staffGroupBox);
	staffLayout->addWidget(treeViewStaff);
	staffGroupBox->setLayout(staffLayout);


	QGridLayout *layout = new QGridLayout(optionGroupBox);
	layout->addWidget(labelKeyword, 0, 0);
	layout->addWidget(lineEditKeyword, 0, 1, 1, 2);
	layout->addWidget(labelSearchPattern, 1, 0);
	layout->addWidget(comboBoxPattern, 1, 1, 1, 2);
	layout->addWidget(labelSearchItem, 2, 0);
	layout->addWidget(comboBoxItem, 2, 1, 1, 2);
	layout->addWidget(checkBoxSearch, 3, 0, 1, 2);
	layout->addWidget(checkBoxSort, 3, 2);
	optionGroupBox->setLayout(layout);

	QVBoxLayout *mainLayout = new QVBoxLayout(customCentralWidget);
	mainLayout->addWidget(staffGroupBox);
	mainLayout->addWidget(optionGroupBox);
	customCentralWidget->setLayout(mainLayout);
	setCentralWidget(customCentralWidget);
	setWindowTitle(QString::fromLocal8Bit("Ա������"));

	const QIcon icon = QIcon(QString::fromUtf8(":/icons/Resources/staff.png"));
    setWindowIcon(icon);
	resize(800, 600);
 }
