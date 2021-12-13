#include <QMouseEvent>

#include "listsetupeditor.h"
#include "ui_listsetupeditor.h"

ListSetupEditor::ListSetupEditor(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ListSetupEditor)
{
	ui->setupUi(this);

	ConnectSigToSlot();
}

ListSetupEditor::~ListSetupEditor()
{
	delete ui;
}

QString ListSetupEditor::GetStrID() const
{
	return m_StrID;
}

void ListSetupEditor::SetStrID(const QString &StrID)
{
	m_StrID = StrID;
}

QString ListSetupEditor::GetTitle() const
{
	return ui->labelTitle->text();
}

void ListSetupEditor::SetTitle(const QString &title)
{
	ui->labelTitle->setText(title);
}

bool ListSetupEditor::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelTitle)
		{
			emit SigClickTitle(m_StrID);
		}
	}

	return QObject::eventFilter(object, event);
}

void ListSetupEditor::ConnectSigToSlot()
{
	ui->labelTitle->installEventFilter(this);

}
