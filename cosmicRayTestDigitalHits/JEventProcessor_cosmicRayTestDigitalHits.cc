// $Id$
//
//    File: JEventProcessor_cosmicRayTestDigitalHits.cc
// Created: Wed Nov  6 10:26:07 PM EST 2024
// Creator: zhikun (on Linux ifarm2401.jlab.org 5.14.0-427.28.1.el9_4.x86_64 x86_64)
//

#include "JEventProcessor_cosmicRayTestDigitalHits.h"
using namespace jana;


// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>
extern "C"{
void InitPlugin(JApplication *app){
	InitJANAPlugin(app);
	app->AddProcessor(new JEventProcessor_cosmicRayTestDigitalHits());
}
} // "C"


//------------------
// JEventProcessor_cosmicRayTestDigitalHits (Constructor)
//------------------
JEventProcessor_cosmicRayTestDigitalHits::JEventProcessor_cosmicRayTestDigitalHits()
{

}

//------------------
// ~JEventProcessor_cosmicRayTestDigitalHits (Destructor)
//------------------
JEventProcessor_cosmicRayTestDigitalHits::~JEventProcessor_cosmicRayTestDigitalHits()
{

}

//------------------
// init
//------------------
jerror_t JEventProcessor_cosmicRayTestDigitalHits::init(void)
{
	// This is called once at program startup. 

	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t JEventProcessor_cosmicRayTestDigitalHits::brun(JEventLoop *eventLoop, int32_t runnumber)
{
	// This is called whenever the run number changes
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t JEventProcessor_cosmicRayTestDigitalHits::evnt(JEventLoop *loop, uint64_t eventnumber)
{
	// This is called for every event. Use of common resources like writing
	// to a file or filling a histogram should be mutex protected. Using
	// loop->Get(...) to get reconstructed objects (and thereby activating the
	// reconstruction algorithm) should be done outside of any mutex lock
	// since multiple threads may call this method at the same time.
	// Here's an example:
	//
	// vector<const MyDataClass*> mydataclasses;
	// loop->Get(mydataclasses);
	//
	// japp->RootFillLock(this);
	//  ... fill historgrams or trees ...
	// japp->RootFillUnLock(this);


	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t JEventProcessor_cosmicRayTestDigitalHits::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t JEventProcessor_cosmicRayTestDigitalHits::fini(void)
{
	// Called before program exit after event processing is finished.
	return NOERROR;
}

