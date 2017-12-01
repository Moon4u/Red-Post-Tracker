
#include <utility>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>

#include "redposttrackercore.h"
#include "redpostparser.h"
#include "redpostdata.h"

#include "redtrackersearcher.h"

////////////////////////////////////////////////////////////////////////////////

const char* const	RED_TRACEKER_URL	= "https://boards.na.leagueoflegends.com/en/redtracker";
const char* const	JSON_WRAPPER		= "json_wrap=1";
const char* const	CREATED_TO			= "created_to=";
const char* const	RIOT_DATE_FORMAT	= "yyyy-MM-ddT00%3A44%3A31.543%2B0000";

////////////////////////////////////////////////////////////////////////////////

/**
 * Constructor.
 */
RedTrackerSearcher::RedTrackerSearcher( QObject* parent /* = nullptr */ )
	: QObject( parent ),
	  m_networkAccessMan( nullptr ),
	  m_time()
{
	m_networkAccessMan	= new QNetworkAccessManager( this );
}
////////////////////////////////////////////////////////////////////////////////

/**
 * Destructor.
 */
RedTrackerSearcher::~RedTrackerSearcher()
{
	delete	m_networkAccessMan;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * A method that makes new request to the riot's API.
 */
void	RedTrackerSearcher::requestNewRedTrackerPage( bool fromStart /* = false */ )
{
	if( fromStart )
		m_time.clear();

	QString	url;
	url	+= RED_TRACEKER_URL;
	url	+= "?";
	if( ! m_time.isEmpty() )
	{
		url	+= CREATED_TO;
		url	+= m_time;
		url	+= "&";
	}
	url	+= JSON_WRAPPER;

	QNetworkRequest	request;
	request.setUrl( url );

	QNetworkReply*	reply	= m_networkAccessMan->get( request );
	connect( reply, SIGNAL( finished() ), this, SLOT( onNewRedtrackerPage() ) );
}
////////////////////////////////////////////////////////////////////////////////

/**
 * This method sets new date, used when sending request to the riot's API.
 * This will take effect on the next request.
 *
 * @param dateFilter	The new date.
 */
void	RedTrackerSearcher::setStartDate( QDate&& dateFilter )
{
	m_time	= dateFilter.toString( RIOT_DATE_FORMAT );
}
////////////////////////////////////////////////////////////////////////////////

/**
 * A slot that is called when we get a response from the server.
 */
void	RedTrackerSearcher::onNewRedtrackerPage()
{
	QNetworkReply*	reply	= qobject_cast< QNetworkReply* >( sender() );

	if( 0 == reply )
	{
		emit	failedToRetrivePage();
		return;
	}

	parseJsonReply( reply->readAll() );
	reply->deleteLater();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * A method that partially parses the result. Takes the last created token, and
 * sends the rest to the red post parser.
 *
 * @param reply	The reply to be parsed.
 */
void	RedTrackerSearcher::parseJsonReply( QByteArray&& reply )
{
	QJsonParseError	parsingError;
	QJsonDocument	doc	= QJsonDocument::fromJson( reply, &parsingError );
	if( QJsonParseError::NoError != parsingError.error )
	{
		emit	failedToRetrivePage();
		return;
	}

	QJsonObject	jsonObject	= doc.object();
	m_time					= jsonObject[ "lastCreated" ].toString();

	RedPostParser*	redPostParser	= red_core.getRedPostParser();
	redPostParser->setData( jsonObject[ "items" ].toString() );
	redPostParser->start();

	emit	requestComplete();
}
////////////////////////////////////////////////////////////////////////////////
