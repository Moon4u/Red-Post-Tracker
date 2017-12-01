
#ifndef __RED_POST_DATA_H__
#define __RED_POST_DATA_H__

////////////////////////////////////////////////////////////////////////////////

/**
 * \class RedPostData
 * \brief A class that holds information about each post.
 *
 * \var RedPostData::m_date
 * The date of the post.
 *
 * \var RedPostData::m_href
 * The link to the post.
 *
 * \var RedPostData::m_title
 * The title of the post.
 *
 * \var RedPostData::m_author
 * The author of the post.
 *
 * \var RedPostData::m_content
 * The content of the post.
 *
 * \var RedPostData::m_category
 * The category of the post.
 */

////////////////////////////////////////////////////////////////////////////////

#include <QDebug>
#include <QVector>
#include <QString>
#include <QDateTime>

////////////////////////////////////////////////////////////////////////////////

const char* const	DATE_TIME_FORMAT	= "yyyy'-'MM'-'dd'T'hh':'mm':'ss'.'z";
const char* const	BASE_URL			= "https://boards.na.leagueoflegends.com/";

////////////////////////////////////////////////////////////////////////////////

class	RedPostData
{
public:
	RedPostData()	{}
	~RedPostData()	{}

	void	setDate( const QString& date )		{ m_date		= QDateTime::fromString( date.left( date.indexOf( '+' ) ), DATE_TIME_FORMAT ); }
	void	setHref( QString&& href )			{ m_href		= QString( BASE_URL ) + href; }
	void	setTitle( QString&& title )			{ m_title		= title; }
	void	setAuthor( QString&& author )		{ m_author		= author; }
	void	setContent( QString&& content )		{ m_content		= content; }
	void	setCategory( QString&& category )	{ m_category	= category; }

	QString			getDate()		const { return	m_date.toString( Qt::TextDate ); }
	const QString&	getHref()		const { return	m_href; }
	const QString&	getTitle()		const { return	m_title; }
	const QString&	getAuthor()		const { return	m_author; }
	const QString&	getContent()	const { return	m_content; }
	const QString&	getCategory()	const { return	m_category; }

private:
	QDateTime	m_date;
	QString		m_href;
	QString		m_title;
	QString		m_author;
	QString		m_content;
	QString		m_category;
};
////////////////////////////////////////////////////////////////////////////////

using	RedPostsList	= QVector< RedPostData* >;

////////////////////////////////////////////////////////////////////////////////

#endif // __RED_POST_DATA_H__
