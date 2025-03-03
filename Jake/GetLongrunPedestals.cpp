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
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <filename> <number of bins> <output filename>" << std::endl;
        return 1;
    }

    char name[100], title[100];
    const char* filename = argv[1];
    const int binnumber = std::stoi(argv[2]);

    TFile *f1 = TFile::Open(filename);
    if (!f1 || f1->IsZombie()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    TTree *t1;
    f1->GetObject("Events", t1);
    if (!t1) {
        std::cerr << "Error: TTree 'Events' not found in file!" << std::endl;
        f1->Close();
        return 1;
    }

    // Initialize histograms
    TH1F *h_ADC[6][222] = {};

    // Create Pedestals graphs and draw them
    TGraphErrors *gE[6][222];
    for (int m = 0; m < 6; m++) {
		for ( int i = 0; i < 222; i++){
        	std::cout << "Creating graph for module " << m << "..." << std::endl;
        	gE[m][i] = new TGraphErrors(binnumber);
        	sprintf(title, "module_%i_channel_%i", m, i);
        	gE[m][i]->SetTitle(title);
        	gE[m][i]->SetMarkerColor(4);
        	gE[m][i]->SetMarkerSize(0.2);
        	gE[m][i]->SetMarkerStyle(21);
			gE[m][i]->SetLineStyle(0);
			gE[m][i]->Write(title);
    	}
	}

    // Pedestal calculation arrays
    Double_t Pedestals[6][222], Total_Noise[6][222], Pedestals_Error[6][222];

    int n_events = t1->GetEntries();
    std::cout << "Filling histograms for " << n_events << " events..." << std::endl;

    data_of_test_beam obj = data_of_test_beam(t1);
	t1->SetBranchStatus("*",0);
	t1->SetBranchStatus("HGCDigi_adc",1);	

    sprintf(title, "%s_Pedestals.root", argv[3]);
	TFile *outputFile = TFile::Open(title, "RECREATE");
	TTree *PedestalsTree = new TTree( "Pedestals", "The data of pesetals" );
	
	PedestalsTree -> Branch ( "HGC_Pedestals", Pedestals, "HGC_Pedestals[6][222]/D");
	PedestalsTree -> Branch ("HGC_Pedestal_Errors", Pedestals_Error, "HGC_Pedestal_Errors[6][222]/D");
	PedestalsTree -> Branch ("HGC_Total_Noise", Total_Noise, "HGC_Total_Noise[6][222]/D");

	outputFile -> cd();
    // Fill histograms
	int events_of_section, total_number = 0;
    for ( int s = 0; s < binnumber ; s++){
		if ( n_events % binnumber > s ){
			events_of_section = n_events / binnumber + 1;
		}
		else{
			events_of_section = n_events / binnumber;
		}

    	for (int m = 0; m < 6; m++) {
        	// std::cout << "Initializing histograms for module " << m + 1 << "..." << std::endl;
        	for (int i = 0; i < 222; i++) {
            	sprintf(name, "mod_%i_ch_%i", m, i);
            	sprintf(title, "mod_%i_ch_%i", m, i);
            	h_ADC[m][i] = new TH1F(name, title, 300, 0, 300);
            	h_ADC[m][i]->GetXaxis()->SetTitle("ADC");
            	h_ADC[m][i]->GetYaxis()->SetTitle("Events");
        	}
    	}

    	for ( int i = 0; i < events_of_section ; i++ ) {
	        if (total_number % 1000 == 0) std::cout << "Processing event " << total_number << " / " << n_events << std::endl;
        	t1->GetEntry(total_number);
	        for (int m = 0; m < 6; m++) {
            	for (int c = 0; c < 222; c++) {
                	if (h_ADC[m][c]) {
                	    h_ADC[m][c]->Fill(obj.HGCDigi_adc[222 * m + c]);
                	}
            	}
        	}
			total_number ++;
    	}

    	// Calculate and write pedestals and noise
		for ( int m = 0; m < 6; m++){
			for ( int i = 0; i < 222; i++ ){
            	Pedestals[m][i] = h_ADC[m][i]->GetMean();
            	Total_Noise[m][i] = h_ADC[m][i]->GetStdDev();
            	Pedestals_Error[m][i] = h_ADC[m][i]->GetMeanError();
				gE[m][i] -> SetPoint( s, s + 1, Pedestals[m][i]);
				gE[m][i] -> SetPointError( s, 0, Total_Noise[m][i]);
				//h_ADC[m][i] -> Write();
				delete h_ADC[m][i];
			}
		}

		PedestalsTree -> Fill();
		//printf("%.1lf %.1lf %.1lf\n", Pedestals[0][0], Total_Noise[0][0], Pedestals_Error[0][0]);
	}

	PedestalsTree -> Write();

	
    sprintf(title, "%s_Pedestals_Graphs.root", argv[3]);
	TFile *outputGraphFile = TFile::Open(title, "RECREATE");
	outputGraphFile -> cd();
    for (int m = 0; m < 6; m++) {
        std::cout << "saving graph for module " << m << "..." << std::endl;
		for ( int i = 0; i < 222; i++){
			gE[m][i] -> Write();
    	}
	}
	
    f1->Close();
	outputFile -> Close();
	outputGraphFile -> Close();

	std::cout << "Processing complete. Results saved to " << title << std::endl;
	return 0;
}
