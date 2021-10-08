#ifndef FORMCOVERART_H
#define FORMCOVERART_H

#include <QWidget>

namespace Ui {
class FormCoverArt;
}

class FormCoverArt : public QWidget
{
	Q_OBJECT

public:
	explicit FormCoverArt(QWidget *parent = nullptr);
	~FormCoverArt();

	QString GetCoverArt();
	void SetCoverArt(QString coverArt);

signals:

	void SigCoverArt();

protected:

	bool eventFilter(QObject *object, QEvent *event);

private:
	Ui::FormCoverArt *ui;
};

#endif // FORMCOVERART_H
