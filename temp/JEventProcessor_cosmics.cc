// $Id$
//
//    File: JEventProcessor_cosmics.cc
// Created: Mon Oct 21 03:51:09 PM EDT 2024
// Creator: isomov (on Linux ifarm2402.jlab.org 5.14.0-427.33.1.el9_4.x86_64 x86_64)
//
#include <map>
using namespace std;

#include "JEventProcessor_cosmics.h"
#include <ECAL/DECALHit.h>


using namespace jana;


// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>
extern "C"{
void InitPlugin(JApplication *app){
	InitJANAPlugin(app);
	app->AddProcessor(new JEventProcessor_cosmics());
}
} // "C"


//------------------
// JEventProcessor_cosmics (Constructor)
//------------------
JEventProcessor_cosmics::JEventProcessor_cosmics()
{

}

//------------------
// ~JEventProcessor_cosmics (Destructor)
//------------------
JEventProcessor_cosmics::~JEventProcessor_cosmics()
{

}

//------------------
// init
//------------------
jerror_t JEventProcessor_cosmics::init(void)
{

    gStyle->SetOptStat(0);
  
	gStyle->SetTitleFontSize(0.05);
  
	gStyle->SetTitleFont(130,"xy");
	gStyle->SetLabelFont(130,"xy");
	
	gStyle->SetLineWidth(1.5);
	gStyle->SetTextSize(1.2);
	
	
	gStyle->SetLabelSize(0.04,"xy");
	gStyle->SetTitleSize(0.03,"xy");
	
	
	gStyle->SetTitleOffset(0.5,"x");
	gStyle->SetTitleOffset(0.2,"y");
	
	gStyle->SetTitleBorderSize(0);
	

  
	// This is called once at program startup. 
        app1 =  new TApplication("app1",NULL,0);

	my_canvas  = new TCanvas("ECAL ", "ECAL ",1000, 700,  1500, 1000);
	
        test = new TH1F("iliya","iliya", 100, -0.5, 99.5);
	hen   = new TH1F("energy ","energy", 100, 0., 100.);
	plot = new TH2F("Plot","Plot",40,-0.5,39.5,40,-0.5,39.5);
	plot->GetXaxis()->SetTitle("Column");
	plot->GetYaxis()->SetTitle("Row");
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t JEventProcessor_cosmics::brun(JEventLoop *eventLoop, int32_t runnumber)
{
	// This is called whenever the run number changes
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t JEventProcessor_cosmics::evnt(JEventLoop *loop, uint64_t eventnumber)
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
  
        vector<const DECALHit *> ecal_hits;
	
        loop->Get(ecal_hits);

       	cout << " Iliya test " << ecal_hits.size() << endl;

	japp->RootFillLock(this);

	for(unsigned int ii = 0; ii < ecal_hits.size(); ii++){
	  const DECALHit *ecal_hit = ecal_hits[ii];
	  double en = ecal_hit->E*1000.;
	  int row = ecal_hit->row;
	  int c1 = ecal_hit->column;
	  hen->Fill(en);
	  cout << " " << ii <<  " Row = " << row <<  " Column = " << c1 << endl;
	  
	  plot->Fill((double)c1,(double)row,en);
	  //	  cout << "  " << ii <<  " Iliya 1 = " << en << endl;
	}

	my_canvas->cd();
	gPad->SetGrid(1);
	plot->Draw("zcol");
	
	my_canvas->Update();

	getchar();
	
	my_canvas->Clear();
	plot->Reset();


	test->Fill(float(ecal_hits.size()));
	
	japp->RootFillUnLock(this);
	
	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t JEventProcessor_cosmics::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t JEventProcessor_cosmics::fini(void)
{
	// Called before program exit after event processing is finished.
	return NOERROR;
}

