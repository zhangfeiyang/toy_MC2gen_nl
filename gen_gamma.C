#include "rootheader.h"
double p0 = 1.07;
double p1 = 0.02;
	
int main(int argc,char **argv){

	ifstream fin("pdf_log");
	const int n = 8;
	
	string filename;
	TH1D *h1[n];
	TH1D *h0[n];
		
	for(int i=0;i<n;i++){
		fin>>filename;
    	TFile *file = new TFile(&filename[0],"read");
		h1[i] = (TH1D*)file->Get("h1");
		h0[i] = (TH1D*)file->Get("h0");
	//	file->Close();	
	}
	
	double sum0 = 0;
	double sum1 = 0;
	double energy,prob,abs_e;
	double gamma_energy[8] = {0.5109989,0.6617,0.834848,1.25287,1.4608,2.223,4.945,6.13};
	for(int j=0;j<8;j++){
		sum0 = 0;
		sum1 = 0;
		for(int i=0;i<5000;i++){
			energy = h1[j]->GetBinCenter(i+1);
			prob = h1[j]->GetBinContent(i+1);;
			sum0 += energy*prob;
	
			abs_e = TMath::Abs(energy);
			sum1 += energy*prob*(p0 + p1*abs_e);
		}
		cout << gamma_energy[j] << "\t" << sum1/sum0 << "\n"; 
	}
	return 0;
}
