// $Id$
//
//    File: JEventProcessor_cosmicRayTestDigitalHits.h
// Created: Wed Nov  6 10:26:07 PM EST 2024
// Creator: zhikun (on Linux ifarm2401.jlab.org 5.14.0-427.28.1.el9_4.x86_64 x86_64)
//

#ifndef _JEventProcessor_cosmicRayTestDigitalHits_
#define _JEventProcessor_cosmicRayTestDigitalHits_

#include <JANA/JEventProcessor.h>

class JEventProcessor_cosmicRayTestDigitalHits:public jana::JEventProcessor{
	public:
		JEventProcessor_cosmicRayTestDigitalHits();
		~JEventProcessor_cosmicRayTestDigitalHits();
		const char* className(void){return "JEventProcessor_cosmicRayTestDigitalHits";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.
};

#endif // _JEventProcessor_cosmicRayTestDigitalHits_

