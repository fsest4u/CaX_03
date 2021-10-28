#include <QMouseEvent>

#include "formsort.h"
#include "ui_formsort.h"

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

void FormSort::ShowMenu()
{
	ui->btnMenu->show();

}

void FormSort::ShowIncDec()
{
	ui->labelIncDec->show();

}

void FormSort::ShowResize()
{
	ui->labelResize->show();

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
	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenu(QAction*)));
}

void FormSort::SetMenuTitle(QString title)
{
	ui->btnMenu->setText(title);
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
						  border-image: url(\":/resource/top-btn22-sortu-n@3x.png\");	\
						}	\
						QLabel:hover	\
						{	\
						  border-image: url(\":/resource/top-btn22-sortu-h@3x.png\");	\
						}");

	}
	else
	{
		style = QString("QLabel	\
						{	\
						  border-image: url(\":/resource/top-btn22-sortd-n@3x.png\");	\
						}	\
						QLabel:hover	\
						{	\
						  border-image: url(\":/resource/top-btn22-sortd-h@3x.png\");	\
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
		else if (object == ui->labelResize)
		{
			emit SigResize();
		}
	}

	return QObject::eventFilter(object, event);
}

void FormSort::SlotMenu(QAction *action)
{
	ui->btnMenu->setText(action->text());

	emit SigMenu(action->data().toInt());
}

void FormSort::Initialize()
{
	ui->btnMenu->hide();
	ui->labelIncDec->hide();
	ui->labelResize->hide();

//	ui->btnMenu->installEventFilter(this);
	ui->labelIncDec->installEventFilter(this);
	ui->labelResize->installEventFilter(this);

	QString style = QString("QMenu::item {	\
								width: 200px;	\
								height: 40px;	\
								color: rgb(90, 91, 94);	\
								font-size: 14pt;	\
								padding: 0px 20px 0px 20px;	\
							}	\
							QMenu::item:selected {	\
								background: rgba(201,237,248,255);	\
							}");

	m_Menu->setStyleSheet(style);
	ui->btnMenu->setMenu(m_Menu);

	SetIncrease(m_bIncrease);
}
