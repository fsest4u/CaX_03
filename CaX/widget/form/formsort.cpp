#include <QMouseEvent>

#include "formsort.h"
#include "ui_formsort.h"

FormSort::FormSort(QWidget *parent) :
	QWidget(parent),
	m_SortMenu(new QMenu(this)),
	m_bIncrease(false),
	ui(new Ui::FormSort)
{
	ui->setupUi(this);

	Initialize();
}

FormSort::~FormSort()
{
	if (m_SortMenu)
	{
		delete m_SortMenu;
		m_SortMenu = nullptr;
	}

	delete ui;
}

void FormSort::ShowSort()
{
	ui->btnSort->show();

}

void FormSort::ShowIncDec()
{
	ui->labelIncDec->show();

}

void FormSort::ShowResize()
{
	ui->labelResize->show();

}

void FormSort::SetSortMenu(QMap<int, QString> list)
{
	QMap<int, QString>::iterator i;
	for (i = list.begin(); i!= list.end(); i++)
	{
		QAction *action = new QAction(i.value(), this);
		action->setData(i.key());
		m_SortMenu->addAction(action);
	}
	connect(m_SortMenu, SIGNAL(triggered(QAction*)), this, SLOT(SlotSortMenu(QAction*)));
}

void FormSort::SetSortMenuTitle(QString title)
{
	ui->btnSort->setText(title);
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

void FormSort::SlotSortMenu(QAction *action)
{
	ui->btnSort->setText(action->text());

	emit SigSort(action->data().toInt());
}

void FormSort::Initialize()
{
	ui->btnSort->hide();
	ui->labelIncDec->hide();
	ui->labelResize->hide();

//	ui->btnSort->installEventFilter(this);
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

	m_SortMenu->setStyleSheet(style);
	ui->btnSort->setMenu(m_SortMenu);

	SetIncrease(m_bIncrease);
}
