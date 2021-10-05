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

	void SetTitle(QString title);
	void SetSubtitle(QString subtitle);
	void SetInfo(QString info);
	void SetCoverArt(QString coverArt);

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
