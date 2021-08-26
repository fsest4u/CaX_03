#include "menuinfo.h"
#include "ui_menuinfo.h"

MenuInfo::MenuInfo(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MenuInfo)
{
	ui->setupUi(this);
}

MenuInfo::~MenuInfo()
{
	delete ui;
}
