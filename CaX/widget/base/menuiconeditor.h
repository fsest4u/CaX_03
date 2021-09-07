#ifndef MENUICONEDITOR_H
#define MENUICONEDITOR_H

#include <QWidget>

namespace Ui {
class MenuIconEditor;
}

class MenuIconEditor : public QWidget
{
	Q_OBJECT

public:
	explicit MenuIconEditor(QWidget *parent = nullptr);
	~MenuIconEditor();

	int GetID();
	void SetID(int nID);

	int GetType() const;
	void SetType(int Type);

	QString GetCoverArt();
	void SetCoverArt(const QString &value);

	QString GetTitle();
	void SetTitle(const QString &value);

	QString GetRawData() const;
	void SetRawData(const QString &RawData);


signals:

	void SigClickCoverArt(int nType, QString rawData = "");
	void SigClickTitle(int nType);

protected:

	bool eventFilter(QObject *object, QEvent *event);

private:

	int		m_ID;
	int		m_Type;
	QString m_CoverArt;
	QString m_RawData;

	Ui::MenuIconEditor *ui;
};

#endif // MENUICONEDITOR_H
