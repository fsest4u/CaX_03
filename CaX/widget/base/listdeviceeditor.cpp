#include <QMouseEvent>

#include "listdeviceeditor.h"
#include "ui_listdeviceeditor.h"

ListDeviceEditor::ListDeviceEditor(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ListDeviceEditor)
{
	ui->setupUi(this);

	ui->labelPlus->installEventFilter(this);
	ui->labelDevice->installEventFilter(this);
	ui->labelCancel->installEventFilter(this);

}

ListDeviceEditor::~ListDeviceEditor()
{
	delete ui;
}

QString ListDeviceEditor::GetMac() const
{
    return m_Mac;
}

void ListDeviceEditor::SetMac(const QString &Mac)
{
    m_Mac = Mac;
}

QString ListDeviceEditor::GetAddr() const
{
    return m_Addr;
}

void ListDeviceEditor::SetAddr(const QString &Addr)
{
    m_Addr = Addr;
}

QString ListDeviceEditor::GetVal() const
{
    return m_Val;
}

void ListDeviceEditor::SetVal(const QString &Val)
{
    m_Val = Val;
	ui->labelDevice->setText(Val);
}

QString ListDeviceEditor::GetDev() const
{
    return m_Dev;
}

void ListDeviceEditor::SetDev(const QString &Dev)
{
	m_Dev = Dev;
}

QFrame *ListDeviceEditor::GetFrameCancel()
{
	return ui->frameCancel;
}

bool ListDeviceEditor::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelPlus)
		{
			// nothing
		}
		else if (object == ui->labelDevice)
		{
			emit SigClickDevice(m_Mac);
		}
		else if (object == ui->labelCancel)
		{
			emit SigClickCancel(m_Mac);
		}
	}

	return QObject::eventFilter(object, event);
}

