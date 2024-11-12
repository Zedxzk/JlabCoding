// $Id$
//
//    File: JEventProcessor_cosmics.h
// Created: Mon Oct 21 03:51:09 PM EDT 2024
// Creator: isomov (on Linux ifarm2402.jlab.org 5.14.0-427.33.1.el9_4.x86_64 x86_64)
//

#ifndef _JEventProcessor_cosmics_
#define _JEventProcessor_cosmics_

#include <JANA/JEventProcessor.h>
#include "TH1F.h"
#include "TH2F.h"


#include <TDirectory.h>
#include "TApplication.h"
#include "TCanvas.h"
#include <RootHeader.h>

class JEventProcessor_cosmics:public jana::JEventProcessor{
	public:
		JEventProcessor_cosmics();
		~JEventProcessor_cosmics();
		const char* className(void){return "JEventProcessor_cosmics";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.
  TApplication *app1;
  TCanvas *my_canvas;
  TH1F *test;
  TH1F *hen;
  TH2F *plot;
  

  
  
};

#endif // _JEventProcessor_cosmics_

