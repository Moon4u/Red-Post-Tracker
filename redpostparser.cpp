
#include "redpostdata.h"

#include "redpostparser.h"

const char* const	titleIdentifier		= "class=\"title-link\"";
const char* const	byLineIdentifier	= "<div class=\"byline\">";
const char* const	timeAgoIdentifier	= "class=\"timeago\" title=\"";
const char* const	hrefIdentifier		= "href=\"";
const char* const	authorIdentifier	= "class=\"author byline\">";
const char* const	postBodyIdentifier	= "<div class=\"body\">";

const int			byLineIdentifierLen	= strlen( byLineIdentifier ) + 2;

////////////////////////////////////////////////////////////////////////////////

/**
 * Constructor.
 */
RedPostParser::RedPostParser( QObject* parent /* = nullptr */ )
	: QObject( parent )
	, m_startPosition( 0 )
	, m_listOfItems( nullptr )
	, m_currentItem( nullptr )
	, m_data()
{
	m_listOfItems = new RedPostsList;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * Destructor.
 */
RedPostParser::~RedPostParser()
{
	delete	m_currentItem;
	delete	m_listOfItems;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * A method that clears and deletes all the data that has been stored so far.
 */
void	RedPostParser::clearData()
{
	for( RedPostsList::iterator it = m_listOfItems->begin();
		 it != m_listOfItems->end();
		 ++it )
		delete	*it;

	m_listOfItems->clear();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * A method that sets the string that is going to be processed.
 *
 * @param data	The string that is going to be processed.
 */
void	RedPostParser::setData( QString&& data )
{
	m_data	= data;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * A method that initiates the processing of the string.
 */
void	RedPostParser::start()
{
	int	startIndex	= m_listOfItems->size();

	for( m_startPosition = m_data.indexOf( titleIdentifier );
		 m_startPosition != -1;
		 m_startPosition = m_data.indexOf( titleIdentifier, m_startPosition + 1 ) )
	{
		if( m_currentItem == nullptr )
			m_currentItem	= new RedPostData;

		if( parseHref() && parseTitle() && parseCategory() && parseDate()
			&& parseAuthor() && parseContent() )
		{
			m_listOfItems->append( m_currentItem );
			m_currentItem = nullptr;
		}
	}

	emit	dataAppended( startIndex );
}
////////////////////////////////////////////////////////////////////////////////

/**
 * A method that returns a list of the processed items.
 */
RedPostsList*	RedPostParser::getItems()
{
	return	m_listOfItems;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * A helper method. It returns the position of a string inside the riot's
 * response string. This method can also return the position at the end of the
 * string instead of at the begining.
 *
 * @param text			The text to be searched for.
 * @param from			The start position of the search.
 * @param atEndOfText	Whether ot return the position at the end of the string
 *						or at the begining.
 * @return				The position of the string.
 */
int	RedPostParser::indexOf( const char* const text, int from, bool atEndOfText /* = false */ )
{
	int	position	= m_data.indexOf( text, from );
	if( atEndOfText )
		position	+= strlen( text );

	return	position;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * A method that parses the link to the post.
 *
 * @return	false if the parsing fails, true otherwise.
 */
bool	RedPostParser::parseHref()
{
	int	hrefStartPos	= indexOf( hrefIdentifier, m_startPosition, true );
	int	hrefEndPos		= indexOf( "\">", hrefStartPos );
	if( hrefStartPos == -1 || hrefEndPos == -1 )
		return	false;

	int	hrefLen	= hrefEndPos - hrefStartPos;
	m_currentItem->setHref( m_data.mid( hrefStartPos, hrefLen ) );

	return	true;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * A method that parses the title of the post.
 *
 * @return	false if the parsing fails, true otherwise.
 */
bool	RedPostParser::parseTitle()
{
	int	titleStartPos	= indexOf( ">", m_startPosition, true );
	int	titleEndPos		= indexOf( "</a>", m_startPosition );

	if( titleEndPos == -1 || titleStartPos == -1 )
		return	false;

	int	titleLen	= titleEndPos - titleStartPos;
	m_currentItem->setTitle( m_data.mid( titleStartPos, titleLen ) );

	return	true;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * A method that parses the author of the post.
 *
 * @return	false if the parsing fails, true otherwise.
 */
bool	RedPostParser::parseAuthor()
{
	int	authorByLineStartPos	= indexOf( authorIdentifier, m_startPosition, true );
	if( authorByLineStartPos == -1 )
		return	false;

	int	authorNameStartPos		= indexOf( ">", authorByLineStartPos, true );
	int	authorNameEndPos		= indexOf( "</a>", authorByLineStartPos );
	if( authorNameStartPos == -1 || authorNameEndPos == -1 )
		return	false;

	int	authorNameLen	= authorNameEndPos - authorNameStartPos;
	m_currentItem->setAuthor( m_data.mid( authorNameStartPos, authorNameLen ) );

	return	true;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * A method that parses the date of the post.
 *
 * @return	false if the parsing fails, true otherwise.
 */
bool	RedPostParser::parseDate()
{
	int	byLinePos		= indexOf( byLineIdentifier, m_startPosition );
	if( byLinePos == -1 )
		return	false;

	int	timeAgoStartPos	= indexOf( timeAgoIdentifier, byLinePos, true );
	if( timeAgoStartPos == -1 )
		return	false;

	int	timeAgoEndPos	= indexOf( ">", timeAgoStartPos );
	if( timeAgoEndPos == -1 )
		return false;

	int	timeAgoLen	= timeAgoEndPos - timeAgoStartPos - 1;
	m_currentItem->setDate( m_data.mid( timeAgoStartPos, timeAgoLen ) );

	return	true;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * A method that parses the category of the post.
 *
 * @return	false if the parsing fails, true otherwise.
 */
bool	RedPostParser::parseCategory()
{
	int	byLinePos	= indexOf( byLineIdentifier, m_startPosition );
	if( byLinePos == -1 )
		return	false;

	int	inStartPos	= indexOf( ">", byLinePos + byLineIdentifierLen, true );
	int	inEndPos	= indexOf( "</a>", byLinePos + byLineIdentifierLen );
	if( inStartPos == -1 || inEndPos == -1 )
		return	false;

	int	inLen	= inEndPos - inStartPos;
	m_currentItem->setCategory( m_data.mid( inStartPos, inLen ) );

	return	true;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * A method that parses the content of the post.
 *
 * @return	false if the parsing fails, true otherwise.
 */
bool	RedPostParser::parseContent()
{
	int	postBodyStartPos	= indexOf( postBodyIdentifier, m_startPosition );
	int	divCount			= 1;
	int	charIndex			= postBodyStartPos + 1;
	while( divCount != 0 )
	{
		QChar	character	= m_data.at( charIndex );

		charIndex++;
		if( character != '<' )
			continue;

		if( m_data.mid( charIndex, 3 ) == "div" )
		{
			divCount++;
			charIndex	+= 2;
			continue;
		}

		if( m_data.mid( charIndex, 4 ) == "/div" )
		{
			divCount--;
			charIndex	+= 3;
			continue;
		}
	}

	m_currentItem->setContent( m_data.mid( postBodyStartPos, charIndex - postBodyStartPos + 1 ) );
	return	true;
}
////////////////////////////////////////////////////////////////////////////////
