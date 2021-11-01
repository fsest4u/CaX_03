#include <QFile>

#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include "util/caxconstants.h"
#include "util/log.h"

AboutDialog::AboutDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AboutDialog)
{
	ui->setupUi(this);

	Initialize();
}

AboutDialog::~AboutDialog()
{
	delete ui;
}

void AboutDialog::Initialize()
{
	QString data;
	QString fileName(":/resource/about.html");

	QFile file(fileName);
	if(!file.open(QIODevice::ReadOnly)) {
		LogWarning("file not opened");
	}
	else
	{
//		LogDebug("file opened");
		data = file.readAll();
		data = data.arg(APP_VERSION);
	}

	file.close();

//	LogDebug("data [%s]", data.toUtf8().data());

	ui->textEdit->setReadOnly(true);
	ui->textEdit->setHtml(data);
}
