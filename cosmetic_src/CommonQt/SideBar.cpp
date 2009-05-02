#include "SideBar.h"

SideBar::SideBar(QWidget *parent)
	: QWidget(parent)
{
	widgetList.clear();
	ui.setupUi(this);
}

SideBar::~SideBar()
{

}

void SideBar::addWidget(QWidget *widget, int x, int y, int row, int column)
{
	ui.gridLayout->addWidget(widget, x, y, row, column);
	widgetList.push_back(widget);
}

void SideBar::hideWidget(bool hide)
{
	if(hide) {
		ui.hideButton->setText("<");
		ui.hideButton->setStatusTip(QString::fromLocal8Bit("չ�������"));
		ui.hideButton->setToolTip(QString::fromLocal8Bit("չ�������"));
		for(list<QWidget*>::iterator it = widgetList.begin() ; widgetList.end() != it ; it++)
			(*it)->close();
	} else {
		ui.hideButton->setText(">");
		ui.hideButton->setStatusTip(QString::fromLocal8Bit("��������"));
		ui.hideButton->setToolTip(QString::fromLocal8Bit("��������"));
		for(list<QWidget*>::iterator it = widgetList.begin() ; widgetList.end() != it ; it++)
			(*it)->show();
	}
}
