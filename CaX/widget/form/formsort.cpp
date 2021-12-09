#include <QMouseEvent>
#include <QWidgetAction>

#include "formsort.h"
#include "ui_formsort.h"

#include "util/caxconstants.h"
#include "util/log.h"

FormSort::FormSort(QWidget *parent) :
	QWidget(parent),
	m_Menu(new QMenu(this)),
	m_ResizeMenu(new QMenu(this)),
	m_Slider(new QSlider(this)),
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

	if (m_ResizeMenu)
	{
		delete m_ResizeMenu;
		m_ResizeMenu = nullptr;
	}

	if (m_Slider)
	{
		delete m_Slider;
		m_Slider = nullptr;
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
	m_Slider->setValue(resize);
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

void FormSort::SlotResizeSliderValueChanged(int value)
{
	emit SigResize(value);
}

void FormSort::SlotResizeSliderReleased()
{
	int value = m_Slider->value();
	emit SigResize(value);
}

void FormSort::Initialize()
{
	ui->btnMenu->hide();
	ui->labelIncDec->hide();
	ui->btnResize->hide();

	ui->labelIncDec->installEventFilter(this);

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

	connect(m_Menu, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenu(QAction*)));

	SetIncrease(m_bIncrease);
	SetResizeMenu();

}

void FormSort::SetResizeMenu()
{
	m_Slider->setOrientation(Qt::Horizontal);
	m_Slider->setMinimum(LIST_HEIGHT_MIN);
	m_Slider->setMaximum(ICON_HEIGHT_MAX);
	m_Slider->setGeometry( 0, 0, 340, 22 );

	QWidgetAction *action = new QWidgetAction(this);
	action->setDefaultWidget(m_Slider);
	m_ResizeMenu->addAction(action);

	ui->btnResize->setMenu(m_ResizeMenu);

	connect(m_Slider, SIGNAL(valueChanged(int)), this, SLOT(SlotResizeSliderValueChanged(int)));
	connect(m_Slider, SIGNAL(sliderReleased()), this, SLOT(SlotResizeSliderReleased()));
}
