
#include <QDesktopServices>
#include <QDateTime>

#include "redpostparser.h"
#include "redposttrackercore.h"
#include "redpostviewmodel.h"
#include "redtrackersearcher.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

////////////////////////////////////////////////////////////////////////////////

const char* const	STATUS_TEXT_TEMPLATE	= "%1 (%2 posts found) pages processed.";
const char* const	STATUS_TEXT_DONE		= "Search complete. %1";

////////////////////////////////////////////////////////////////////////////////

/**
 * Constructor.
 */
MainWindow::MainWindow( QWidget* parent /* = nullptr */ )
	: QWidget( parent )
	, m_pagesCompleted( 0 )
	, m_maxPages( 0 )
	, m_statusTextTemplate( QString( STATUS_TEXT_TEMPLATE ) )
	, m_ui( new Ui::MainWindowClass )
{
	m_ui->setupUi(this);

	m_resultsModel	= new RedPostViewModel( m_ui->m_tvResults );
	m_ui->m_tvResults->setModel( m_resultsModel );

	QHeaderView*	header	= m_ui->m_tvResults->horizontalHeader();
	header->setSectionResizeMode( RedPostViewModel::ID_TITLE,  QHeaderView::Stretch );
	header->setSectionResizeMode( RedPostViewModel::ID_AUTHOR, QHeaderView::ResizeToContents );
	header->setSectionResizeMode( RedPostViewModel::ID_DATE,   QHeaderView::ResizeToContents );

	m_ui->m_lePagesToLookup->setValidator( new QIntValidator( this ) );
	m_ui->m_dteDateFilter->setDate( QDate::currentDate() );

	connect( m_ui->m_tvResults, SIGNAL( doubleClicked( const QModelIndex& ) ), this, SLOT( onResultsItemDblClicked( const QModelIndex& ) ) );
	connect( m_ui->m_btnStartSearch, SIGNAL( pressed() ), this, SLOT( onSearchRequested() ) );
	connect( m_ui->m_leSearchField, SIGNAL( returnPressed() ), this, SLOT( onSearchRequested() ) );
	connect( red_core.getRedSearcher(), SIGNAL( requestComplete() ), this, SLOT( onRequestCompleted() ) );
	connect( red_core.getRedSearcher(), SIGNAL( failedToRetrivePage() ), this, SLOT( onFailedToRetrivePage() ) );
}
////////////////////////////////////////////////////////////////////////////////

/**
 * Destructor.
 */
MainWindow::~MainWindow()
{
	delete	m_ui;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * This is slot that is called when a page has been successfully fetched from
 * riot's APIs.
 */
void	MainWindow::onRequestCompleted()
{
	bool	isSearchComplete	= true;
	if( m_maxPages == 0 || m_pagesCompleted < m_maxPages )
	{
		m_pagesCompleted++;
		red_core.getRedSearcher()->requestNewRedTrackerPage();
		isSearchComplete	= false;
	}

	updateStatusLine( isSearchComplete );
}
////////////////////////////////////////////////////////////////////////////////

/**
 * This is slot that is called when the user requests new search.
 */
void	MainWindow::onSearchRequested()
{
	RedTrackerSearcher*	searcher	= red_core.getRedSearcher();
	QDate				dateFilter	= m_ui->m_dteDateFilter->date();

	red_core.getRedPostParser()->clearData();
	m_resultsModel->clearData();

	QString	pagesFiledText	= m_ui->m_lePagesToLookup->text();
	if( ! pagesFiledText.isEmpty() )
		m_maxPages			= pagesFiledText.toInt();
	m_pagesCompleted	= 0;
	updateStatusLine();

	searcher->setStartDate( std::move( dateFilter ) );
	m_resultsModel->setFilterString( m_ui->m_leSearchField->text() );
	searcher->requestNewRedTrackerPage();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * This is a slot that is called when the fetching of a page failed.
 */
void	MainWindow::onFailedToRetrivePage()
{
	qDebug() << "Failed to retrive a page";
}
////////////////////////////////////////////////////////////////////////////////

/**
 * This is slot that is called when a resul from the table view are double
 * clicked.
 * The link associated with that row will be open in the user's browser.
 *
 * @param index	The index that was double clicked.
 */
void	MainWindow::onResultsItemDblClicked( const QModelIndex& index )
{
	if( index.row() != RedPostViewModel::ID_TITLE )
		return;

	QDesktopServices::openUrl ( index.data( RedPostViewModel::ID_HREF ).toString() );
}
////////////////////////////////////////////////////////////////////////////////

/**
 * This is a method that updates the status line.
 *
 * @param isSearchComplete	Whether the search continues or not.
 */
void	MainWindow::updateStatusLine( bool isSearchComplete /* = false */ )
{
	QString	statusLineText	= m_statusTextTemplate.arg( m_pagesCompleted )
												  .arg( m_resultsModel->itemsSize() );
	if( isSearchComplete )
		statusLineText	= QString( STATUS_TEXT_DONE ).arg( statusLineText );

	m_ui->m_lblStatus->setText( statusLineText );
}
////////////////////////////////////////////////////////////////////////////////
