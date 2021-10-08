#include "formlabel.h"
#include "ui_formlabel.h"

FormLabel::FormLabel(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FormLabel)
{
	ui->setupUi(this);
}

FormLabel::~FormLabel()
{
	delete ui;
}
