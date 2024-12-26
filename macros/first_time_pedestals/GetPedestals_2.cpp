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
#include "TPaveText.h"
// Include custom header
#include "headers/data_of_Relay1726225188.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <filename> <option>" << std::endl;
        return 1;
    }

    const char* filename = argv[1];
    int option = std::stoi(argv[2]);

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
    char name[100], title[100];
    for (int m = 0; m < 6; m++) {
        std::cout << "Initializing histograms for module " << m + 1 << "..." << std::endl;
        for (int i = 0; i < 222; i++) {
            sprintf(name, "mod_%i_ch_%i", m, i);
            sprintf(title, "mod_%i_ch_%i", m, i);
            h_ADC[m][i] = new TH1F(name, title, 300, 0, 300);
            h_ADC[m][i]->GetXaxis()->SetTitle("ADC");
            h_ADC[m][i]->GetYaxis()->SetTitle("Events");
        }
    }

    // Create and configure the histograms for Pedestals subtracted by CM
    TH1F *h_ADC_CM[6][222] = {};
    for (int m = 0; m < 6; m++) {
        std::cout << "Initializing histograms for module " << m + 1 << "..." << std::endl;
        for (int i = 0; i < 222; i++) {
            sprintf(name, "subCM_mod_%i_ch_%i", m , i);
            sprintf(title, "subCM_mod_%i_ch_%i", m , i);
            h_ADC_CM[m][i] = new TH1F(name, title, 300, 0, 300);
            h_ADC_CM[m][i]->GetXaxis()->SetTitle("ADC");
            h_ADC_CM[m][i]->GetYaxis()->SetTitle("Events");
        }
    }

    // Pedestal calculation arrays
    double ch_number[222];
    double Pedestals[6][222], Total_Noise[6][222], Pedestals_Error[6][222];

    int n_events = t1->GetEntries();
    std::cout << "Filling histograms for " << n_events << " events..." << std::endl;

    data_of_Relay1726225188 obj = data_of_Relay1726225188(t1);
	t1->SetBranchStatus("*",0);
	t1->SetBranchStatus("HGCDigi_adc",1);	

    // Fill histograms
    for (int i = 0; i < n_events; i++) {
        if (i % 1000 == 0) std::cout << "Processing event " << i << " / " << n_events << std::endl;
        t1->GetEntry(i);
        for (int m = 0; m < 6; m++) {
            for (int c = 0; c < 222; c++) {
                if (h_ADC[m][c]) {
                    h_ADC[m][c]->Fill(obj.HGCDigi_adc[222 * m + c]);
                }
            }
        }
    }

    // Calculate and write pedestals and noise
    for (int m = 0; m < 6; m++) {
        for (int c = 0; c < 222; c++) {
            ch_number[c] = c * 1.0 + 1.0;
            Pedestals[m][c] = h_ADC[m][c]->GetMean();
            Total_Noise[m][c] = h_ADC[m][c]->GetStdDev();
            Pedestals_Error[m][c] = h_ADC[m][c]->GetMeanError();
        }
    }


	TFile *outputFile = TFile::Open("Fitted_Pedestals.root","RECREATE");

	double CM [6][6];
	std::cout << "Filling histograms CM substracted for " << n_events << " events..." << std::endl;
    for (int i = 0; i < n_events; i++) {
        if (i % 1000 == 0) std::cout << "Processing event " << i << " / " << n_events << std::endl;
        t1->GetEntry(i);
        for (int m = 0; m < 6; m++) {
			for (int h = 0; h < 6; h++){
				for (int c = 0; c < 37; c++){	
					CM[m][h]+=obj.HGCDigi_adc[222*m+h*37+c]-Pedestals[m][h*37+c];
				}
				CM[m][h]=CM[m][h]/37;
				for (int c = 0; c < 37; c++){	
					h_ADC_CM[m][h*37+c]->Fill(obj.HGCDigi_adc[222*m + h*37 + c] - CM[m][h]);
				}	
			}
        }
    }
 


    // Create Pedestals graphs and draw them
    TGraphErrors *gE[6];
    for (int m = 0; m < 6; m++) {
        std::cout << "Creating graph for module " << m << "..." << std::endl;
        gE[m] = new TGraphErrors(222, ch_number, Pedestals[m], nullptr , Total_Noise[m]);
        sprintf(title, "module%i", m );
        gE[m]->SetTitle(title);
        gE[m]->SetMarkerColor(4);
        gE[m]->SetMarkerSize(0.2);
        gE[m]->SetMarkerStyle(21);
		gE[m]->SetLineStyle(0);
		gE[m]->Write(title);
    }


    f1->Close();

    /* Cleanup histograms
	std::cout << "Starting cleanup of histograms" << std::endl;
	for (int i = 0; i < 6; ++i) {
    	for (int j = 0; j < 222; ++j) {
        	if (h_ADC[i][j] != nullptr) { // Only delete if pointer is non-null
            	delete h_ADC[i][j];
            	h_ADC[i][j] = nullptr;
        	}
			else {
			std::cout<<"module "<<i<<" channel "<<j<<" is null!"<<std::endl;
			}
    	}
	}
*/
	std::cout << "Processing complete. Results saved to subCM_PedestalsGraphs.root." << std::endl;	

	//Analysis Section
	
	TH1F *fixed_h_ADC[6][222];
	int int_Pedestals[6][222];
 	//configure the fixed_h_ADC
    for (int mod = 0; mod < 6; ++mod) {
	   	for (int ch = 0; ch < 222; ++ch) {
			sprintf(name,"fixed_mod_%i_ch_%i", mod , ch);
            sprintf(title,"fixed_mod_%i_ch_%i", mod , ch);
            fixed_h_ADC[mod][ch] = new TH1F(name, title, 40, -20, 20);
            fixed_h_ADC[mod][ch]->GetXaxis()->SetTitle("Relative_ADC");
            fixed_h_ADC[mod][ch]->GetYaxis()->SetTitle("Events");
            fixed_h_ADC[mod][ch]->SetStats(0);
    //Setting bin contents for new Histogram
	        	for (int bin = 1; bin <= 40; ++bin){
					fixed_h_ADC[mod][ch]->SetBinContent(bin,h_ADC[mod][ch]->GetBinContent(int_Pedestals[mod][ch]+bin-20));
            }
        }
    }

	//Fitting
	TF1 *fitFunc;
    TCanvas *c1;
    char new_name[100];
    double fitted_mean[6][222]; double fitted_sigma[6][222];
    for (int mod = 0; mod < 6; ++mod) {
        for (int ch = 0; ch < 222; ++ch) {
            c1 = new TCanvas("c1","Fitted Histograms",800,600);
            fixed_h_ADC[mod][ch]->SetLineColor(kBlack);
            fixed_h_ADC[mod][ch]->SetMarkerStyle(20);
            fixed_h_ADC[mod][ch]->Draw("E");
            gPad->Modified();
            gPad->Update();

			fixed_h_ADC[mod][ch]->Fit("gaus","","",-20,20);
            fitFunc = fixed_h_ADC[mod][ch]->GetFunction("gaus");
            fitFunc->SetLineColor(kBlue);
            fitFunc->SetLineWidth(2);
            fitted_mean[mod][ch] = fitFunc->GetParameter(1);
            fitted_sigma[mod][ch] = fitFunc->GetParameter(2);
            gPad->Modified();
            gPad->Update();
            TPaveText *pt = new TPaveText(0.6, 0.6, 0.9, 0.8, "NDC");
            pt->AddText(Form("Chi2/ndof = %.2f", fitFunc->GetChisquare() / fitFunc->GetNDF()));
            pt->AddText(Form("#mu = %.2f", fitted_mean[mod][ch]));
            pt->AddText(Form("#sigma = %.2f", fitted_sigma[mod][ch]));
            pt->Draw("same");
            gPad->Modified();
            gPad->Update();
            sprintf(s: new_name,format: "Fitted_Histogram_mod_%i_ch_%i",mod,ch);
            c1->Write(new_name);  // Give a unique name for each saved canvas
        }
    }
    outputFile->Close();
	return 0;
}
