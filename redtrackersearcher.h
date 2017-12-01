
#ifndef __RED_TRACKER_SEARCHER_H__
#define __RED_TRACKER_SEARCHER_H__

////////////////////////////////////////////////////////////////////////////////

/**
 * \class RedTrackerSearcher
 * \brief A class that does the networking.
 *
 * This class is responsible for sending a request and recieving response from
 * the riot's API.
 *
 * \var RedTrackerSearcher::m_networkAccessMan
 * Network access manager for sending & recieving requests & responses.
 *
 * \var RedTrackerSearcher::m_time
 * The time string that is used when composing new request.
 */

////////////////////////////////////////////////////////////////////////////////

#include <QDateTime>
#include <QNetworkRequest>

////////////////////////////////////////////////////////////////////////////////

class	QNetworkAccessManager;

////////////////////////////////////////////////////////////////////////////////

class RedTrackerSearcher : public QObject
{
	Q_OBJECT

public:
	explicit	RedTrackerSearcher( QObject* parent = nullptr );
	virtual		~RedTrackerSearcher();

	void		requestNewRedTrackerPage( bool fromStart = false );
	void		setStartDate( QDate&& dateFilter );

signals:
	void		failedToRetrivePage();
	void		requestComplete();

private slots:
	void		onNewRedtrackerPage();

private:
	void		parseJsonReply( QByteArray&& reply );

private:
	QNetworkAccessManager*	m_networkAccessMan;
	QString					m_time;
};
////////////////////////////////////////////////////////////////////////////////

#endif // __RED_TRACKER_SEARCHER_H__
