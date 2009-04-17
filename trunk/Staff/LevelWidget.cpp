#include "LevelWidget.h"
#include <QFont>

LevelWidget::LevelWidget(QWidget *parent, int mode)
	: QWidget(parent)
{
	ui.setupUi(this);
	QFont font = QFont("SimSun", 9);
	settingFont(font);
	int col = 0;
	m_mode = mode;

	if(0 == mode) {
		ui.submitPushButton->setDisabled(true);
	} 

	m_DataModel = new QStandardItemModel(0, 4, this);
	ui.levelTableView->setModel(m_DataModel);
	ui.levelTableView->setAlternatingRowColors(true);
	ui.levelTableView->hideColumn(0);
	ui.levelTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

	m_DataModel->setHeaderData(col, Qt::Horizontal, QString::fromLocal8Bit("���"));
	ui.levelTableView->setColumnWidth(col++, 40);
	m_DataModel->setHeaderData(col, Qt::Horizontal, QString::fromLocal8Bit("�ȼ�"));
	ui.levelTableView->setColumnWidth(col++, 60);
	m_DataModel->setHeaderData(col, Qt::Horizontal, QString::fromLocal8Bit("���(%)"));
	ui.levelTableView->setColumnWidth(col++, 65);
	m_DataModel->setHeaderData(col++, Qt::Horizontal, QString::fromLocal8Bit("��ע"));


	connect(ui.addPushButton, SIGNAL(clicked()), this, SLOT(add()));
	connect(ui.removePushButton, SIGNAL(clicked()), this, SLOT(remove()));
	connect(ui.submitPushButton, SIGNAL(clicked()), this, SLOT(submit()));
	
}

LevelWidget::~LevelWidget()
{

}

void LevelWidget::settingFont(QFont& font)
{
	font.setBold(true);
	ui.levelTableView->horizontalHeader()->setFont(font);
	ui.levelTableView->verticalHeader()->setFont(font);
	font.setBold(false);
	ui.addPushButton->setFont(font);
	ui.removePushButton->setFont(font);
	ui.submitPushButton->setFont(font);
	ui.levelTableView->setFont(font);
}

void LevelWidget::add()
{
	int row = m_DataModel->rowCount();
	m_DataModel->insertRow(row);
	ui.levelTableView->setRowHeight(row, 20);

	m_DataModel->setData(m_DataModel->index(row, 0), 0);
	m_DataModel->setData(m_DataModel->index(row, 1), "");
	m_DataModel->setData(m_DataModel->index(row, 2), 0);
	m_DataModel->setData(m_DataModel->index(row, 3), "");

}
void LevelWidget::remove()
{
	int row = ui.levelTableView->currentIndex().row();
	m_DataModel->removeRow(row);
}
void LevelWidget::submit()
{
}

list<Level>* LevelWidget::getLevelList()
{	list<Level>* levelList = new list<Level>;
	for (int row=0; row<m_DataModel->rowCount(); row++)
	{	
		Level tempLevel;
		int col=0;
		QModelIndex data;
		data = m_DataModel->index(row, col++);
		tempLevel.setId(data.data().toUInt());
		data = m_DataModel->index(row, col++);
		if(data.data().toString().isEmpty()) continue;// ignore job with level name
		tempLevel.setName(data.data().toString().toLocal8Bit().data());
		data = m_DataModel->index(row, col++);
		tempLevel.setProfit(data.data().toUInt());
		data = m_DataModel->index(row, col++);
		tempLevel.setDescription(data.data().toString().toLocal8Bit().data());
		levelList->push_back(tempLevel);
	}
	return levelList;
}

