#ifndef INFOTRACKS_H
#define INFOTRACKS_H

#include <QWidget>

namespace Ui {
class InfoTracks;
}

class InfoTracks : public QWidget
{
	Q_OBJECT

public:
	explicit InfoTracks(QWidget *parent = nullptr);
	~InfoTracks();

	void SetTitle(const QString title);
	void SetSubtitle(const QString subtitle);
	void SetInfo(const QString info);

protected:

	bool eventFilter(QObject *object, QEvent *event);

signals:

	void SigPlayAll();
	void SigPlayRandom();
	void SigSubmenu();
	void SigSort();

private:
	Ui::InfoTracks *ui;
};

#endif // INFOTRACKS_H
