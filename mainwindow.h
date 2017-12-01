
#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

////////////////////////////////////////////////////////////////////////////////

/**
 * \class MainWindow
 * \brief This is the main window for the project.
 *
 *	This is the main window class. It contains table view for the results, a few
 * line edit fields for the search and filtering.
 *
 * \var MainWindow::m_pagesCompleted
 * This variable holds the number of pages that were successfully fetched from
 * the riot's api.
 *
 * \var MainWindow::m_maxPages
 * The variable holds the maximum number of pages to be fetched from the server.
 * If the variable is 0 the search will continue untill new search is requested,
 * or untill the program is closed.
 *
 * \var MainWindow::m_statusTextTemplate
 * This is a template string for the status text at the bottom of the window.
 *
 * \var MainWindow::m_resultsModel
 * This is the model used by tha table view.
 *
 * \var MainWindow::m_ui
 * This is the UI class.
 */

////////////////////////////////////////////////////////////////////////////////

#include <QWidget>

////////////////////////////////////////////////////////////////////////////////

namespace Ui {
	class MainWindowClass;
}
class	RedPostViewModel;

////////////////////////////////////////////////////////////////////////////////

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	explicit MainWindow( QWidget *parent = nullptr );
	virtual  ~MainWindow();

private slots:
	void	onRequestCompleted();
	void	onSearchRequested();
	void	onFailedToRetrivePage();
	void	onResultsItemDblClicked( const QModelIndex& index );

private:
	void	updateStatusLine( bool isSearchComplete = false );

private:
	int						m_pagesCompleted;
	int						m_maxPages;

	QString					m_statusTextTemplate;
	RedPostViewModel*		m_resultsModel;
	Ui::MainWindowClass*	m_ui;
};
////////////////////////////////////////////////////////////////////////////////

#endif // __MAIN_WINDOW_H__
