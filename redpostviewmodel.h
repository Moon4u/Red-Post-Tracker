
#ifndef __RED_POST_VIEW_MODEL_H__
#define __RED_POST_VIEW_MODEL_H__

////////////////////////////////////////////////////////////////////////////////

/**
 * \class RedPostViewModel
 * \brief This is the model used for the results view.
 *
 *	This model fetches tha data from the RedPostParser, whenever it is notified
 * that new data has been parsed. These parsed items are filter by a string.
 *
 * \enum USER_DATA_IDS
 * A user data ID used when looking for the link to an item.
 *
 * \enum DISPLAY_IDS
 * An ID for each row used when the Qt::DisplayRole is required.
 *
 * \var RedPostViewModel::m_headerTitles
 * This variable holds the titles to all headers for the model.
 *
 * \var RedPostViewModel::m_filterString
 * This is the string that is going to be used when filtering the items.
 *
 * \var RedPostViewModel::m_data
 * This is the data that is used by the model.
 */

////////////////////////////////////////////////////////////////////////////////

#include "redpostdata.h"

#include <QAbstractTableModel>

////////////////////////////////////////////////////////////////////////////////

class RedPostViewModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum	USER_DATA_IDS
	{
		ID_HREF = Qt::UserRole
	};

	enum	DISPLAY_IDS
	{
		ID_TITLE=0,
		ID_AUTHOR,
		ID_DATE,

		TOTAL_TABLE_COLUMNS
	};

public:
	explicit	RedPostViewModel( QObject* parent = nullptr );
	virtual		~RedPostViewModel();

	void		clearData();
	int			itemsSize();

	void		setFilterString( const QString& filterString );
	void		setFilterString( QString&& filterString );

	int			rowCount( const QModelIndex& )		const override;
	int			columnCount( const QModelIndex& )	const override;

	QVariant	data( const QModelIndex& index, int role )							const override;
	QVariant	headerData( int section, Qt::Orientation orientation, int role )	const override;

private slots:
	void	onDataAppended( int fromIndex );

private:
	void	reconstructData( int fromIndex = 0 );

private:
	QMap< int, QString >	m_headerTitles;
	QString					m_filterString;
	RedPostsList*			m_data;
};
////////////////////////////////////////////////////////////////////////////////

#endif // __RED_POST_VIEW_MODEL_H__
