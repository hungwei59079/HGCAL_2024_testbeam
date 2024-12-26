#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include "TObject.h"
#include <cmath>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TLorentzVector.h"
#include "TGraphErrors.h"
#include "headers/data_of_Relay1726225188.h"

void PedestalsAnalysis(int module_number, int channel_number){
	gROOT->SetBatch(kTRUE);
    TFile *f1 = TFile::Open("subCM_PedestalsGraphs.root");
    TH1F *h_ADC[6][222]; // Array to hold the histograms
	TH1F *fixed_h_ADC[6][222];
	float Pedestals[6][222];
	int int_Pedestals[6][222];
    // Loop over modules and channels
    for (int mod = 0; mod < 6; ++mod) {
        for (int ch = 0; ch < 222; ++ch) {
            // Construct the key name
            std::string keyName = "subCM_mod_" + std::to_string(mod) + "_ch_" + std::to_string(ch);

            // Retrieve the histogram and store it in the array
            f1->GetObject(keyName.c_str(), h_ADC[mod][ch]);
			Pedestals[mod][ch] = h_ADC[mod][ch]->GetMean();
			int_Pedestals[mod][ch] = round(Pedestals[mod][ch]);

            // Optional: Check if retrieval was successful
            if (h_ADC[mod][ch] == nullptr) {
                std::cerr << "Histogram " << keyName << " not found in file." << std::endl;
            }
        }
    }


	
	//configure the fixed_h_ADC
	char name[100]; char title[100];
	for (int mod = 0; mod < 6; ++mod) {
        for (int ch = 0; ch < 222; ++ch) {
			sprintf(name, "fixed_mod_%i_ch_%i", mod , ch);
            sprintf(title, "fixed_mod_%i_ch_%i", mod , ch);
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
	
	TFile *outputFile = TFile::Open("Fitted_with_subCM.root","RECREATE");

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
			sprintf(new_name,"Fitted_Histogram_mod_%i_ch_%i",mod,ch);
			c1->Write(new_name);  // Give a unique name for each saved canvas
		}
	}
	outputFile->Close();
	//fixed_h_ADC[module_number][channel_number]->Draw();
}

