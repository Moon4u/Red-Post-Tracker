
#include "redpostparser.h"
#include "redposttrackercore.h"

#include "redpostviewmodel.h"

////////////////////////////////////////////////////////////////////////////////

const char* const	HEADER_TITLE_TEXT	= "Post Title";
const char* const	HEADER_AUTHOR_TEXT	= "Post Author";
const char* const	HEADER_DATE_TEXT	= "Post Date";

////////////////////////////////////////////////////////////////////////////////

/**
 * Constructor.
 */
RedPostViewModel::RedPostViewModel( QObject* parent /* = nullptr */ )
	: QAbstractTableModel( parent )
	, m_data( new RedPostsList )
{
	m_headerTitles[ ID_TITLE ]	= HEADER_TITLE_TEXT;
	m_headerTitles[ ID_AUTHOR ]	= HEADER_AUTHOR_TEXT;
	m_headerTitles[ ID_DATE ]	= HEADER_DATE_TEXT;

	connect( red_core.getRedPostParser(), SIGNAL( dataAppended( int ) ), this, SLOT( onDataAppended( int ) ) );
}
////////////////////////////////////////////////////////////////////////////////

/**
 * Destructor.
 */
RedPostViewModel::~RedPostViewModel()
{
	for( RedPostsList::iterator it = m_data->begin();
		 it != m_data->end();
		 ++it )
	{
		delete	*it;
	}

	delete	m_data;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * This method cleares the data of the model.
 */
void	RedPostViewModel::clearData()
{
	beginResetModel();
	m_data->clear();
	endResetModel();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * This method returns the number of items that the model has.
 */
int	RedPostViewModel::itemsSize()
{
	return	m_data->size();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * This is a setter for filter string.
 */
void	RedPostViewModel::setFilterString( const QString& filterString )
{
	m_filterString	= filterString;
	reconstructData();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * This is setter for filter string (using rrefs).
 */
void	RedPostViewModel::setFilterString( QString&& filterString )
{
	m_filterString	= filterString;
	reconstructData();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * This is a reimplemented method. It returns the count of the rows.
 */
int	RedPostViewModel::rowCount( const QModelIndex& )	const
{
	if( m_data == nullptr )
		return	0;

	return	m_data->size();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * This is reimplemented method. It returns the count of the columns.
 */
int	RedPostViewModel::columnCount( const QModelIndex& )	const
{
	return	TOTAL_TABLE_COLUMNS;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * This is reimplemented method. It returns information about an item on a given
 * row.
 */
QVariant	RedPostViewModel::data( const QModelIndex& index, int role ) const
{
	RedPostData*	item	= m_data->at( index.row() );
	switch( role )
	{
		case	Qt::DisplayRole:
		{
			switch( index.column() )
			{
				case	ID_TITLE:
					return	item->getTitle();

				case	ID_AUTHOR:
					return	item->getAuthor();

				case	ID_DATE:
					return	item->getDate();

				default:
					break;
			}
			break;
		}

		case	ID_HREF:
			return	item->getHref();

		default:
			break;
	}

	return	QVariant();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * This is reimplemented method. It returns information about a given header.
 */
QVariant	RedPostViewModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if( orientation == Qt::Vertical )
		return	QAbstractTableModel::headerData( section, orientation, role );

	if( role != Qt::DisplayRole )
		return	QAbstractTableModel::headerData( section, orientation, role );

	switch( section )
	{
		case	ID_TITLE:
		case	ID_AUTHOR:
		case	ID_DATE:
			return	m_headerTitles[ section ];

		default:
			break;
	}

	return	QAbstractTableModel::headerData( section, orientation, role );
}
////////////////////////////////////////////////////////////////////////////////

/**
 * This is slot that is called when new data is added to the parsed items.
 *
 * @param fromIndex The index of the new data point.
 */
void	RedPostViewModel::onDataAppended( int fromIndex )
{
	beginResetModel();
	reconstructData( fromIndex );
	endResetModel();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * Reconstructs the data items based on the items in the parser class and the
 * filter string.
 *
 * @param fromIndex	The index of the new items (if 0 - starts from the begining).
 */
void	RedPostViewModel::reconstructData( int fromIndex /* = 0 */ )
{
	bool	isFilterStringEmpty	= m_filterString.isEmpty() ;
	if( fromIndex == 0 || isFilterStringEmpty )
		m_data->clear();

	RedPostsList*	itemsList	= red_core.getRedPostParser()->getItems();
	if( isFilterStringEmpty )
	{
		m_data->append( *itemsList );
		return;
	}

	for( RedPostsList::iterator it = itemsList->begin() + fromIndex;
		 it != itemsList->end();
		 ++it )
	{
		RedPostData*	currentItem	= *it;

		if(  currentItem->getTitle().contains( m_filterString, Qt::CaseInsensitive ) ||
			currentItem->getContent().contains( m_filterString, Qt::CaseInsensitive ) )
			m_data->append( currentItem );
	}
}
////////////////////////////////////////////////////////////////////////////////
