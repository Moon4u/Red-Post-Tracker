
#ifndef __RED_POST_PARSER_H__
#define __RED_POST_PARSER_H__

////////////////////////////////////////////////////////////////////////////////

/**
 * \class RedPostParser
 * \brief This is the class that handels the parsing of the response from RIOT's
 * APIs.
 *
 * The riot's response comes as a single string containing all posts. We go
 * through the string and look for a known tags and identifiers.
 *
 * \var RedPostParser::m_startPosition
 * This variable holds the current processing position of the string.
 *
 * \var RedPostParser::m_listOfItems
 * This is a list of items that have been found in the riot's response.
 *
 * \var RedPostParser::m_currentItem
 * This is the item that is currently being updated.
 *
 * \var RedPostParser::m_data
 * This is the string that has been recieved.
 */

////////////////////////////////////////////////////////////////////////////////

#include <QObject>
#include <QString>

////////////////////////////////////////////////////////////////////////////////

class	RedPostData;
using	RedPostsList	= QVector< RedPostData* >;

////////////////////////////////////////////////////////////////////////////////

class RedPostParser : public QObject
{
	Q_OBJECT

public:
	explicit	RedPostParser( QObject* parent = nullptr );
	virtual		~RedPostParser();

	void			clearData();
	void			setData( QString&& data );
	void			start();
	RedPostsList*	getItems();

signals:
	void	dataAppended( int startIndexOfNewData );

private:
	int		indexOf( const char* const text, int from, bool atEndOfText = false );

	bool	parseHref();
	bool	parseTitle();
	bool	parseAuthor();
	bool	parseDate();
	bool	parseCategory();
	bool	parseContent();

private:
	int				m_startPosition;

	RedPostsList*	m_listOfItems;
	RedPostData*	m_currentItem;
	QString			m_data;
};
////////////////////////////////////////////////////////////////////////////////

#endif // __RED_POST_PARSER_H__
