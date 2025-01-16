#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <experimental/filesystem>

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TLorentzVector.h"
// Include custom header
#include "../../headers/test_beam.h"

namespace fs = std::experimental::filesystem;

int main() {
    std::string root_directory = "/data1/SepTB2024"; // Base directory to search
    std::string target_prefix_1 = "NANO_172716";
    std::string target_prefix_2 = "NANO_172717";
    std::string target_prefix_3 = "NANO_172718";
    std::string target_prefix_4 = "NANO_17272";
    std::vector<std::string> file_paths;

    // Iterate through the directory structure recursively
    for (const auto& entry : fs::recursive_directory_iterator(root_directory)) {
        if (fs::is_regular_file(entry)) { // Use the free-standing function
            std::string filename = entry.path().filename().string();

            // Check for prefix and ".root" suffix
            if (filename.find(target_prefix_1) == 0 && 
                filename.size() >= 5 && filename.substr(filename.size() - 5) == ".root") {
                file_paths.push_back(entry.path().string());
            }
            if (filename.find(target_prefix_2) == 0 && 
                filename.size() >= 5 && filename.substr(filename.size() - 5) == ".root") {
                file_paths.push_back(entry.path().string());
            }
            if (filename.find(target_prefix_3) == 0 && 
                filename.size() >= 5 && filename.substr(filename.size() - 5) == ".root") {
                file_paths.push_back(entry.path().string());
            }
            if (filename.find(target_prefix_4) == 0 && 
                filename.size() >= 5 && filename.substr(filename.size() - 5) == ".root") {
                file_paths.push_back(entry.path().string());
            }

        }
    }

	std::vector<int> bias = {-130,-300,-400,-500,-600,-700};

	TFile *f1 = TFile::Open("/data1/SepTB2024/Relay1727169610/7ba35975-cd2e-1163-858c-2e6e57ed22d0/prompt/NANO_1727169613_1.root");
	TTree *t1;
	f1->GetObject("Events",t1);
	test_beam obj = test_beam(t1);
	double Pedestals[8][222], Total_Noise[6][8][222], Pedestals_Error[8][222];
	double ch_number[222];
	int n_events;
	TH1F *h_ADC[8][222] = {};
	TGraphErrors *gE[6][8];
	TGraphErrors *B_to_P[8][222] = {};
	TGraph *B_to_N[8][222] = {};
	int file_number=0;
	char name[100], title[100];
	for (int m = 0; m < 8; m++) {
		std::cout << "Initializing histograms for module " << m + 1 << "..." << std::endl;
		for (int i = 0; i < 222; i++) {
			sprintf(name,"mod_%i_ch_%i", m, i);
			sprintf(title,"mod_%i_ch_%i", m, i);
			h_ADC[m][i] = new TH1F(name, title, 300, 0, 300);
			h_ADC[m][i]->GetXaxis()->SetTitle("ADC");
			h_ADC[m][i]->GetYaxis()->SetTitle("Events");

		}
	}

	//loop over files starts here
    for (const auto& filepath : file_paths) {
		std::cout << "Analyzing file: " << filepath << std::endl;
		f1 = TFile::Open(filepath.c_str());
		f1->ls();
		f1->GetObject("Events",t1);
		test_beam *obj = new test_beam(t1);
		t1->SetBranchStatus("*",0);
		t1->SetBranchStatus("HGCDigi_adc",1);
		
		n_events = t1->GetEntries();

		std::cout<<"number of events: "<<n_events<<std::endl;
	//Filling histograms for calculations:
		for (int i = 0; i < n_events; i++) {
	 		if (i % 1000 == 0) std::cout << "Processing event " << i << " / " << n_events << std::endl;
			//std::cout<<"test_1"<<std::endl;
        	t1->GetEntry(i);
			//std::cout<<"test_2"<<std::endl;
			for (int m = 0; m < 8; m++) {
				for (int c = 0; c < 222; c++) {
					if (m == 7 && c >=111) {
						continue;
					}
					else {
		            	h_ADC[m][c]->Fill(obj->HGCDigi_adc[222 * m + c]);
            	    }
            	}
			}
    	}
	// Calculate and write pedestals and noise
		for (int m = 0; m < 8; m++) {
			for (int c = 0; c < 222; c++) {
				if (m == 7 && c >=111){
					Pedestals[m][c] = 0;
					Total_Noise[file_number][m][c] = 0;
					Pedestals_Error[m][c] = 0;
					continue;
				}
				ch_number[c] = c * 1.0 + 1.0;
				Pedestals[m][c] = h_ADC[m][c]->GetMean();
				Total_Noise[file_number][m][c] = h_ADC[m][c]->GetStdDev();
				Pedestals_Error[m][c] = h_ADC[m][c]->GetMeanError();
			}
		}

		f1->Close();
	//Record Pedestals in Tgraphs
		for (int m = 0; m < 8; m++) {
			std::cout << "Creating graph for module " << m << "..." << std::endl;
			gE[file_number][m] = new TGraphErrors(222, ch_number, Pedestals[m], nullptr , Pedestals_Error[m]);
			sprintf(title, "Bias:%i; module%i", bias[file_number] ,  m );
			gE[file_number][m]->SetName(title);
			gE[file_number][m]->SetTitle(title);
			gE[file_number][m]->SetMarkerColor(4);
			gE[file_number][m]->SetMarkerSize(0.2);
			gE[file_number][m]->SetMarkerStyle(21);
			gE[file_number][m]->SetLineStyle(0);
		}

		
		file_number++;
		delete obj;
    }
	
	for (int m = 0; m < 8; m++) {
		for (int i = 0; i < 222; i++) {
			B_to_P[m][i] = new TGraphErrors();
			B_to_N[m][i] = new TGraph();
			sprintf(name,"B_to_P_for_mod_%i_ch_%i", m, i);
			sprintf(title,"B_to_P_for_mod_%i_ch_%i", m, i);
			B_to_P[m][i]->SetName(name);
			B_to_P[m][i]->SetTitle(title);
			sprintf(name,"B_to_N_for_mod_%i_ch_%i", m, i);
			sprintf(title,"B_to_N_for_mod_%i_ch_%i", m, i);
			B_to_N[m][i]->SetName(name);
			B_to_N[m][i]->SetTitle(title);
			B_to_P[m][i]->GetXaxis()->SetTitle("Bias Voltage");
			B_to_P[m][i]->GetYaxis()->SetTitle("Pedestals");
			B_to_N[m][i]->GetXaxis()->SetTitle("Bias Voltage");
			B_to_N[m][i]->GetYaxis()->SetTitle("Noise");
		}
	}

	for (int m = 0; m < 8; m++){
		for (int c = 0; c < 222; c++){
			if (222 * m + c >= 1665){
					continue;
				}
			for (int f = 0; f < 6; f++){
				B_to_P[m][c]->SetPoint(f,bias[f],gE[f][m]->GetPointY(c));
				B_to_P[m][c]->SetPointError(f,0,gE[f][m]->GetErrorY(c));
				B_to_N[m][c]->SetPoint(f,bias[f],Total_Noise[f][m][c]);
			}
		}
	}

	TFile *outputFile = TFile::Open("~/HGCAL/2024/root_files/Pedestals_with_BV.root", "RECREATE");
	outputFile->cd();
	for (int f = 0; f < 6; f++){
		for (int m = 0; m < 8; m++){
			gE[f][m]->Write();
		}
	}
	for (int m = 0; m < 8; m++) {
		for (int i = 0; i < 222; i++) {
			B_to_P[m][i]->Write();
			B_to_N[m][i]->Write();
		}
	}
    return 0;
}
