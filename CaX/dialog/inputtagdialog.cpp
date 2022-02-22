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

void InputTagDialog::SetTagList(const QStringList &tagList, const QString &tag)
{
	m_TagList = tagList;
	if (m_TagList.count() > 0)
	{
		ui->comboBox->addItems(m_TagList);
		int index = 0;
		for (int i = 0; i < m_TagList.count(); i++)
		{
			if (!m_TagList.at(i).compare(tag))
			{
				index = i;
				break;
			}
		}
		ui->comboBox->setCurrentIndex(index);
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
