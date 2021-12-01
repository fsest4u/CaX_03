#ifndef ICONCOVERART_H
#define ICONCOVERART_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

class IconCoverArtDelegate;
class Loading;

namespace Ui {
class IconCoverArt;
}

class IconCoverArt : public QWidget
{
	Q_OBJECT

public:
	explicit IconCoverArt(QWidget *parent = nullptr);
	~IconCoverArt();

	enum {
		ICON_COVER_ART_MUSIC_DB = 0,
		ICON_COVER_ART_BROWSER,
		ICON_COVER_ART_MAX
	};

	QList<CJsonNode> GetNodeList() const;
	void SetNodeList(QList<CJsonNode> &list, int type);
	void ClearNodeList();

	QStandardItemModel *GetModel();
	IconCoverArtDelegate *GetDelegate();

signals:

	void SigSearchCoverArt(int index, QString url);
	void SigAppendIconList();


private slots:

	void SlotScrollValueChanged(int value);

private:

	void Initialize();

	QListView				*m_ListView;
	QStandardItemModel		*m_Model;
	IconCoverArtDelegate	*m_Delegate;
	QScrollBar				*m_ScrollBar;

//	Loading					*m_pLoading;

	QList<CJsonNode>		m_NodeList;

	Ui::IconCoverArt *ui;
};

#endif // ICONCOVERART_H
