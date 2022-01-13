#include "inputtagdialog.h"
#include "ui_inputtagdialog.h"

InputTagDialog::InputTagDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::InputTagDialog)
{
	ui->setupUi(this);
	ui->comboBox->setEditable(true);
}

InputTagDialog::~InputTagDialog()
{
	delete ui;
}

QStringList InputTagDialog::GetTagList() const
{
	return m_TagList;
}

void InputTagDialog::SetTagList(const QStringList &tagList)
{
	m_TagList = tagList;
	if (tagList.count() > 0)
	{
		ui->comboBox->addItems(tagList);
		ui->comboBox->setCurrentIndex(0);
	}
}

QString InputTagDialog::GetTagName() const
{
	return ui->comboBox->currentText();
}

void InputTagDialog::SetTagName(const QString name)
{
	ui->comboBox->setCurrentText(name);
}
