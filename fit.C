#include "rootheader.h"
TH1D *h_K40;
TH1D *h_n_H;
TH1D *h_n_Fe;
TH1D *h_n_C;
TH1D *h_Am_C;
TH1D *h_Ge68;
TH1D *h_Cs137;
TH1D *h_Co60;
TH1D *h_Mn54;
TH1D *h;
TH1F* h_scale_K40;  
TH1F* h_scale_n_H;  
TH1F* h_scale_n_Fe;
TH1F* h_scale_n_C;  
TH1F* h_scale_Am_C;
TH1F* h_scale_Ge68;
TH1F* h_scale_Cs137;
TH1F* h_scale_Co60;
TH1F* h_scale_Mn54; 
TH1F *h_scale;
double get_ratio(double *X,double *p){
	
		double prob[5000],energy[5000];
		double xx = X[0];
        double scale;
        if(xx<(0.6617+0.511)/2)                        {h = h_Ge68;h_scale =  h_scale_K40;  }
        if(xx>(0.6617+0.511)/2 && xx<(0.6617+0.835)/2) {h = h_Cs137;h_scale = h_scale_n_H;  }
        if(xx>(0.6617+0.835)/2 && xx<(1.2500+0.835)/2) {h = h_Mn54;h_scale =  h_scale_n_Fe; }
        if(xx>(1.2500+0.835)/2 && xx<(1.4608+1.250)/2) {h = h_Co60;h_scale =  h_scale_n_C;  }
        if(xx>(1.4608+1.250)/2 && xx<(1.4608+2.223)/2) {h = h_K40;h_scale =   h_scale_Am_C; }
        if(xx>(1.4608+2.223)/2 && xx<(4.9450+2.223)/2) {h = h_n_H;h_scale =   h_scale_Ge68; }
        if(xx>(4.9450+2.223)/2 && xx<(4.9450+6.130)/2) {h = h_n_C;h_scale =   h_scale_Cs137;}
        if(xx>(4.9450+6.130)/2 && xx<(6.1300+7.500)/2) {h = h_Am_C;h_scale =  h_scale_Co60; }
        if(xx>(6.1300+7.500)/2)                        {h = h_n_Fe;h_scale =  h_scale_Mn54; }

		scale = h_scale->GetMean();
		double abs_e;
		double s1=0,s2=0;
		for(int i=0;i<5000;i++) {
				energy[i]=h->GetBinCenter(i+1);
				prob[i]=h->GetBinContent(i+1);
				s2 += energy[i]*prob[i]; 
				abs_e = TMath::Abs(energy[i]);
				s1 += energy[i]*prob[i]*(p[0]+p[1]*abs_e); 
		}
	    return s1/s2;
};


