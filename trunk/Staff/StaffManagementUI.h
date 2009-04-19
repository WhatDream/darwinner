#ifndef STAFFMANAGEMENT_H
#define STAFFMANAGEMENT_H

#define emptyNameWarnning "Ա�������������գ� \n�뷵�ز���дԱ��������"
#define removeStaffConfirm "ȷ��ɾ��ѡ��Ա����%1����\n���Ա����ְ�����������ĸ�Ա��״̬����������ϵͳ��ɾ����Ա����"
#define removeStaffError "δ�ܳɹ�ɾ��ѡ��Ա���������ԡ�"


#include "DUIObserver.h"
class Staff;
#include <QtGui>
class QEvent;
#include <QFont>
#include <list>
#include <map>
#include <string>
class Job;
class Level;
class Status;
class StaffDetail;
class QModelIndex;
using namespace std;

class StaffManagementUI : public QMainWindow, public DUIObserver
{
	Q_OBJECT
public:
	StaffManagementUI();
	~StaffManagementUI();
	void OnEvent(Message& Msg);
	void SettingFont(QFont& font);
	bool event(QEvent * ev);

protected:
	void setupUi();
	virtual bool eventFilter ( QObject * obj, QEvent * ev );
	void staffDetail(uint32 id);
	void getAllStaff();
	void getJobType();
	void getLevelType();
	void getStatusType();
	void removeStaff(uint32 id, string name);

private:
	void addStaff(std::list<Staff> *staffList);
	QMessageBox::StandardButton showMessageBox(QMessageBox::Icon icon, string title, string info = "");

	//ui items
	QAction *actionBrowseMyRecords;
	QAction *actionBrowseMyInfo;
	QAction *actionBrowseMySalary;
	QAction *actionChangeMyPassword;
	QAction *actionStaffConfig;
	QAction *actionAboutQt;
	QAction *actionAboutCosmetic;
	QAction *actionExport;
	QAction *actionMenu;
	QAction *actionLogOff;
	QAction *actionExit;
	QAction *actionStaffDetail;
	QAction *actionDeleteStaff;
	QAction *actionCasher4Client;
	QTreeView *treeViewStaff;
	QComboBox *comboBoxPattern;
	QLabel *labelSearchPattern;
	QLineEdit *lineEditKeyword;
	QLabel *labelKeyword;
	QComboBox *comboBoxItem;
	QLabel *labelSearchItem;
	QCheckBox *checkBoxSort;
	QCheckBox *checkBoxSearch;
	QMenuBar *menubar;
	QMenu *menu_File;
	QMenu *menu_Personal;
	QMenu *menu_Setting;
	QMenu *menu_About;
	QStatusBar *statusbar;
	QGroupBox *staffGroupBox;
	QGroupBox *optionGroupBox;
	QGroupBox *infoGroupBox;
	QWidget *customCentralWidget;
	StaffDetail* staffDetailWidget;
	QPushButton *deletePushButton;
	QPushButton *addPushButton;
	QCheckBox *seletCheckBox;
	QMessageBox * messageBox;
	
	//data items;
	bool started;
	QFont font;
	QAbstractItemModel* m_stuffDataModel;
	QSortFilterProxyModel* m_sortProxyModel;
	map<uint32, string> m_staffLevel;
	map<uint32, string> m_staffType;
	map<uint32, string> m_staffState;

private slots:
	void Exit();
	void Logoff();
	void Menu();
	void addStaff(Staff* staff);
	void modifyStaff(Staff* staff);

	void filterRegExpChanged();
	void filterColumnChanged();
	void sortChanged();
	void staffActivated(const QModelIndex& item);
	void removeStaff();
	void cash4Client();
	void browseMyInfo();
	void changeMyPassword();
	void changePasswrod(string oldpw, string newpw);
};

#endif // STUFFMANAGEMENT_H
