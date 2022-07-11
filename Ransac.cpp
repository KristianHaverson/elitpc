
struct TPoint_ {
	public:
		double x;
		double y;

		TPoint_ (){
			x=0.0;
			y=0.0;
		}
		TPoint_ (double i, double j){
			x=i;
			y=j;
		}
};


struct Vector {
	public:
		TPoint_ a;
		TPoint_ b;
		double x;
		double y;

		Vector(){
			x=0;
			y=0;
		}
		Vector (TPoint_ p1,TPoint_ p2){
			x=p2.x-p1.x;
			y=p2.y-p1.y;
		}
};


struct Line {
	public:
		TPoint_ start;
		TPoint_ finish;
		Vector direction_vector;

		Line(){};

		Line(TPoint_ st, TPoint_ fin) {
			direction_vector = Vector(st, fin);
			start = st;
			finish = fin;
		};
};


int operator^(const Vector v1, const Vector v2) {
	int ret = ((v1.x) * (v2.y)) - ((v1.y) * (v2.x));
	return ret;
};

int operator*(const Vector v1, const Vector v2) {
	int ret = ((v1.x) * (v2.x)) + ((v1.y) * (v2.y));
	return ret;
};

double distance_line_point(Line l, TPoint_ p) {
	Vector AB = l.direction_vector;
	Vector AC(l.start, p);
	int abs = ((AB)*(AB));
	double norm = (double) std::sqrt(abs);
	//return distance by using area of triangle is base*height / 2
	//and then calculate area using cross product
	double dist = ((double) (AB^AC)) / ((double) norm);
	return std::abs(dist);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

TF1 *Ransac(TH2D *hist, int sampSize, int maxItter, double thresh){

			int X_min=0;
			int X_max=700;
			int BR;
			int iterations;
			int count_0;
			int count_1=1;
			double MaybeInliers_y[sampSize];
			double MaybeInliers_x[sampSize];
			double final_slope;
			double final_offset;
			double par1 ;        // save the parameters from this itteration.
			double par2;
			double Y_min ;       // Define start and end points of the line
			double Y_max;
			double res;
			vector<int> x;
			vector<int> y;
			int k;
			int i;
			int a;
			TF1 *f;
			TF1 *f2;
			
			int xBins = hist->GetNbinsX();
			int yBins = hist->GetNbinsY();
			
			for(int i=0; i<xBins; i++){
				for(int j=0; j<yBins;j++){
					BR =(int) (hist->GetBinContent(i, j)+1 )/50;
					//pixValue = Hist->GetBinContent(i, j);
			
				}
			}
			
			

return f;


};
/*
TF1 RansacFit(double Data[512][672], int SampSize, int MaxItter , double thresh){

			// --- static variables --- //
			int X_min=0;
			int X_max=700;
			int BR;
			int iterations;
			int count_0;
			int count_1=1;
			double MaybeInliers_y[SampSize];
			double MaybeInliers_x[SampSize];
			double final_slope;
			double final_offset;
			double par1 ;        // save the parameters from this itteration.
			double par2;
			double Y_min ;       // Define start and end points of the line
			double Y_max;
			double res;
			vector<int> x;
			vector<int> y;
			int k;
			int i;
			int a;
			TF1 *f;
			TF1 *f2;


			// --- Vector plot of 2D hist --- //
			for ( ipix = 0; ipix < Xpix; ipix++){
				for ( jpix = 0; jpix < Ypix; jpix++){
					BR =(int) (Data[ipix][jpix] +1 )/50; // add one so it rounds to 1; if not it might be zero.
					if(Image2D[ipix][jpix]>0){    //adds more points the brighter the image is there,
						for ( k=0;k<BR;k++){          //weighting depending onm brightness (higher chance of being picked as a radnom point later in the code)
							y.push_back(ipix-256);
							x.push_back(jpix-336);
						}
					}
				}
			}

			// --- Normal fit function of plot --- //
			int size = x.size();
			TFitResultPtr fitResult;
			TGraph vec(size,x.data(),y.data());
			f = new TF1("f", "[0]*x+[1]" ,-300.,300.);
			f->SetParNames("slope","offset");
			fitResult = vec.Fit(f,"q");
			if (fitResult!=0){ fit_NULL=1;return FNULL;} // fit 0 means there is a fit?


			// --- Start of loop --- //
				
			for(iterations=0; iterations<MaxItter;iterations++){
				count_0 = 0; // Set count_0 to 0 at the start of each iteration
				for(i=0; i<SampSize; i++){                 // Save #samp_SIZE random data points to 'maybe_Inliers',
					a = rand() % size +1;                   //RNG between 1 and SIZE
					MaybeInliers_x[i] = x[a];
					MaybeInliers_y[i] = y[a];
				}
				
				// --- Plot2: random sample --- //
				TGraph g2(SampSize, MaybeInliers_x, MaybeInliers_y);                    // create TGraph of sample data
				f2= new TF1("f2", "[0]*x+[1]" ,-300.,300.);                              // Define fit function to random sample data
				f2->SetParNames("sample_slope","sample_offset");                               // Name parameters
				g2.Fit(f2,"Q");

				// --- Rip parameters --- //
				par1 = f2->GetParameter("sample_slope");   // save the parameters from this itteration.
				par2 = f2->GetParameter("sample_offset");

			

				Y_min = (par1*X_min)+ par2;               // Define start and end points of the line
				Y_max = (par1*X_max)+ par2;

				TPoint_ p1(X_min, Y_min);                   // Use point point class
				TPoint_ p2(X_max, Y_max);

				Line l1(p1, p2);

				// --- Rate the model --- //

				for( i = 0; i<size; i++){
					TPoint_ p3(x.at(i),y.at(i));           // for each data point:
					res = distance_line_point(l1, p3);   // calculate its distance from the current iterations model.

					if (res<thresh){count_0++;}          // If the residual is within the threshld then add to "count_0"
					}

					if (count_0>count_1){          //If this iterations model has a higher count of inliers, update the final parameters.
						count_1=count_0;
						final_slope = par1;
						final_offset = par2;
					}
					iterations++;
				}

			y.clear();   //Clear vectors after each image
			x.clear();

				


			TF1 line_func("line_func", "[0]*x+[1]" ,-400.,400.);
			line_func.SetParameter(0,final_slope);
			line_func.SetParameter(1,final_offset);


			delete f;
			delete f2;
	

			return line_func;

		}*/
