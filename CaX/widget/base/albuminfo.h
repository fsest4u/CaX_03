#ifndef ALBUMINFO_H
#define ALBUMINFO_H

#include <QWidget>

namespace Ui {
class AlbumInfo;
}

class AlbumInfo : public QWidget
{
	Q_OBJECT

public:
	explicit AlbumInfo(QWidget *parent = nullptr);
	~AlbumInfo();

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
	Ui::AlbumInfo *ui;
};

#endif // ALBUMINFO_H
