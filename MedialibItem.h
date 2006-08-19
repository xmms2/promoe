#ifndef __MEDIALIBITEM_H__
#define __MEDIALIBITEM_H__

#include <QStringList>

class MedialibItem {
	public:
		enum MedialibItemType {
			NoneItem,
			ArtistItem,
			AlbumItem,
			SongItem
		};

		MedialibItem (const QStringList &text = QStringList (), MedialibItem *parent = NULL, MedialibItemType type = NoneItem) {
			m_text = text;
			m_type = type;
			m_parent = parent;
			if (parent) {
				parent->m_childs.append (this);
			}
		};

		bool mayHaveChilds () const {
			if (m_type == NoneItem || m_type == SongItem)
				return false;
			return true;
		};

		QString data (int c) const {
			if (m_text.size () > c) {
				return m_text.at (c);
			}
			return QString ();
		};

		MedialibItem *child (int c) const {
			return m_childs.value (c);
		}

		int childCount () const {
			return m_childs.count ();
		};

		MedialibItem *parent () const {
			return m_parent;
		};

		int row () const {
			if (m_parent)
				return m_parent->m_childs.indexOf (const_cast<MedialibItem*> (this));
			return 0;
		}

	private:
		MedialibItemType m_type;
		QStringList m_text;
		MedialibItem *m_parent;
		QList<MedialibItem *> m_childs;
};

#endif
