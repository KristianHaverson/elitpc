
#include "TH2Analysis.cpp"
#include "structures.hpp"
#include "draw.hpp"
#include "Ransac.cpp"
#include <tuple>
using namespace std;


// --- Define pretty colo(u)rs --- //
#define RESET   "\033[0m"
#define BLACK   "\033[30m"    /* Black */
#define RED     "\033[31m"    /* Red */
#define GREEN   "\033[32m"    /* Green */
#define YELLOW  "\033[33m"    /* Yellow */
#define BLUE    "\033[34m"    /* Blue */
#define MAGENTA "\033[35m"    /* Magenta */
#define CYAN    "\033[36m"    /* Cyan */
#define WHITE   "\033[37m"    /* White */
#define BOLDBLACK   "\033[1m\033[30m"       /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"       /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"       /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"       /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"       /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"       /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"       /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"       /* Bold White */



int TH2Viewer(){


	/////////////////////////////////////////////////////////////////////////////
	// ** Switches ** //

	int TimeOrMm = 1;  // 0 = time, 1 = mm 
	string inputDirecTime ="/home/kris/Desktop/TrackTreeTime.root";
	string inputDirecMm  ="/home/kris/Desktop/TrackTree_2022-04-12T08:03:44.531_0000.root";
	int nEntries = 2674; // <---- check this
	bool Thresh = true;
	bool Projswitch = true;



	/////////////////////////////////////////////////////////////////////////////
	// ** Variables ** //
	char stepfwd;
	string U, V, W ;
	const char *title;
	string HistTitle;
	gStyle->SetPalette(kRainBow);  // Randbow colour for 2D Hist
	TH2D *UHist;
	TH2D *VHist;
	TH2D *WHist;
	int xSize;
	int ySize;
	if(Projswitch==true){xSize=ySize=1400;}else{xSize=1400;ySize=400;}
	TCanvas *c1= new TCanvas("c1","",xSize,ySize);
	/////////////////////////////////////////////////////////////////////////////
	// ** File input ** //


	TFile *f1;
	if(TimeOrMm == 0){
		f1 = TFile::Open(inputDirecTime.c_str());
	}else if(TimeOrMm==1){
		f1 = TFile::Open(inputDirecMm.c_str());
	}else{
		cout<<RED<<"Invalid 'TimeOrDistance' option"<<endl;
		cout<<"For distance in time bins, set = 0"<<endl;
		cout<<"For Distance in mm, set = 1"<<RESET<<RESET<<endl;
		return -1;
	}

	cout<<endl<<"************"<<endl;
	if(f1!=0) cout<<GREEN<<"File opened succesfully"<<RESET<<endl;
	if(f1==0) cout<<RED<<"File Failed opening"<<RESET<<endl;	
	cout<<"************"<<endl<<endl;


	/////////////////////////////////////////////////////////////////////////////
	// ** main loop** //
	for(int evt =2 ;evt<nEntries;evt++){
		cout<<"Event "<<evt<<endl;


		if(TimeOrMm == 0){
			U = "hraw_U_vs_time_evt"+to_string(evt)+";1";	//<<<------------ Check
			V = "hraw_V_vs_time_evt"+to_string(evt)+";1";	
			W = "hraw_W_vs_time_evt"+to_string(evt)+";1";
			HistTitle = "ELITPC Tracks: Raw Strips Vs Time";
		}else if(TimeOrMm==1){	
			U = "hraw_U_vs_time_mm_evt"+to_string(evt)+";1";	//<<<------------ Check 
			V = "hraw_V_vs_time_mm_evt"+to_string(evt)+";1";
			W = "hraw_W_vs_time_mm_evt"+to_string(evt)+";1";
			HistTitle = "ELITPC Tracks: Raw Strips Vs mm";	
		}else{cout<<"ERROR"<<endl;return -1;}

		f1->GetObject(U.c_str(),UHist);
		f1->GetObject(V.c_str(),VHist);
		f1->GetObject(W.c_str(),WHist);


		///////////////////////////////////////////////////////////////////////////
		// ** Analysis ** //
		if(Thresh == true){
			Threshold(UHist);
			Threshold(VHist);
			Threshold(WHist);
		}


		///////////////////////////////////////////////////////////////////////////
		// ** Projections ** //

		if(Projswitch==true){


			TH1D *UX, *UY,*VX,*VY,*WX,*WY;
			std::tuple<double,double,double> lenUX, lenUY, lenVX, lenVY, lenWX, lenWY;
			double length, xMin, XMax;
			double Y[2]={0.1,0.1};

			c1->Divide(3,3);

			// ** U Strips ** //
			//x 
			UX = proj(UHist,0);
			title = "Strip U X Proj";;
			lenUX = trackProjInfo(UX,1,1); 
			length = get<0>(lenUX); 
			cout<<"Len U xproj = "<<length<<" mm"<<endl;
			double X1[2]={get<1>(lenUX),get<2>(lenUX)};
			endpointsProj1 = new TGraph(2,X1,Y);
			drawX(c1,4,title,UX,endpointsProj1); 

			//y
			UY= proj(UHist,1);
			title = "Strip U Y Proj" ;
			lenUY = trackProjInfo(UY,1,1); 
			length = get<0>(lenUY);
			cout<<"Len U yproj = "<<length<<" mm"<<endl<<endl;
			double X2[2]={get<1>(lenUY),get<2>(lenUY)};
			endpointsProj2 = new TGraph(2,X2,Y);
			drawY(c1,7,title,UY,endpointsProj2);

			// ** V Strips** //
			// x
			VX = proj(VHist,0);
			title = "Strip V X Proj";
			lenVX = trackProjInfo(VX,1,1);
			length = get<0>(lenVX);
			cout<<"Len V Xproj = "<<length<<" mm"<<endl;
			double X3[2]={get<1>(lenVX),get<2>(lenVX)};
			endpointsProj3 = new TGraph(2,X3,Y);
			drawX(c1,5,title,VX,endpointsProj3);
			//y
			VY= proj(VHist,1); 
			title = "Strip V Y Proj" ;
			lenVY = trackProjInfo(VY,1,1);
			length = get<0>(lenVY);
			cout<<"Len V Yproj = "<<length<<" mm"<<endl<<endl;
			double X4[2]={get<1>(lenVY),get<2>(lenVY)};
			endpointsProj4 = new TGraph(2,X4,Y);
			drawY(c1,8,title,VY,endpointsProj4);

			// ** W Strips ** //
			// x
			WX = proj(WHist,0);
			title = "Strip W X Proj";
			lenWX = trackProjInfo(WX,1,1);
			length = get<0>(lenWX); 
			cout<<"Len W Xproj = "<<length<< " mm"<<endl;
			double X5[2]={get<1>(lenWX),get<2>(lenWX)};
			endpointsProj5 = new TGraph(2,X5,Y);
			drawX(c1,6,title,WX,endpointsProj5);
			//x
			WY= proj(WHist,1);
			title = "Strip W Y Proj" ;
			lenWY = trackProjInfo(WY,1,1);
			length = get<0>(lenWY); 
			cout<<"Len W YProj = "<<length<<" mm"<<endl<<endl;
			double X6[2]={get<1>(lenWY),get<2>(lenWY)};
			endpointsProj6 = new TGraph(2,X6,Y);
			drawY(c1,9,title,WY,endpointsProj6);


			draw(UHist,c1,1);
			draw(VHist,c1,2);
			draw(WHist,c1,3);
			processImg(c1);


		}else if(Projswitch==false)
		{

			c1->Divide(3,1);
			c1->SetWindowPosition(1,1);
			c1->SetTitle(HistTitle.c_str());
			draw(UHist,c1,1);
			draw(VHist,c1,2);
			draw(WHist,c1,3);
			processImg(c1);
		}


		///////////////////////////////////////////////////////////////////////////
		// ** Stepper ** //
		cout<<"************"<<endl;
		cout<<"Step forward? y/n"<<endl;
		cin>>stepfwd;
		if(stepfwd=='n'){delete c1;f1->Close(); return 0;}	



		///////////////////////////////////////////////////////////////////////////
		// ** Clean up ** //
		c1->Clear();

		delete endpointsProj1;
		delete endpointsProj2;	
		delete endpointsProj3;	
		delete endpointsProj4;	
		delete endpointsProj5;	
		delete endpointsProj6;		


}

delete c1;
f1->Close();


return 0;
}

