#ifndef LISTSERVICEEDITOR_H
#define LISTSERVICEEDITOR_H

#include <QWidget>

namespace Ui {
class ListServiceEditor;
}

class ListServiceEditor : public QWidget
{
	Q_OBJECT

public:
	explicit ListServiceEditor(QWidget *parent = nullptr);
	~ListServiceEditor();

	QString GetID() const;
	void SetID(QString strID);

	int GetType() const;
	void SetType(int nType);

	QString GetIcon() const;
	void SetIcon(const QString &Icon);

	QString GetTitle() const;
	void SetTitle(const QString &Title);

	QString GetBottom() const;
	void SetBottom(const QString &Bottom);

	QString GetDuration() const;
	void SetDuration(const QString &Duration);

	QString GetArt() const;
	void SetArt(const QString &Art);

	QString GetRawData() const;
	void SetRawData(const QString &RawData);



signals:

	void SigClickIcon(QString rawData);
	void SigClickTitle(QString rawData);

protected:

	bool eventFilter(QObject *object, QEvent *event);

private:

	QString			m_StrID;
	int			m_nType;
	QString		m_Icon;
	QString		m_Art;
	QString		m_RawData;

	Ui::ListServiceEditor *ui;
};

#endif // LISTSERVICEEDITOR_H
