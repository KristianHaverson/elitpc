#include "structures.hpp"

void Threshold(TH2D * Hist){

	int xBins = Hist->GetNbinsX();
	int yBins = Hist->GetNbinsY();

	int thresh =  Hist->GetMaximum();
	thresh = 0.1*thresh;
	int pixValue;

	for(int i=0; i<xBins; i++){
		for(int j=0; j<yBins;j++){
			pixValue = Hist->GetBinContent(i, j);
			if(pixValue<thresh){Hist->SetBinContent(i,j,0);}
		} 
	}


}

TH1D* proj(TH2D *Hist, int option){

	TH1D *proj;

	if(option == 0){
		proj = Hist->ProjectionX(); 

	}else if(option ==1){
		proj = Hist->ProjectionY();

	}else{
		cout<<"Please pick valid option"<<endl;
		proj = NULL;
	}
	return proj;
}



std::tuple<double,double,double> trackProjInfo(TH1D *clone,
		double ImageSD,
		double ImageFWXM
		){
	double length;
	double Intensity;
	double iMin_centre;
	double iMax_centre ;
	double jMin_centre;
	double jMax_centre ;
	double X_Min ;
	double X_Max ;
	double Y_Min;
	double Y_Max ;
	Intensity = clone->GetMaximum();

	//Intensity = Intensity*ImageFWXM;                
	Intensity = Intensity*0.1;                

	iMin_centre = clone->FindFirstBinAbove(Intensity);
	iMax_centre = clone->FindLastBinAbove(Intensity);
	X_Min = clone->GetBinCenter(iMin_centre); //get the value here
	X_Max = clone->GetBinCenter(iMax_centre);

	//	cout<<"min"<<X_Min<<endl;
	//	cout<<"max"<<X_Max<<endl;
	for(int i=1 ;i<10; i++){

		if(	clone->GetBinContent(iMax_centre-i)<X_Max){
			for (int k=i;k>=-1;k--){	clone->SetBinContent(iMax_centre-k,0);	}
		}

		if(	clone->GetBinContent(iMin_centre+i)<X_Min){
			for (int k=i;k>=-1;k--){	clone->SetBinContent(iMin_centre+k,0);	}
		}

	}

	iMin_centre = clone->FindFirstBinAbove(Intensity);
	iMax_centre = clone->FindLastBinAbove(Intensity);

	X_Min = clone->GetBinCenter(iMin_centre);
	X_Max = clone->GetBinCenter(iMax_centre);

	length = X_Max - X_Min;
	//	cout<<"Length = "<<length<<endl;

	//	ImageFWXM = 2 * sqrt(2*log(1/ImageFWXM));

	//	length = length - ImageFWXM*ImageSD;

	//cout<<X_Min<<" "<<X_Max<<endl;
	//double X_D2[2]={X_Min,X_Max};
	//double Y_D2[2]={0.1,0.1};
	//	XLimProj[0] = X_Min-30;  
	//	XLimProj[1] = X_Max+30;  
	//endpointsProj = new TGraph(2,X_D2,Y_D2);

	//	delete clone;

	return  std::make_tuple(length,X_Min,X_Max);
}  

/*
	 std::tuple<TGraph,double,double>TGraph projEndpoints(xMin,XMax){
	 double X[2]={X_Min,X_Max};
	 double Y[2]={0.1,0.1};
	 XLimProj[0] = X_Min-30;  
	 XLimProj[1] = X_Max+30;  
	 EndpointsXProj = new TGraph(2,X_D2,Y_D2);



	 }
	 */

void centerPixel(){



}
