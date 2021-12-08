#ifndef FORMTITLE_H
#define FORMTITLE_H

#include <QWidget>

#include "util/caxconstants.h"

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
	void SetTitleFont(int size = FONT_SIZE_ICON_TITLE, QString color = FONT_COLOR_NORMAL, QString weight = FONT_WEIGHT_NORMAL);

	QString GetSubtitle();
	void SetSubtitle(QString subtitle);
	void SetSubtitleFont(int size = FONT_SIZE_ICON_SUBTITLE, QString color = FONT_COLOR_NORMAL, QString weight = FONT_WEIGHT_NORMAL);

signals:

	void SigTitle();
	void SigSubtitle();

protected:

	bool eventFilter(QObject *object, QEvent *event);

private:
	Ui::FormTitle *ui;
};

#endif // FORMTITLE_H
