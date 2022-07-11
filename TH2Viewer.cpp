
#include "TH2Analysis.cpp"
#include "structures.hpp"
#include "draw.hpp"
#include "Ransac.cpp"
#include <tuple>
using namespace std;



void TH2Viewer(){

	/////////////////////////////////////////////////////////////////////////////
	// ** Switches ** //
	bool view_mm = true;
	bool view_time = false;

	bool analysis_mm = true;
	bool analysis_time = false;

	bool projSwitch_mm = true;
	bool projSwitch_time = false;


	/////////////////////////////////////////////////////////////////////////////
	// ** Variables ** //
	char stepfwd;
	int nEntries = 2674;
	string U_Time, V_Time, W_Time; 
	string U_mm, V_mm, W_mm; 
	string inputTree1, inputTree2;
	TFile *f1, *f2;
	const char *title;
	gStyle->SetPalette(kRainBow);  // Randbow colour for 2D Hist

	/////////////////////////////////////////////////////////////////////////////
	// ** File input ** //
	inputTree1 ="/home/kris/Desktop/TrackTreeTime.root";  //<<<--- Change
	f1 = TFile::Open(inputTree1.c_str());

	inputTree2 ="/home/kris/Desktop/TrackTree_2022-04-12T08:03:44.531_0000.root";
	f2 = TFile::Open(inputTree2.c_str());

	// ** Error managment ** //
	cout<<endl<<"************"<<endl;
	if(f1!=0) cout<<"File 1 opened succesfully"<<endl;
	if(f1==0) cout<<"File 1 Failed opening"<<endl;		
	if(f2!=0) cout<<"File 2 opened succesfully"<<endl;
	if(f2==0) cout<<"File 2 Failed opening"<<endl;
	cout<<"************"<<endl<<endl;


	/////////////////////////////////////////////////////////////////////////////
	// ** main loop** //
	for(int evt =1 ;evt<nEntries;evt++){




		
		///////////////////////////////////////////////////////////////////////////
		// ** Fetch TH2s - Time** //
		if(view_time == true){
			U_Time = "hraw_U_vs_time_evt"+to_string(evt)+";1";	//<<<------------ Check
			V_Time = "hraw_V_vs_time_evt"+to_string(evt)+";1";	
			W_Time = "hraw_W_vs_time_evt"+to_string(evt)+";1";	

			f1->GetObject(U_Time.c_str(),UHist_Time);
			f1->GetObject(V_Time.c_str(),VHist_Time);
			f1->GetObject(W_Time.c_str(),WHist_Time);
		}

		// ** Fetch TH2s - mm ** //

		if(view_mm == true){
			U_mm = "hraw_U_vs_time_mm_evt"+to_string(evt)+";1";	//<<<------------ Check 
			V_mm = "hraw_V_vs_time_mm_evt"+to_string(evt)+";1";
			W_mm = "hraw_W_vs_time_mm_evt"+to_string(evt)+";1";	

			f2->GetObject(U_mm.c_str(),UHist_mm);
			f2->GetObject(V_mm.c_str(),VHist_mm);
			f2->GetObject(W_mm.c_str(),WHist_mm);
		}

		///////////////////////////////////////////////////////////////////////////
		// ** Analysis** //
		if(analysis_time == true){
			Threshold(UHist_Time);
			Threshold(VHist_Time);
			Threshold(WHist_Time);
		}

		if(analysis_mm == true){
			Threshold(UHist_mm);
			Threshold(VHist_mm);
			Threshold(WHist_mm);
		}



		///////////////////////////////////////////////////////////////////////////
		// ** Plotting - Time ** //

		if(view_time == true){

			
			c1 = new TCanvas("c1","ELITPC Tracks: Raw Strips Vs Time",1400,410);
			c1->Divide(3,1);
			c1->SetWindowPosition(1,1);
			draw(UHist_Time,c1,1);
			draw(VHist_Time,c1,2);
			draw(WHist_Time,c1,3);
			processImg(c1);

			// ** projections - time ** //
			if(projSwitch_time==true){
				c11 = new TCanvas("c11","Time U proj",450,550);
				c12 = new TCanvas("c12","Time V proj",450,550);
				c13 = new TCanvas("c13","Time W proj",450,550);
				c11->SetWindowPosition(1,480);
				c12->SetWindowPosition(550,480);
				c13->SetWindowPosition(1030,480);
				c11->Divide(1,2);
				c12->Divide(1,2);
				c13->Divide(1,2);

				UXt = proj(UHist_Time,0); title = "Strip U X Proj";;
				draw(c11,1,title,UXt);
				UYt	= proj(UHist_Time,1); title = "Strip U Y Proj" ;
				draw(c11,2,title,UYt);
				processImg(c11);

				VXt = proj(VHist_Time,0); title = "Strip V X Proj";;
				draw(c12,1,title,VXt);
				VYt	= proj(VHist_Time,1); title = "Strip V Y Proj" ;
				draw(c12,2,title,VYt);
				processImg(c12);

				WXt = proj(WHist_Time,0); title = "Strip W X Proj";;
				draw(c13,1,title,WXt);
				WYt	= proj(WHist_Time,1); title = "Strip W Y Proj" ;
				draw(c13,2,title,WYt);
				processImg(c13);

			}



		}

		// ** Plotting - mm ** //
		if(view_mm == true){
			cout<<"Event "<<evt<<endl;
			std::tuple<double,double,double> lenUX, lenUY, lenVX, lenVY, lenWX, lenWY;
			double length, xMin, XMax;
			double Y[2]={0.1,0.1};
	
			

			// ** projections - mm ** //
			if(projSwitch_mm==true){
				c21 = new TCanvas("c21","mm U proj",450,550);
				c22 = new TCanvas("c22","mm V proj",450,550);
				c23 = new TCanvas("c23","mm W proj",450,550);
				c21->SetWindowPosition(1,480);
				c22->SetWindowPosition(550,480);
				c23->SetWindowPosition(1030,480);
				c21->Divide(1,2);
				c22->Divide(1,2);
				c23->Divide(1,2);



				// ** U Strips ** //
				//x 
				UXmm = proj(UHist_mm,0); title = "Strip U X Proj";;
				lenUX = 	trackProjInfo(UXmm,1,1); 
				length = get<0>(lenUX); cout<<"Len U xproj = "<<length<<" mm"<<endl;
				double X1[2]={get<1>(lenUX),get<2>(lenUX)};
				endpointsProj1 = new TGraph(2,X1,Y);
				drawX(c21,1,title,UXmm,endpointsProj1); 
				//y
				UYmm	= proj(UHist_mm,1); title = "Strip U Y Proj" ;
				lenUY = 	trackProjInfo(UYmm,1,1); 
				length = get<0>(lenUY); cout<<"Len U yproj = "<<length<<" mm"<<endl<<endl;
				double X2[2]={get<1>(lenUY),get<2>(lenUY)};
				endpointsProj2 = new TGraph(2,X2,Y);
				drawY(c21,2,title,UYmm,endpointsProj2);

				// ** V Strips** //
				// x
				VXmm = proj(VHist_mm,0); title = "Strip V X Proj";;
				lenVX = 	trackProjInfo(VXmm,1,1);
				length = get<0>(lenVX); cout<<"Len V Xproj = "<<length<<" mm"<<endl;
				double X3[2]={get<1>(lenVX),get<2>(lenVX)};
				endpointsProj3 = new TGraph(2,X3,Y);
				drawX(c22,1,title,VXmm,endpointsProj3);
				//y
				VYmm	= proj(VHist_mm,1); title = "Strip V Y Proj" ;
				lenVY = 	trackProjInfo(VYmm,1,1);
				length = get<0>(lenVY); cout<<"Len V Yproj = "<<length<<" mm"<<endl<<endl;
				double X4[2]={get<1>(lenVY),get<2>(lenVY)};
				endpointsProj4 = new TGraph(2,X4,Y);
				drawY(c22,2,title,VYmm,endpointsProj4);

				// ** W Strips ** //
				// x
				WXmm = proj(WHist_mm,0); title = "Strip W X Proj";;
				lenWX = 	trackProjInfo(WXmm,1,1);
				length = get<0>(lenWX); cout<<"Len W Xproj = "<<length<< " mm"<<endl;
				double X5[2]={get<1>(lenWX),get<2>(lenWX)};
				endpointsProj5 = new TGraph(2,X5,Y);
				drawX(c23,1,title,WXmm,endpointsProj5);
				//x
				WYmm	= proj(WHist_mm,1); title = "Strip W Y Proj" ;
				lenWY = 	trackProjInfo(WYmm,1,1);
				length = get<0>(lenWY); cout<<"Len W YProj = "<<length<<" mm"<<endl<<endl;
				double X6[2]={get<1>(lenWY),get<2>(lenWY)};
				endpointsProj6 = new TGraph(2,X6,Y);
				drawY(c23,2,title,WYmm,endpointsProj6);





				processImg(c21);
				processImg(c22);
				processImg(c23);
			}
			
			// ** Plotting main** //
			c2 = new TCanvas("c2","ELITPC Tracks: Raw Strips Vs mm",1400,410);
			c2->Divide(3,1);
			c2->SetWindowPosition(1,1);
			draw(UHist_mm,c2,1,endpointsProj1,endpointsProj2);
			draw(VHist_mm,c2,2,endpointsProj3,endpointsProj4);
			draw(WHist_mm,c2,3,endpointsProj5,endpointsProj6);
			processImg(c2);


		}


		///////////////////////////////////////////////////////////////////////////
		// ** Stepper ** //
		cout<<"************"<<endl;
		cout<<"Step forward? y/n"<<endl;
		cin>>stepfwd;
		if(stepfwd=='n') return;



		///////////////////////////////////////////////////////////////////////////
		// ** Clean up pointers! ** //
		if(c1)delete c1;
		if(c2)delete c2;
		if(c11)delete c11;
		if(c12)delete c12;
		if(c13)delete c13;
		if(c21)delete c21;
		if(c22)delete c22;
		if(c23)delete c23;
		if(endpointsProj1)delete endpointsProj1;
		if(endpointsProj2)delete endpointsProj2;
		if(endpointsProj3)delete endpointsProj3;
		if(endpointsProj4)delete endpointsProj4;
		if(endpointsProj5)delete endpointsProj5;
		if(endpointsProj6)delete endpointsProj6;


	}



}
