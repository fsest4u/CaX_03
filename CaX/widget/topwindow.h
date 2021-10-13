#ifndef TOPWINDOW_H
#define TOPWINDOW_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class TopWindow;
}

class TopWindow : public QWidget
{
	Q_OBJECT

public:
	explicit TopWindow(QWidget *parent = nullptr);
	~TopWindow();

	void AddTitle(QString title);
	void RemoveTitle();
	void SetTitle(int index);

	QPushButton* GetBtnMenu();
	QPushButton* GetBtnHome();
	QPushButton* GetBtnPrev();
	QPushButton* GetBtnNext();
	QPushButton* GetBtnSearch();

private:

	QList<QString> m_TitleList;

	Ui::TopWindow *ui;
};

#endif // TOPWINDOW_H
