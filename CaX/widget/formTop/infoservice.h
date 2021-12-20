#ifndef INFOSERVICE_H
#define INFOSERVICE_H

#include <QWidget>

//class FormTitle;
class FormPlay;
class FormSort;

namespace Ui {
class InfoService;
}

class InfoService : public QWidget
{
	Q_OBJECT

public:
	explicit InfoService(QWidget *parent = nullptr);
	~InfoService();

	void SetTitle(const QString title);
//	void SetSubtitle(const QString title);

	FormPlay *GetFormPlay();
	FormSort *GetFormSort();

protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotTitle();
	void SlotSubtitle();

private:

	void ConnectSigToSlot();

//	FormTitle		*m_pFormTitle;
	FormPlay		*m_pFormPlay;
	FormSort		*m_pFormSort;

	Ui::InfoService *ui;
};

#endif // INFOSERVICE_H
