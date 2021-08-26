#ifndef LOADING_H
#define LOADING_H

#include <QWidget>
#include <QLabel>

class WaitingSpinnerWidget;

class Loading : public QWidget
{
	Q_OBJECT
public:
	explicit Loading(QWidget *parent = 0);

	void Start();
	void Stop();

signals:

private:

	WaitingSpinnerWidget* m_Spinner;

};

#endif // LOADING_H
