#ifndef FORMPLAY_H
#define FORMPLAY_H

#include <QWidget>

namespace Ui {
class FormPlay;
}

class FormPlay : public QWidget
{
	Q_OBJECT

public:
	explicit FormPlay(QWidget *parent = nullptr);
	~FormPlay();

private:
	Ui::FormPlay *ui;
};

#endif // FORMPLAY_H
