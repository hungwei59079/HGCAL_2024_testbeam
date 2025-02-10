#include <cstdio>
#include <experimental/filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "TF1.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TTree.h"
// Include custom header
#include "../../headers/test_beam.h"

namespace fs = std::experimental::filesystem;

int main() {
  std::string root_directory = "/data1/SepTB2024"; // Base directory to search
  std::vector<std::string> target_prefixes = {"NANO_172716", "NANO_172717",
                                              "NANO_172718", "NANO_172720"};
  std::vector<std::string> file_paths;

  // Iterate through the directory structure recursively
  for (const auto &entry : fs::recursive_directory_iterator(root_directory)) {
    if (fs::is_regular_file(entry)) {
      std::string filename = entry.path().filename().string();

      for (const auto &prefix : target_prefixes) {
        if (filename.find(prefix) == 0 && filename.size() >= 5 &&
            filename.substr(filename.size() - 5) == ".root") {
          file_paths.push_back(entry.path().string());
        }
      }
    }
  }

  TFile *f1 = nullptr;
  TTree *t1 = nullptr;
  TH1F *h_ADC_1[6] = {nullptr};
  TH1F *h_ADC_2[6] = {nullptr};
  int file_number = 0;
  char name[100];
  char title[100];

  // Initialize histograms
  for (int i = 0; i < 6; i++) {
    sprintf(name, "Bias_%i_mod_0_ch_74", i);
    sprintf(title, "Bias = %i; mod_0_ch_74", i);
    h_ADC_1[i] = new TH1F(name, title, 300, 0, 300);
    h_ADC_1[i]->GetXaxis()->SetTitle("ADC");
    h_ADC_1[i]->GetYaxis()->SetTitle("Events");

    sprintf(name, "Bias_%i_mod_5_ch_9", i);
    sprintf(title, "Bias = %i; mod_5_ch_9", i);
    h_ADC_2[i] = new TH1F(name, title, 300, 0, 300);
    h_ADC_2[i]->GetXaxis()->SetTitle("ADC");
    h_ADC_2[i]->GetYaxis()->SetTitle("Events");
  }

  // Process each file
  for (const auto &filepath : file_paths) {
    if (file_number >= 6) {
      std::cerr << "Warning: More than 6 files found, ignoring extra files.\n";
      break;
    }

    std::cout << "Opening file: " << filepath << std::endl;
    f1 = TFile::Open(filepath.c_str(), "READ");
    if (!f1 || f1->IsZombie()) {
      std::cerr << "Error: Failed to open file " << filepath << std::endl;
      continue;
    }
	// First histogram (HGCDigi_channel == 74)
	std::cout << "Drawing histogram for HGCDigi_channel == 74, fedReadoutSeq == 0" << std::endl;
	TH1F *h_temp = (TH1F *)gROOT->FindObject("h_temp");
	if (h_temp) delete h_temp;  // Clear any existing histogram

	t1->Draw("HGCDigi_adc>>h_temp", "HGCDigi_channel==74 && HGCDigi_fedReadoutSeq==0", "goff");
	h_temp = (TH1F *)gROOT->FindObject("h_temp");

	if (h_temp) {
  	  	h_ADC_1[file_number] = (TH1F *)h_temp->Clone(Form("h_ADC_1_%d", file_number));
    	h_ADC_1[file_number]->SetDirectory(0);  // Prevent deletion when file closes
    	std::cout << "Histogram h_ADC_1_" << file_number << " cloned with "  << h_ADC_1[file_number]->GetEntries() << " entries." << std::endl;
} else {
   		std::cerr << "Error: Failed to retrieve h_ADC_1_" << file_number << std::endl;
}

// Second histogram (HGCDigi_channel == 9)
	std::cout << "Drawing histogram for HGCDigi_channel == 9, fedReadoutSeq == 5" << std::endl;
	h_temp = (TH1F *)gROOT->FindObject("h_temp");
	if (h_temp) delete h_temp;

	t1->Draw("HGCDigi_adc>>h_temp", "HGCDigi_channel==9 && HGCDigi_fedReadoutSeq==5", "goff");
	h_temp = (TH1F *)gROOT->FindObject("h_temp");

	if (h_temp) {
   		h_ADC_2[file_number] = (TH1F *)h_temp->Clone(Form("h_ADC_2_%d", file_number));
    	h_ADC_2[file_number]->SetDirectory(0);  // Prevent deletion when file closes
    	std::cout << "Histogram h_ADC_2_" << file_number << " cloned with " << h_ADC_2[file_number]->GetEntries() << " entries." << std::endl;
} else {
    	std::cerr << "Error: Failed to retrieve h_ADC_2_" << file_number << std::endl;
}

    	f1->GetObject("Events", t1);
    if (!t1) {
      	std::cerr << "Error: TTree 'Events' not found in " << filepath << std::endl;
     	f1->Close();
      	delete f1;
		continue;
    }


    // Close file
    f1->Close();
    delete f1;
    file_number++;
  }

  // Save histograms to file
  std::cout << "Saving histograms to output file...\n";
  TFile *outputFile = TFile::Open(
      "~/HGCAL/2024/root_files/Pedestals_with_BV_comparison.root", "RECREATE");
  if (!outputFile || outputFile->IsZombie()) {
    std::cerr << "Error: Failed to open output file for writing.\n";
    return 1;
  }

  for (int i = 0; i < file_number; i++) {
    if (h_ADC_1[i]) {
      h_ADC_1[i]->Write();
      std::cout << "Saved histogram: " << h_ADC_1[i]->GetName() << std::endl;
    }
    if (h_ADC_2[i]) {
      h_ADC_2[i]->Write();
      std::cout << "Saved histogram: " << h_ADC_2[i]->GetName() << std::endl;
    }
  }

  outputFile->Close();
  delete outputFile;
  std::cout << "Histograms saved successfully.\n";

  return 0;
}
