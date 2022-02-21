#include "confirmcoverartdialog.h"
#include "ui_confirmcoverartdialog.h"

ConfirmCoverArtDialog::ConfirmCoverArtDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ConfirmCoverArtDialog)
{
	ui->setupUi(this);
}

ConfirmCoverArtDialog::~ConfirmCoverArtDialog()
{
	delete ui;
}

void ConfirmCoverArtDialog::SetImagePath(QString imagePath)
{
	QString style;
	style = QString("QLabel	\
					{	\
					  border-image: url(\'%1\');	\
					}").arg(imagePath);
	ui->labelCoverArt->setStyleSheet(style);
}

