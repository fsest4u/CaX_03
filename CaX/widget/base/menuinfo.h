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

	void SetTitle(const QString title);

protected:

	bool eventFilter(QObject *object, QEvent *event);

signals:

	void SigPlayAll();
	void SigPlayRandom();
	void SigSubmenu();
	void SigSort();

private:
	Ui::MenuInfo *ui;
};

#endif // MENUINFO_H
