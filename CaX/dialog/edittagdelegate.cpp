#include <QMouseEvent>
#include <QComboBox>
#include <QLineEdit>
#include <QPainter>

#include "edittagdelegate.h"
#include "edittagdialog.h"

#include "util/log.h"

EditTagDelegate::EditTagDelegate() :
	m_BtnFavorite(new QPushButton)
{
	m_AlbumList.clear();
	m_AlbumArtistList.clear();
	m_ArtistList.clear();
	m_GenreList.clear();
	m_ComposerList.clear();
	m_MoodList.clear();
}

EditTagDelegate::~EditTagDelegate()
{
	if (m_BtnFavorite)
	{
		delete m_BtnFavorite;
		m_BtnFavorite = nullptr;
	}
}


QStringList EditTagDelegate::GetMoodList() const
{
	return m_MoodList;
}

void EditTagDelegate::SetMoodList(const QStringList &MoodList)
{
	m_MoodList = MoodList;
}

QStringList EditTagDelegate::GetComposerList() const
{
	return m_ComposerList;
}

void EditTagDelegate::SetComposerList(const QStringList &ComposerList)
{
	m_ComposerList = ComposerList;
}

QStringList EditTagDelegate::GetGenreList() const
{
	return m_GenreList;
}

void EditTagDelegate::SetGenreList(const QStringList &GenreList)
{
	m_GenreList = GenreList;
}

QStringList EditTagDelegate::GetArtistList() const
{
	return m_ArtistList;
}

void EditTagDelegate::SetArtistList(const QStringList &ArtistList)
{
	m_ArtistList = ArtistList;
}

QStringList EditTagDelegate::GetAlbumArtistList() const
{
	return m_AlbumArtistList;
}

void EditTagDelegate::SetAlbumArtistList(const QStringList &AlbumArtistList)
{
	m_AlbumArtistList = AlbumArtistList;
}

QStringList EditTagDelegate::GetAlbumList() const
{
	return m_AlbumList;
}

void EditTagDelegate::SetAlbumList(const QStringList &AlbumList)
{
	m_AlbumList = AlbumList;
}


void EditTagDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() == EditTagDialog::EDIT_TAG_FAVORITE)
	{
		QStyleOptionButton button;
		button.rect  = QRect(option.rect.x(),	// + (option.rect.width() - 15) / 2,
							 option.rect.y() + (option.rect.height() - 26) / 2,
							 15,
							 26);
//		button.text = "test";
		button.features = QStyleOptionButton::Flat;

		QString style;
		if (index.data().toString().toInt() == 0)
		{
			style = QString("	\
							QPushButton	\
							{	\
								border-image: url(\":/resource/play-btn12-like-n.png\");	\
							}");
		}
		else
		{
			style = QString("	\
							QPushButton	\
							{	\
								border-image: url(\":/resource/play-btn12-like-u.png\");	\
							}");
		}
		m_BtnFavorite->setStyleSheet(style);
		m_BtnFavorite->style()->drawControl(QStyle::CE_PushButton, &button, painter, m_BtnFavorite);

	}
	else if (index.column() == EditTagDialog::EDIT_TAG_ARTIST
			 || index.column() == EditTagDialog::EDIT_TAG_ALBUM
			 || index.column() == EditTagDialog::EDIT_TAG_GENRE
			 || index.column() == EditTagDialog::EDIT_TAG_ALBUM_ARTIST
			 || index.column() == EditTagDialog::EDIT_TAG_COMPOSER
			 || index.column() == EditTagDialog::EDIT_TAG_MOOD
			 || index.column() == EditTagDialog::EDIT_TAG_YEAR)
	{
		QItemDelegate::paint(painter, option, index);
		painter->save();

		QString title;
		if (index.column() == EditTagDialog::EDIT_TAG_ARTIST)
		{
			title = qvariant_cast<QString>(index.data(EditTagDialog::EDIT_TAG_ARTIST));
		}
		else if (index.column() == EditTagDialog::EDIT_TAG_ALBUM)
		{
			title = qvariant_cast<QString>(index.data(EditTagDialog::EDIT_TAG_ALBUM));
		}
		else if (index.column() == EditTagDialog::EDIT_TAG_GENRE)
		{
			title = qvariant_cast<QString>(index.data(EditTagDialog::EDIT_TAG_GENRE));
		}
		else if (index.column() == EditTagDialog::EDIT_TAG_ALBUM_ARTIST)
		{
			title = qvariant_cast<QString>(index.data(EditTagDialog::EDIT_TAG_ALBUM_ARTIST));
		}
		else if (index.column() == EditTagDialog::EDIT_TAG_COMPOSER)
		{
			title = qvariant_cast<QString>(index.data(EditTagDialog::EDIT_TAG_COMPOSER));
		}
		else if (index.column() == EditTagDialog::EDIT_TAG_MOOD)
		{
			title = qvariant_cast<QString>(index.data(EditTagDialog::EDIT_TAG_MOOD));
		}
		else if (index.column() == EditTagDialog::EDIT_TAG_YEAR)
		{
			title = qvariant_cast<QString>(index.data(EditTagDialog::EDIT_TAG_YEAR));
		}

		QFont fontTitle("Segoe UI", 9, QFont::Normal, false);
		QFontMetrics fmTitle(fontTitle);

		QRect rectTitle = option.rect;

//		painter->drawRect(rectYear);

		painter->setPen(QColor(84, 84, 84));
		if (!title.isEmpty())
		{
			painter->setFont(fontTitle);
			painter->drawText(rectTitle, title);
		}

		painter->restore();
	}
}

