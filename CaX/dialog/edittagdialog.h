#ifndef EDITTAGDIALOG_H
#define EDITTAGDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

#include "util/CJsonNode.h"

namespace Ui {
class EditTagDialog;
}

class EditTagDialog : public QDialog
{
	Q_OBJECT

public:
	explicit EditTagDialog(QWidget *parent = nullptr);
	~EditTagDialog();

	void SetNodeList(QList<CJsonNode> list);

private:

	enum {
		EDIT_TAG_TITLE = 0,
		EDIT_TAG_FAVORITE,
		EDIT_TAG_ARTIST,
		EDIT_TAG_ALBUM,
		EDIT_TAG_GENRE,
		EDIT_TAG_ALBUM_ARTIST,
		EDIT_TAG_COMPOSER,
		EDIT_TAG_YEAR,
		EDIT_TAG_MOOD,
		EDIT_TAG_RATING,
		EDIT_TAG_MAX
	};

	QStandardItemModel		*m_Model;

	Ui::EditTagDialog *ui;
};

#endif // EDITTAGDIALOG_H
