#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TLorentzVector.h"
#include "Rtypes.h"
#include "TCanvas.h"
#include "TStyle.h"

int main ( int argc, char ** argv ){
	if ( argc == 1 ){
		std::cerr << "usage: " << argv[0] << "<output filename> <input file1> <input file2> ..." << std::endl ;
		return 1;
	}

	std::cout << "there are " << argc - 2<< " files to plot\n";
	int n_of_files = argc - 2;
	TFile * files;
	TTree * Events;
	Double_t HGC_Pedestals[6][222];
	Double_t HGC_Pedestals_Noise[6][222];
	TFile * output_file = TFile::Open(argv[1], "RECREATE");
	int num_of_sections;
	std::vector <Double_t> x;
	std::vector <Double_t> Pedestals_y, Pedestals_y_Err;
	TH1F * HGC_Pedestals_Hist [6][222];
	char name[100], title[100];
	for ( int i = 0; i < 6; i++ ){
		for ( int j = 0; j < 222; j++ ){
			sprintf(name, "mod_%i_ch_%i", i, j);
			sprintf(title, "mod_%i_ch_%i", i, j);
			HGC_Pedestals_Hist[i][j] = new TH1F( name, title, 300, 

	for ( int i = 0; i < n_of_files; i++){
		std::cout << "Processing: " << i + 1<< "/" << n_of_files << ".\n";
		files = TFile:: Open(argv[i+2]);
		Events = (TTree *) files -> Get ( "Pedestals" );
		num_of_sections = Events -> GetEntries();
		Events -> SetBranchAddress("HGC_Pedestals", HGC_Pedestals);
		Events -> SetBranchAddress("HGC_Total_Noise", HGC_Pedestals_Noise);
		for( int j = 0; j < num_of_sections; j++){
			Events -> GetEntry(j);
			for ( int k = 0; k < 37; k++ ){
				//std::cout << k << " " << HGC_Pedestals[0][k] << "\n";
				x. push_back (k + 1);
				Pedestals_y. push_back( HGC_Pedestals[0][k]);
				Pedestals_y_Err. push_back( HGC_Pedestals_Noise[0][k]);
			}
		}

		files -> Close();
	}

	output_file -> cd();

	TGraphErrors * gE = new TGraphErrors(x.size());
	for ( int i = 0; i < x.size(); i++){
		gE -> SetPoint( i , x[i], Pedestals_y[i]);
		gE -> SetPointError( i, 0, Pedestals_y_Err[i]);
	}
		
    gE -> SetTitle("Pedestals");
    gE -> SetMarkerColor(4);
    gE -> SetMarkerSize(0.6);
    gE -> SetMarkerStyle(21);
	gE -> SetLineStyle(0);

	TCanvas * Pedestals_Canvas = new TCanvas("Pedestals for first 37 channels", "Pedestals for 37 channels", 800, 600);
	Pedestals_Canvas -> SetGrid();               // 設定畫布網格
    gStyle->SetEndErrorSize(4);

	gE -> Draw("AP");
	gE -> Write();

	std::cout << "finished\n";

	delete Pedestals_Canvas;
	delete gE;
	output_file -> Close();

	return 0;
}
