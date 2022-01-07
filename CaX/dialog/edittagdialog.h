#ifndef EDITTAGDIALOG_H
#define EDITTAGDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

class EditTagDelegate;

namespace Ui {
class EditTagDialog;
}

class EditTagDialog : public QDialog
{
	Q_OBJECT

public:
	explicit EditTagDialog(QWidget *parent = nullptr);
	~EditTagDialog();

	enum {
		EDIT_TAG_ID = 0,
		EDIT_TAG_TITLE,
		EDIT_TAG_FAVORITE,
		EDIT_TAG_ARTIST,
		EDIT_TAG_ALBUM,
		EDIT_TAG_GENRE,
		EDIT_TAG_ALBUM_ARTIST,
		EDIT_TAG_COMPOSER,
		EDIT_TAG_YEAR,
		EDIT_TAG_MOOD,
//		EDIT_TAG_RATING,
		EDIT_TAG_MAX
	};

	void SetNodeList(QList<CJsonNode> list);

	QStandardItemModel *GetModel() const;
	QMap<QStringList, QString> GetUpdateCell() const;

private slots:

	void SlotDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
	void SlotSectionClicked(int logicalIndex);
	void SlotChangeFavorite(const QModelIndex &index);

private:

	void ConnectSigToSlot();
	void Initialize();

	EditTagDelegate			*m_Delegate;
	QStandardItemModel		*m_Model;

	QMap<QStringList, QString> m_MapUpdateCell;

	bool					m_EnableChange;

	Ui::EditTagDialog *ui;
};

#endif // EDITTAGDIALOG_H
