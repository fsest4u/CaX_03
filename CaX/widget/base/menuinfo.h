#ifndef MENUINFO_H
#define MENUINFO_H

#include <QWidget>

namespace Ui {
class MenuInfo;
}

class MenuInfo : public QWidget
{
	Q_OBJECT

public:
	explicit MenuInfo(QWidget *parent = nullptr);
	~MenuInfo();

private:
	Ui::MenuInfo *ui;
};

#endif // MENUINFO_H
