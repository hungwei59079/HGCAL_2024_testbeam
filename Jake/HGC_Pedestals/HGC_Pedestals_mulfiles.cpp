#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

// Include ROOT headers with quotes for compatibility with g++
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TLorentzVector.h"
#include "Rtypes.h"

// Include custom header
#include "headers/data_of_test_beam.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << "<output filename> <input filenames>" << std::endl << "This program can help you to combine several root files of HGCal data to get the pedestals "<< std::endl;
        return 1;
    } 
	else{
		std::cout << "there are " << argv - 1 << " files" << std::endl;
	}

    const char* output_filename = argv[1];

	//Init inout and output files/trees.

    TFile * output = TFile::Open(output_filename, "RECREATE");

	TFile * HGC_Data;
	TTree * HGC_Events;
	UShort_t HGC_ADC[1332];

	//Init output hists

	char name[100], title[100];
	TH1F * H_ADC[6][222] = {};
   	for ( int i = 0; i < 6; i++){
		for ( int j = 0; j < 222; j++){
			sprintf(name, "mod %i ch %i ADCs", i, j);
			sprintf(title, "mod %i ch %i ADCs", i, j);
			H_ADC[i][j] = new TH1F ( name, title, 300, 0, 300);
			H_ADC[i][j] -> GetXaxis() -> SetTitle("ADC");
			H_ADC[i][j] -> GetYaxis() -> SetTitle("Events");
		}
	}

	int NofE, NofE_N = 0;//nambers of the events of ecah file
	for ( int NofF = 2; NofF < argc; NofF ++ ){
		std::cout << "processing: " << NofF << std::endl;
		HGC_Data = TFile :: Open (argv[NofF]);//the first arg is the output filename
		HGC_Events = ( TTree * ) HGC_Data -> Get("Events");
		HGC_Events -> SetBranchAddress ( "HGCDigi_adc", HGC_ADC );
		HGC_Events -> SetBranchStatus ("*", 0);
		HGC_Events -> SetBranchStatus( "HGCDigi_adc", 1);
		
		int NofE_T = HGC_Events -> GetEntries();

		for ( NofE = 0; NofE < NofE_T; NofE ++ ){
			HGC_Events -> GetEntry(NofE);
			NofE_N ++;
			if ( NofE_N % 10000 == 0 ){
				std::cout << "processing: " << NofE_N << std::endl;
			}
			for ( int i = 0; i < 6 ; i++){
				for ( int j = 0; j < 222; j++){
					H_ADC[i][j] -> Fill(HGC_ADC[i*6+j]);
				}
			}
		}

		HGC_Data -> Close();

	}

	output -> cd();

	for ( int i = 0; i < 6; i++ ){
		for ( int j = 0; j < 222; j++){
			H_ADC[i][j] -> Write();
		}
	}

	std::cout << "Processing complete. Results saved to " << output_filename << std::endl;

	output -> Close();
        return 0;
    }


