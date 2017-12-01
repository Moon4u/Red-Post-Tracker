
#ifndef __RED_POST_TRACKER_CORE_H__
#define __RED_POST_TRACKER_CORE_H__

////////////////////////////////////////////////////////////////////////////////

/**
 * \class RedPostTrackerCore
 * \brief This is a global class that holds all other global classes.
 *
 * \var RedPostTrackerCore::m_redPostParser
 * A pointer to the red post parser class.
 *
 * \var RedPostTrackerCore::m_redTrackerSearcher
 * A pointer to the red tracker searcher class.
 */

////////////////////////////////////////////////////////////////////////////////

#define red_core RedPostTrackerCore::getInstance()

////////////////////////////////////////////////////////////////////////////////

class	RedPostParser;
class	RedTrackerSearcher;

////////////////////////////////////////////////////////////////////////////////

class RedPostTrackerCore
{
public:
	static RedPostTrackerCore&	getInstance()
	{
		static RedPostTrackerCore	instance;
		return	instance;
	}

	RedTrackerSearcher*			getRedSearcher()	const;
	RedPostParser*				getRedPostParser()	const;

private:
	explicit	RedPostTrackerCore();
	virtual		~RedPostTrackerCore();

private:
	RedPostParser*		m_redPostParser;
	RedTrackerSearcher*	m_redTrackerSearcher;
};
////////////////////////////////////////////////////////////////////////////////

#endif // __RED_POST_TRACKER_CORE_H__
