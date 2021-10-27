#include "addplaylistdialog.h"
#include "ui_addplaylistdialog.h"

AddPlaylistDialog::AddPlaylistDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddPlaylistDialog)
{
	ui->setupUi(this);
}

AddPlaylistDialog::~AddPlaylistDialog()
{
	delete ui;
}

QString AddPlaylistDialog::GetName()
{
	return ui->lineEdit->text();
}

void AddPlaylistDialog::SetName(QString name)
{
	ui->lineEdit->setText(name);
}
