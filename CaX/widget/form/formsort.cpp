#include "formsort.h"
#include "ui_formsort.h"

FormSort::FormSort(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FormSort)
{
	ui->setupUi(this);
}

FormSort::~FormSort()
{
	delete ui;
}