QWidget *EditTagDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() == EditTagDialog::EDIT_TAG_ARTIST
		 || index.column() == EditTagDialog::EDIT_TAG_ALBUM
		 || index.column() == EditTagDialog::EDIT_TAG_GENRE
		 || index.column() == EditTagDialog::EDIT_TAG_ALBUM_ARTIST
		 || index.column() == EditTagDialog::EDIT_TAG_COMPOSER
		 || index.column() == EditTagDialog::EDIT_TAG_MOOD
		 || index.column() == EditTagDialog::EDIT_TAG_YEAR)
	{
		QStringList tagList;
		QString tag;
		if (index.column() == EditTagDialog::EDIT_TAG_ARTIST)
		{
			tagList = m_ArtistList;
			tag = qvariant_cast<QString>(index.data(EditTagDialog::EDIT_TAG_ARTIST));
		}
		else if (index.column() == EditTagDialog::EDIT_TAG_ALBUM)
		{
			tagList = m_AlbumList;
			tag = qvariant_cast<QString>(index.data(EditTagDialog::EDIT_TAG_ALBUM));
		}
		else if (index.column() == EditTagDialog::EDIT_TAG_GENRE)
		{
			tagList = m_GenreList;
			tag = qvariant_cast<QString>(index.data(EditTagDialog::EDIT_TAG_GENRE));
		}
		else if (index.column() == EditTagDialog::EDIT_TAG_ALBUM_ARTIST)
		{
			tagList = m_AlbumArtistList;
			tag = qvariant_cast<QString>(index.data(EditTagDialog::EDIT_TAG_ALBUM_ARTIST));
		}
		else if (index.column() == EditTagDialog::EDIT_TAG_COMPOSER)
		{
			tagList = m_ComposerList;
			tag = qvariant_cast<QString>(index.data(EditTagDialog::EDIT_TAG_COMPOSER));
		}
		else if (index.column() == EditTagDialog::EDIT_TAG_MOOD)
		{
			tagList = m_MoodList;
			tag = qvariant_cast<QString>(index.data(EditTagDialog::EDIT_TAG_MOOD));
		}


		QComboBox *comboBox = new QComboBox(parent);
		comboBox->addItems(tagList);
		int index = 0;
		for (int i = 0; i < tagList.count(); i++)
		{
			if (!tagList.at(i).compare(tag))
			{
				index = i;
				break;
			}
		}
		comboBox->setEditable(true);
		comboBox->setCompleter(nullptr);
		comboBox->setCurrentIndex(index);
		return comboBox;
	}
	else if (index.column() == EditTagDialog::EDIT_TAG_YEAR)
	{
		QLineEdit *lineEdit = new QLineEdit(parent);
		// Set validator
		QIntValidator *validator = new QIntValidator(1900, 2100, lineEdit);
		lineEdit->setValidator(validator);
		return lineEdit;
	}

	return nullptr;
}

bool EditTagDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	if (event->type() == QMouseEvent::MouseButtonPress)
	{
		if (((QMouseEvent*)event)->button() == Qt::LeftButton)
		{
			if (index.column() == EditTagDialog::EDIT_TAG_FAVORITE)
			{
				emit SigClickFavorite(index);
			}
		}
	}

	return QItemDelegate::editorEvent(event, model, option, index);
}

