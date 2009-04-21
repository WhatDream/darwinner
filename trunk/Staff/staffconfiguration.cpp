#include "staffconfiguration.h"

StaffConfiguration::StaffConfiguration(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	createIcons();
//	connect(ui.pageJob, SIGNAL(submitted(list<Job>*), this, SIGNAL(jobSubmitted(list<Job>*));
//	connect(ui.pageLevel, SIGNAL(submitted(list<Level>*), this, SIGNAL(levelSubmitted(list<Level>*));
}

StaffConfiguration::~StaffConfiguration()
{

}

void StaffConfiguration::createIcons()
 {
     QListWidgetItem *jobButton = new QListWidgetItem(ui.ItemListWidget);
     jobButton->setIcon(QIcon(":/staff/Resources/job.png"));
	jobButton->setText(QString::fromLocal8Bit("ְ������"));
     jobButton->setTextAlignment(Qt::AlignCenter);
     jobButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

     QListWidgetItem *levelButton = new QListWidgetItem(ui.ItemListWidget);
     levelButton->setIcon(QIcon(":/staff/Resources/level.png"));
     levelButton->setText(QString::fromLocal8Bit("�ȼ�����"));
     levelButton->setTextAlignment(Qt::AlignCenter);
     levelButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
/*
     QListWidgetItem *queryButton = new QListWidgetItem(contentsWidget);
     queryButton->setIcon(QIcon(":/images/query.png"));
     queryButton->setText(tr("Query"));
     queryButton->setTextAlignment(Qt::AlignHCenter);
     queryButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
*/
     connect(ui.ItemListWidget,
             SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
             this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
 }

 void StaffConfiguration::changePage(QListWidgetItem *current, QListWidgetItem *previous)
 {
     if (!current)
         current = previous;

     ui.ItemStackedWidget->setCurrentIndex(ui.ItemListWidget->row(current));
 }