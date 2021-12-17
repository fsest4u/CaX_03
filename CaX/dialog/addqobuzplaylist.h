#ifndef ADDQOBUZPLAYLIST_H
#define ADDQOBUZPLAYLIST_H

#include <QDialog>

namespace Ui {
class AddQobuzPlaylist;
}

class AddQobuzPlaylist : public QDialog
{
	Q_OBJECT

public:
	explicit AddQobuzPlaylist(QWidget *parent = nullptr);
	~AddQobuzPlaylist();

	QString GetName() const;
	void SetName(const QString name);

	QString GetDesc() const;
	void SetDesc(const QString desc);

private:
	Ui::AddQobuzPlaylist *ui;
};

#endif // ADDQOBUZPLAYLIST_H
