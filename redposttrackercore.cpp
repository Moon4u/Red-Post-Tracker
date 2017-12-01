
#include "redpostparser.h"
#include "redtrackersearcher.h"

#include "redposttrackercore.h"

////////////////////////////////////////////////////////////////////////////////

/**
 * Constructor.
 */
RedPostTrackerCore::RedPostTrackerCore()
{
	m_redTrackerSearcher	= new RedTrackerSearcher;
	m_redPostParser			= new RedPostParser;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * Destructor.
 */
RedPostTrackerCore::~RedPostTrackerCore()
{
	delete	m_redTrackerSearcher;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * Returns the pointer to the red searcher class.
 */
RedTrackerSearcher*	RedPostTrackerCore::getRedSearcher()	const
{
	return	m_redTrackerSearcher;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * Returns the pointer to the red post parser class.
 */
RedPostParser*	RedPostTrackerCore::getRedPostParser()	const
{
	return	m_redPostParser;
}
////////////////////////////////////////////////////////////////////////////////
