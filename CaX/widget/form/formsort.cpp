#include <QMouseEvent>
#include <QWidgetAction>

#include "formsort.h"
#include "ui_formsort.h"

#include "dialog/resizedialog.h"

#include "util/caxconstants.h"
#include "util/log.h"

FormSort::FormSort(QWidget *parent) :
	QWidget(parent),
	m_Menu(new QMenu(this)),
	m_bIncrease(false),
	ui(new Ui::FormSort)
{
	ui->setupUi(this);

	Initialize();
}

FormSort::~FormSort()
{
	if (m_Menu)
	{
		delete m_Menu;
		m_Menu = nullptr;
	}

	delete ui;
}

void FormSort::ShowMenu(bool show)
{
	if (show)
	{
		ui->btnMenu->show();
	}
	else
	{
		ui->btnMenu->hide();
	}
}

void FormSort::ShowIncDec(bool show)
{
	if (show)
	{
		ui->labelIncDec->show();
	}
	else
	{
		ui->labelIncDec->hide();
	}
}

void FormSort::ShowResize(bool show)
{
	if (show)
	{
		ui->btnResize->show();
	}
	else
	{
		ui->btnResize->hide();
	}
}

void FormSort::ClearMenu()
{
	m_Menu->clear();
}

void FormSort::SetMenu(QMap<int, QString> list)
{
	QMap<int, QString>::iterator i;
	for (i = list.begin(); i!= list.end(); i++)
	{
		QAction *action = new QAction(i.value(), this);
		action->setData(i.key());
		m_Menu->addAction(action);
	}
}

void FormSort::SetMenuTitle(QString title)
{
	ui->btnMenu->setText(title);
}

void FormSort::SetResize(int resize)
{
	m_ResizeValue = resize;
}

void FormSort::SetSliderMinimum(int value)
{
	m_ResizeMin = value;
}

bool FormSort::GetIncrease() const
{
	return m_bIncrease;
}

void FormSort::SetIncrease(bool bIncrease)
{
	m_bIncrease = bIncrease;

	QString style;

	if (bIncrease)
	{
		style = QString("QLabel	\
						{	\
						  border-image: url(\":/resource/top-btn22-sortu-n.png\");	\
						}	\
						QLabel:hover	\
						{	\
						  border-image: url(\":/resource/top-btn22-sortu-h.png\");	\
						}");

	}
	else
	{
		style = QString("QLabel	\
						{	\
						  border-image: url(\":/resource/top-btn22-sortd-n.png\");	\
						}	\
						QLabel:hover	\
						{	\
						  border-image: url(\":/resource/top-btn22-sortd-h.png\");	\
						}");

	}
	ui->labelIncDec->setStyleSheet(style);
}

bool FormSort::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (object == ui->labelIncDec)
		{
			bool bIncrease = !m_bIncrease;
			SetIncrease(bIncrease);
			emit SigIncDec(bIncrease);
		}

	}

	return QObject::eventFilter(object, event);
}

void FormSort::SlotMenu(QAction *action)
{
	ui->btnMenu->setText(action->text());

	emit SigMenu(action->data().toInt());
}

void FormSort::SlotBtnResize()
{
//	LogDebug("resize [%d][%d][%d][%d]", ui->btnResize->geometry().x(), ui->btnResize->geometry().y(), ui->btnResize->geometry().width(), ui->btnResize->geometry().height());
	QPoint point = ui->btnResize->mapToGlobal(ui->btnResize->rect().bottomLeft());
	ResizeDialog dialog;
	dialog.setGeometry(point.x() - dialog.width(), point.y(), dialog.width(), dialog.height());
	dialog.SetSliderValue(m_ResizeMin, ICON_HEIGHT_MAX, m_ResizeValue);
	connect(&dialog, SIGNAL(SigSliderValueChanged(int)), this, SLOT(SlotResizeSliderValueChanged(int)));

	if (dialog.exec() == QDialog::Accepted)
	{

	}
}

void FormSort::SlotResizeSliderValueChanged(int value)
{
	m_ResizeValue = value;
	emit SigResize(m_ResizeValue);
}

void FormSort::Initialize()
{
	ui->btnMenu->hide();
	ui->labelIncDec->hide();
	ui->btnResize->hide();

	ui->labelIncDec->installEventFilter(this);

	QString style = QString("QMenu::item {	\
								width: 300px;	\
								height: 40px;	\
								color: rgb(90, 91, 94);	\
								font-size: 12pt;	\
								padding: 0px 20px 0px 20px;	\
							}	\
							QMenu::item:selected {	\
								background: rgba(201,237,248,255);	\
							}");

	m_Menu->setStyleSheet(style);
	ui->btnMenu->setMenu(m_Menu);

	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenu(QAction*)));
	connect(ui->btnResize, SIGNAL(clicked()), this, SLOT(SlotBtnResize()));

	SetIncrease(m_bIncrease);

}


