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
	char file_name[100];
	//std::cout << "bb\n";
	sprintf( file_name, "%s.root", argv[1]);
	
	//std::cout << "before\n";
	TFile * files;
	//std::cout << "after\n";
	TTree * Events;
	Double_t HGC_Pedestals[6][222];
	Double_t HGC_Pedestals_Noise[6][222];
	TFile * output_file = TFile::Open(file_name, "RECREATE");
	int num_of_sections;
	std::vector <Double_t> x;
	std::vector <Double_t> Pedestals_y, Pedestals_y_Err;
	TH1F * HGC_Pedestals_Hist [6][222];
	char name[100], title[100];
	for ( int i = 0; i < 6; i++ ){
		for ( int j = 0; j < 222; j++ ){
			sprintf(name, "mod_%i_ch_%i", i, j);
			sprintf(title, "mod_%i_ch_%i", i, j);
			HGC_Pedestals_Hist[i][j] = new TH1F( name, title, 4000, 50, 250);
		}
	}
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
			for ( int k = 0; k < 6; k++ ){
				for ( int m = 0; m < 222; m++ ){
					//std::cout << "Filling\n" ;
					HGC_Pedestals_Hist[k][m] -> Fill ( HGC_Pedestals[k][m] );
				}
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

	sprintf ( file_name, "%s_high_stdev.txt", argv[1]);
	FILE * file_of_high_stdev = fopen (file_name, "w");
	sprintf ( file_name, "%s_all_stdev.txt", argv[1]);
	FILE * file_of_all_stdev = fopen (file_name, "w");

	for ( int k = 0; k < 6; k++ ){
		for ( int m = 0; m < 222; m++ ){
			HGC_Pedestals_Hist[k][m] -> Write();
			fprintf( file_of_all_stdev, "%.4lf", HGC_Pedestals_Hist[k][m] -> GetStdDev());
			if ( m < 221 ){
				fprintf( file_of_all_stdev, " ");
			}
			if ( HGC_Pedestals_Hist[k][m] -> GetStdDev() > 0.19 ){
				std::cout << k << " " << m << HGC_Pedestals_Hist[k][m] -> GetStdDev() << std::endl;
				fprintf( file_of_high_stdev, "%d %d %.4lf\n", k, m, HGC_Pedestals_Hist[k][m] -> GetStdDev());
			}
		}
		fprintf( file_of_all_stdev, "\n");
	}

	std::cout << "finished\n";

	fclose(file_of_high_stdev);
	fclose(file_of_all_stdev);

	delete Pedestals_Canvas;
	delete gE;
	output_file -> Close();

	return 0;
}
