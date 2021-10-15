#include "loading.h"

#include "waitingspinnerwidget.h"

Loading::Loading(QWidget *parent)
	: QWidget(parent)
	, m_Spinner(new WaitingSpinnerWidget(parent, true, true))

{
	// general
	m_Spinner->setRoundness(50.0);
	m_Spinner->setMinimumTrailOpacity(15.0);
	m_Spinner->setTrailFadePercentage(70.0);
	m_Spinner->setNumberOfLines(12);
	m_Spinner->setLineLength(30);
	m_Spinner->setLineWidth(10);
	m_Spinner->setInnerRadius(30);
	m_Spinner->setRevolutionsPerSecond(1);
	m_Spinner->setColor(QColor(52, 174, 214));

	// circle
//	m_Spinner->setRoundness(0);
//	m_Spinner->setMinimumTrailOpacity(15.0);
//	m_Spinner->setTrailFadePercentage(50.0);
//	m_Spinner->setNumberOfLines(48);
//	m_Spinner->setLineLength(10);
//	m_Spinner->setLineWidth(10);
//	m_Spinner->setInnerRadius(70);
//	m_Spinner->setRevolutionsPerSecond(1);
//	m_Spinner->setColor(QColor(100, 100, 100));
}

void Loading::Start()
{
	Stop();

	m_Spinner->start();

}

void Loading::Stop()
{
	m_Spinner->stop();

}
