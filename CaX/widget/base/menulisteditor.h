#ifndef MENULISTEDITOR_H
#define MENULISTEDITOR_H

#include <QWidget>

namespace Ui {
class MenuListEditor;
}

class MenuListEditor : public QWidget
{
	Q_OBJECT

public:
	explicit MenuListEditor(QWidget *parent = nullptr);
	~MenuListEditor();

	int GetType() const;
	void SetType(int nType);

	QString GetIcon() const;
	void SetIcon(const QString &Icon);

	QString GetTitle() const;
	void SetTitle(const QString &Title);

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

	int			m_nType;
	QString		m_Icon;
	QString		m_Art;
	QString		m_RawData;

	Ui::MenuListEditor *ui;
};

#endif // MENULISTEDITOR_H
