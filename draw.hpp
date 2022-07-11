#ifndef draw_hpp
#define draw_hpp

#include <stdio.h>
void drawX(TCanvas *c,int dir,const char *title ,TH1D *Hist,TGraph *end){
	

	c->cd(dir);
	Hist->Draw();
	Hist->SetStats(0);
	Hist->SetTitle(title);
	end->Draw("p");
	end->SetMarkerStyle(5);
	end->SetMarkerColor(2);
	end->SetMarkerSize(3); 
	double ax1, ax2, ay1, ay2;
	end->GetPoint(0,ax1,ay1); 
	end->GetPoint(1,ax2,ay2); 
	//cout<<ax1<<endl<<ax2<<endl;
	Hist->GetXaxis()->SetRangeUser(ax1-10,ax2+10);
}


void drawY(TCanvas *c,int dir,const char *title ,TH1D *Hist,TGraph *end){
	

	c->cd(dir);
	Hist->Draw();
	Hist->SetStats(0);
	Hist->SetTitle(title);
	end->Draw("p");
	end->SetMarkerStyle(5);
	end->SetMarkerColor(2);
	end->SetMarkerSize(3); 
	double ax1, ax2, ay1, ay2;
	end->GetPoint(0,ax1,ay1); 
	end->GetPoint(1,ax2,ay2); 
	//cout<<ax1<<endl<<ax2<<endl;
	Hist->GetXaxis()->SetRangeUser(ax1-10,ax2+10);
}



void draw(TCanvas *c,int dir,const char *title ,TH1D *Hist){
	

	c->cd(dir);
	Hist->Draw();
	Hist->SetStats(0);
	Hist->SetTitle(title);


}

void draw( TH2D *Hist,TCanvas *c, int dir,TGraph *X, TGraph *Y){
	c->cd(dir);
	Hist->Draw("colz2 1");
	Hist->SetStats(0);
	double ax1, ax2, ay1, ay2;
	double bx1, bx2, by1, by2;
	X->GetPoint(0,ax1,bx1); 
	X->GetPoint(1,ax2,bx2); 
	Y->GetPoint(0,ay1,by1); 
	Y->GetPoint(1,ay2,by2); 
if(ax2-ax1>11){Hist->GetXaxis()->SetRangeUser(ax1-30,ax2+30);}else {Hist->GetXaxis()->SetRangeUser(ax1-120,ax2+120);}

if(ay2-ay1>11){Hist->GetYaxis()->SetRangeUser(ay1-30,ay2+30);}else{Hist->GetYaxis()->SetRangeUser(ay1-120,ay2+120);}
}

void draw( TH2D *Hist,TCanvas *c, int dir){
	c->cd(dir);
	Hist->Draw("colz2 1");
	Hist->SetStats(0);
}
void processImg(TCanvas *c){

	c->Modified();
	c->Update();
	gSystem->ProcessEvents();

}
#endif /* ReadTreeH_hpp */


