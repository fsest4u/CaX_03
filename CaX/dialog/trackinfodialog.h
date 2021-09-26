#ifndef TRACKINFODIALOG_H
#define TRACKINFODIALOG_H

#include <QDialog>

namespace Ui {
class TrackInfoDialog;
}

class TrackInfoDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TrackInfoDialog(QWidget *parent = nullptr);
	~TrackInfoDialog();

	void SetTitle(const QString &value);
	void SetAlbum(const QString &value);
	void SetArtist(const QString &value);
	void SetGenre(const QString &value);
	void SetAlbumArtist(const QString &value);
	void SetComposer(const QString &value);
	void SetMood(const QString &value);
	void SetTempo(const QString &value);
	void SetTrack(const QString &value);
	void SetYear(const QString &value);
	void SetCDYear(const QString &value);
	void SetCDNumber(const QString &value);

private:
	Ui::TrackInfoDialog *ui;
};

#endif // TRACKINFODIALOG_H
