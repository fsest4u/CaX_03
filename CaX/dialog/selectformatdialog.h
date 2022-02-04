#ifndef SELECTFORMATDIALOG_H
#define SELECTFORMATDIALOG_H

#include <QDialog>

namespace Ui {
class SelectFormatDialog;
}


//////////////////////////////////////////////////
// convert format
//////////////////////////////////////////////////

#define FORMAT_CAF				"caf"
#define FORMAT_WAV				"wav"
#define FORMAT_AIFF				"aiff"
#define FORMAT_FLAC				"flac"
#define FORMAT_M4A				"m4a"
#define FORMAT_MP3				"mp3"
#define FORMAT_OGG				"ogg"
#define FORMAT_WMA				"wma"

class SelectFormatDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SelectFormatDialog(QWidget *parent = nullptr);
	~SelectFormatDialog();

	enum
	{
		FORMAT_ID_CAF = 0,
		FORMAT_ID_WAV,
		FORMAT_ID_AIFF,
		FORMAT_ID_FLAC,
		FORMAT_ID_M4A,
		FORMAT_ID_MP3,
		FORMAT_ID_OGG,
		FORMAT_ID_WMA,
		FORMAT_ID_MAX
	};

	QString GetFormat() const;
	void SetFormat(const QString &Format);

private slots:

	void SlotClickFormat();

private:

	QString		m_Format;

	Ui::SelectFormatDialog *ui;
};

#endif // SELECTFORMATDIALOG_H
