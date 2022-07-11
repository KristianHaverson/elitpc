
#include "TFile.h"
#include "HistoManager.h"
#include "EventSourceROOT.h"
#include <boost/property_tree/json_parser.hpp>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "TTree.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TString.h"
#include "TStopwatch.h"
#include <boost/program_options.hpp>
#include "IonRangeCalculator.h"
#include "dEdxFitter.h"
#include "TrackBuilder.h"
#include "HistoManager.h"
#include "EventSourceROOT.h"
#ifdef WITH_GET
#include "EventSourceGRAW.h"
#include "EventSourceMultiGRAW.h"
#endif
#include "SigClusterTPC.h"
#include "EventTPC.h"
#include "RecoOutput.h"
#include "RunIdParser.h"
#include "InputFileHelper.h"
#include "MakeUniqueName.h"
#include "colorText.h"




///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////// 

std::string createROOTFileName(const  std::string & grawFileName){

	std::string rootFileName = grawFileName;
	std::string::size_type index = rootFileName.find(",");
	if(index!=std::string::npos){
		rootFileName = grawFileName.substr(0,index);
	}
	index = rootFileName.rfind("/");
	if(index!=std::string::npos){
		rootFileName = rootFileName.substr(index+1,-1);
	}
	if(rootFileName.find("CoBo_ALL_AsAd_ALL")!=std::string::npos){
		rootFileName = rootFileName.replace(0,std::string("CoBo_ALL_AsAd_ALL").size(),"TrackTree");
	}
	else if(rootFileName.find("CoBo0_AsAd")!=std::string::npos){
		rootFileName = rootFileName.replace(0,std::string("CoBo0_AsAd").size()+1,"TrackTree");
	}
	else if(rootFileName.find("EventTPC")!=std::string::npos){
		rootFileName = rootFileName.replace(0,std::string("EventTPC").size(),"TrackTree");
	}
	else{
		std::cout<<KRED<<"File format unknown: "<<RST<<rootFileName<<std::endl;
		exit(1);
	}
	index = rootFileName.rfind("graw");
	if(index!=std::string::npos){
		rootFileName = rootFileName.replace(index,-1,"root");
	}

	return rootFileName;
}

/////////////////////////////////////
/////////////////////////////////////
int makeTrackTree(const  std::string & geometryFileName, 
		  const  std::string & dataFileName);
/////////////////////////////////////
/////////////////////////////////////

int main(int argc, char *argv[]) {

	// **** Input names **** //				   
	std::string grawFilePath =  "/home/kris/programs/TPCReco/";	
	std::string graw1 = "CoBo0_AsAd0_2022-04-12T08:03:44.531_0000.graw";
	std::string graw2 = "CoBo0_AsAd1_2022-04-12T08:03:44.533_0000.graw";
	std::string graw3 = "CoBo0_AsAd2_2022-04-12T08:03:44.536_0000.graw";
	std::string graw4 = "CoBo0_AsAd3_2022-04-12T08:03:44.540_0000.graw";
	
	std::string geometryFilePath =  "/home/kris/programs/TPCReco/scratch/TPCReco/TPCReco-src/resources/";
	std::string geometryFileName = "geometry_ELITPC_190mbar_3332Vdrift_25MHz.dat";
	
	
	
	
	std::string dataFile = grawFilePath+graw1+","+grawFilePath+graw2+","+grawFilePath+graw3+","+grawFilePath+graw4;
	std::string geometryFile = geometryFilePath+geometryFileName;		   
	


	// **** Make the TTree **** //	     
	makeTrackTree(geometryFile, dataFile);


	return 0;
}


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

int makeTrackTree(const  std::string & geometryFileName, 
		  const  std::string & dataFileName){

	std::shared_ptr<EventSourceBase> myEventSource;

	// **** Read in input graw input file **** //
	if(dataFileName.find(".graw")!=std::string::npos){
#ifdef WITH_GET
		if(dataFileName.find(",")!=std::string::npos){
			myEventSource = std::make_shared<EventSourceMultiGRAW>(geometryFileName);
		}else{
			myEventSource = std::make_shared<EventSourceGRAW>(geometryFileName);
			dynamic_cast<EventSourceGRAW*>(myEventSource.get())->setFrameLoadRange(160);
		}
#endif
	}else{
		std::cout<<KRED<<"Wrong input file: "<<RST<<dataFileName<<std::endl;
		return -1;
	}


	// **** Histogram options **** //
	HistoManager myHistoManager;
	myHistoManager.setGeometry(myEventSource->getGeometry());
	//myHistoManager.toggleAutozoom();


		// **** Create output root file name **** //
		std::string recoFileName =  createROOTFileName(dataFileName);// "root1.root";



	// ????? //	  
	std::shared_ptr<eventraw::EventInfo> myEventInfo = std::make_shared<eventraw::EventInfo>();

	// **** Set outputs **** //
	RecoOutput myRecoOutput;
	myRecoOutput.setEventInfo(myEventInfo);
	myRecoOutput.open(recoFileName);
	myEventSource->loadDataFile(dataFileName);
	std::cout<<KBLU<<"File with "<<RST<<myEventSource->numberOfEntries()<<" frames loaded."<<std::endl;

	// **** Event loop **** //
	unsigned int nEntries = myEventSource->numberOfEntries();
	//nEntries = 20;
	
	for(unsigned int iEntry=0;iEntry<(nEntries);++iEntry){
		if(nEntries>10 && iEntry%(nEntries/10)==0){
			std::cout<<KBLU<<"Processed: "<<int(100*(double)iEntry/nEntries)<<" % events"<<RST<<std::endl;
		}
		myEventSource->loadFileEntry(iEntry);
		std::cout<<"EventID: "<<myEventSource->currentEventNumber()<<std::endl;
		myHistoManager.setEvent(myEventSource->getCurrentEvent());
		for(int strip_dir=0;strip_dir<3;++strip_dir){
		
			myHistoManager.getRawStripVsTimeInMM(strip_dir)->Write();
			//myHistoManager.getRawStripVsTime(strip_dir)->Write();
		}


		int eventId = myEventSource->getCurrentEvent()->GetEventId();

		myEventInfo->SetEventId(eventId);
		myRecoOutput.setEventInfo(myEventInfo);				   
		myRecoOutput.update(); 


	}
	myRecoOutput.close();

	return nEntries;
}

