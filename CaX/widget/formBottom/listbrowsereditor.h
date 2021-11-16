#ifndef LISTBROWSEREDITOR_H
#define LISTBROWSEREDITOR_H

#include <QWidget>

class FormCoverArt;

namespace Ui {
class ListBrowserEditor;
}

class ListBrowserEditor : public QWidget
{
	Q_OBJECT

public:
	explicit ListBrowserEditor(QWidget *parent = nullptr);
	~ListBrowserEditor();

	QString GetID() const;
	void SetID(QString strID);

	int GetType() const;
	void SetType(int nType);

	QString GetTitle() const;
	void SetTitle(const QString &title);

	// for browser - begin
	QString GetSubtitle() const;
	void SetSubtitle(const QString &subtitle);

	QString GetDuration() const;
	void SetDuration(const QString &duration);

	QString GetFilesize() const;
	void SetFileSize(const QString &filesize);
	// for browser - end

	QString GetRawData() const;
	void SetRawData(const QString &rawData);

	FormCoverArt *GetFormCoverArt() const;



signals:

//	void SigClickCoverArt(QString rawData);
	void SigClickTitle(int nType, QString rawData = "");

protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:



private:

	void ConnectSigToSlot();

	FormCoverArt	*m_pFormCoverArt;

	QString			m_StrID;
	int				m_nType;
	QString			m_RawData;

	Ui::ListBrowserEditor *ui;
};

#endif // LISTBROWSEREDITOR_H
