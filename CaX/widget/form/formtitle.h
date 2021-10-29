#ifndef FORMTITLE_H
#define FORMTITLE_H

#include <QWidget>

namespace Ui {
class FormTitle;
}

class FormTitle : public QWidget
{
	Q_OBJECT

public:
	explicit FormTitle(QWidget *parent = nullptr);
	~FormTitle();

	QString GetTitle();
	void SetTitle(QString title);
	void SetTitleFont(int size = 14, QString color = "545454");

	QString GetSubtitle();
	void SetSubtitle(QString subtitle);
	void SetSubtitleFont(int size = 14, QString color = "545454");

signals:

	void SigTitle();
	void SigSubtitle();

protected:

	bool eventFilter(QObject *object, QEvent *event);

private:
	Ui::FormTitle *ui;
};

#endif // FORMTITLE_H