int main(int argv,char **argc){
	
	int seed = atoi(argc[1]);
	gRandom->SetSeed(seed);	

    TFile *file_K40   = new TFile("/home/zhangfy/nl_target/toyMC/K40_pdf.root","read");
    TFile *file_n_H   = new TFile("/home/zhangfy/nl_target/toyMC/n_H_pdf.root","read");
    TFile *file_n_Fe  = new TFile("/home/zhangfy/nl_target/toyMC/n_Fe_pdf.root","read");
    TFile *file_n_C   = new TFile("/home/zhangfy/nl_target/toyMC/n_C_pdf.root","read");
    TFile *file_Am_C  = new TFile("/home/zhangfy/nl_target/toyMC/Am_C_pdf.root","read");
    TFile *file_Ge68  = new TFile("/home/zhangfy/nl_target/toyMC/Ge68_pdf.root","read");
    TFile *file_Cs137 = new TFile("/home/zhangfy/nl_target/toyMC/Cs137_pdf.root","read");
    TFile *file_Co60  = new TFile("/home/zhangfy/nl_target/toyMC/Co60_pdf.root","read");
    TFile *file_Mn54  = new TFile("/home/zhangfy/nl_target/toyMC/Mn54_pdf.root","read");

    h_K40  = (TH1D*)file_K40->Get("h1");
    h_n_H  = (TH1D*)file_n_H->Get("h1");
    h_n_Fe = (TH1D*)file_n_Fe->Get("h1");
    h_n_C  = (TH1D*)file_n_C->Get("h1");
    h_Am_C = (TH1D*)file_Am_C->Get("h1");
    h_Ge68 = (TH1D*)file_Ge68->Get("h1");
    h_Cs137= (TH1D*)file_Cs137->Get("h1");
    h_Co60 = (TH1D*)file_Co60->Get("h1");
    h_Mn54 = (TH1D*)file_Mn54->Get("h1");

    h_scale_K40  = (TH1F*)file_K40->Get("h0");
    h_scale_n_H  = (TH1F*)file_n_H->Get("h0");
    h_scale_n_Fe = (TH1F*)file_n_Fe->Get("h0");
    h_scale_n_C  = (TH1F*)file_n_C->Get("h0");
    h_scale_Am_C = (TH1F*)file_Am_C->Get("h0");
    h_scale_Ge68 = (TH1F*)file_Ge68->Get("h0");
    h_scale_Cs137= (TH1F*)file_Cs137->Get("h0");
    h_scale_Co60 = (TH1F*)file_Co60->Get("h0");
    h_scale_Mn54 = (TH1F*)file_Mn54->Get("h0");


	const int n = 8;
	double energy[n],r[n],Erec[n],eErec[n];
	double pe[n],epe[n],sigma[n],esigma[n];
	double er[n];
	ifstream fin("/home/zhangfy/nl_target/toyMC/data0");
	ifstream fine("/home/zhangfy/nl_target/toyMC/uncertainty");
	double bias[n],ebias[n];

	TFile *data_file = new TFile((Form("/home/zhangfy/nl_target/results/data_%i.root",seed)),"recreate");
	TTree *t_data = new TTree("t","");
	double pars[5];
	double chi2;
	t_data->Branch("pars",pars,"pars[5]/D");
	t_data->Branch("r",r,"r[8]/D");
	t_data->Branch("er",er,"er[8]/D");
	t_data->Branch("chi2",&chi2,"chi2/D");

	for(int i=0;i<n;i++){
			/*fin>>energy[i]>>pe[i]>>epe[i]>>sigma[i]>>esigma[i];
			Erec[i] = pe[i]/scale;	
			eErec[i] = epe[i]/scale;	
			if(i==0 || i==3) {
				Erec[i] /= 2;
			}
			r[i] = Erec[i]/energy[i];
			er[i] = eErec[i]/energy[i];
			*/
			fine>>bias[i]>>ebias[i];
			fin>>energy[i]>>r[i];
			
			r[i] *= 1 + gRandom->Gaus(bias[i],ebias[i])/100.0;
			er[i] = TMath::Abs(r[i]*ebias[i]/100.0); 
			cout<<r[i]<<"\t"<<er[i] <<endl;
			
	}
	
	TCanvas *c1 = new TCanvas();
	
	TGraphErrors *T1 = new TGraphErrors(n,energy,r,0,er);
    T1->SetMarkerStyle(20);
    T1->SetMarkerSize(0.8);
    T1->SetLineWidth(2);
    T1->SetLineColor(kBlack);
    T1->SetMarkerColor(kBlack);
    T1->Draw("Ap");

	TF1 *f1 = new TF1("f1",get_ratio,0,8,2);
	f1->SetParameters(1.05412,0.0069017);
	T1->Fit("f1","","",0,8);

	double* tmp_pars = f1->GetParameters();
	chi2 = f1->GetChisquare();
	
	for(int i=0;i<5;i++)
		pars[i] = tmp_pars[i];	
	
	t_data->Fill();
	t_data->Write();
	
	double fit[n];
	for(int i=0;i<n;i++) {
		fit[i] = f1->Eval(energy[i]);
	}
	TGraph *T2 = new TGraph(n,energy,fit);

    T2->SetMarkerColor(kRed);
    T2->SetLineColor(kRed);
    T2->SetLineWidth(2);
    T2->SetMarkerStyle(20);
    T2->SetMarkerSize(0.8);
    T2->Draw("pCsame");

    gStyle->SetStatY(0.6);
    gStyle->SetStatX(0.9);
    gStyle->SetStatH(0.18);
    gStyle->SetStatW(0.18);

	c1->Print(Form("/home/zhangfy/nl_target/toyMC/results/data_%i.png",seed));	
	c1->Print(Form("/home/zhangfy/nl_target/toyMC/results/data_%i.C",seed));	
	c1->Write();
	data_file->Close();

	//double th_r[n]={0.961133,0.968143,0.97784,0.994842,0.997859,1.02436,1.05276};
	////TGraph *T2 = new TGraph(n,energy,th_r);
	////T2->Draw("same");
	//TLegend *l = new TLegend(0.1,0.2,0.3,0.4);
	//l->AddEntry(T,"gamma energy non-linearity","l");
	//l->AddEntry(f,"electron energy non-linearity","l");
	//l.Draw();

	return 0;
}
