#ifndef LISTBROWSEREDITOR_H
#define LISTBROWSEREDITOR_H

#include <QWidget>
#include <QMenu>

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

	int GetID() const;
	void SetID(int ID);

	int GetType() const;
	void SetType(int Type);

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

	int GetService() const;
	void SetService(int Service);

	void ClearMenu();
	void SetMenu(QMap<int, QString> map);

	FormCoverArt *GetFormCoverArt() const;



signals:

//	void SigClickCoverArt(QString rawData);
	void SigClickTitle(int type, QString rawData = "");
	void SigMenu(int index, int type);
	void SigMenu(int index, int type, QString menuName);
	void SigMenuAction(QString path, int type, int menuID);

protected:

	bool eventFilter(QObject *object, QEvent *event);

private slots:

	void SlotMenu();
	void SlotMenuAction(QAction* action);

private:

	void ConnectSigToSlot();
	void Initialize();

	FormCoverArt	*m_pFormCoverArt;
	QMenu			*m_Menu;

	int				m_ID;
	int				m_Type;
	QString			m_RawData;

	int				m_Service;

	Ui::ListBrowserEditor *ui;
};

#endif // LISTBROWSEREDITOR_H
