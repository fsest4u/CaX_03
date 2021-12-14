#ifndef TOPWINDOW_H
#define TOPWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QMenu>

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

	void ClearMenu();
	void SetMenu(QMap<int, QString> list);

	void ShowCBSearch(bool show);
//	void ClearCBSearch();
//	void SetCBSearch(QStringList list);
//	QStringList GetCBSearch();

signals:

	void SigMenu();
	void SigMenuAction(int menuID);

	void SigSearchKeyword(QString keyword);

protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotMenu();
	void SlotMenuAction(QAction *action);

private:

	void ConnectSigToSlot();
	void Initialize();

	QList<QString>	m_TitleList;
	QMenu			*m_Menu;

	Ui::TopWindow *ui;
};

#endif // TOPWINDOW_H
