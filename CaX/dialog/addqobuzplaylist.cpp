#include "addqobuzplaylist.h"
#include "ui_addqobuzplaylist.h"

AddQobuzPlaylist::AddQobuzPlaylist(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddQobuzPlaylist)
{
	ui->setupUi(this);
}

AddQobuzPlaylist::~AddQobuzPlaylist()
{
	delete ui;
}

QString AddQobuzPlaylist::GetName() const
{
	return ui->lineEditName->text();
}

void AddQobuzPlaylist::SetName(const QString name)
{
	ui->lineEditName->setText(name);
}

QString AddQobuzPlaylist::GetDesc() const
{
	return ui->lineEditDesc->text();
}

void AddQobuzPlaylist::SetDesc(const QString desc)
{
	ui->lineEditDesc->setText(desc);
}
