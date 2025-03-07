#include <cstdio>
#include <experimental/filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "TF1.h"
#include "TFile.h"
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
    if (fs::is_regular_file(entry)) { // Use the free-standing function
      std::string filename = entry.path().filename().string();

      // Check for any prefix and ".root" suffix
      for (const auto &prefix : target_prefixes) {
        if (filename.find(prefix) == 0 && filename.size() >= 5 &&
            filename.substr(filename.size() - 5) == ".root") {
          file_paths.push_back(entry.path().string());
          break;
        }
      }
    }
  }

  std::vector<int> bias = {-130, -300, -400, -500, -600, -700};

  TFile *f1 = nullptr;
  TTree *t1 = nullptr;

  double ch_number[222];
  int n_events;
  TH1F *h_TOT[8][222] = {};
  int TOT_events[6][8][222] = {};
  double TOT_mean[6][8][222] = {};
  int file_number = 0;

  char name[100], title[100];
  for (int m = 0; m < 8; m++) {
    std::cout << "Initializing histograms for module " << m + 1 << "..."
              << std::endl;
    for (int i = 0; i < 222; i++) {
      sprintf(name, "mod_%i_ch_%i", m, i);
      sprintf(title, "mod_%i_ch_%i", m, i);
      h_TOT[m][i] = new TH1F(name, title, 1024, 0, 1024);
      h_TOT[m][i]->GetXaxis()->SetTitle("ADC");
      h_TOT[m][i]->GetYaxis()->SetTitle("Events");
    }
  }

  // Print all found file paths
  if (file_paths.empty()) {
    std::cerr << "No matching .root files found in " << root_directory << "!"
              << std::endl;
  } else {
    std::cout << "Found " << file_paths.size()
              << " matching .root files:" << std::endl;
    for (const auto &path : file_paths) {
      std::cout << path << std::endl;
    }
  }

  // loop over files starts here
  for (const auto &filepath : file_paths) {
    std::cout << "Analyzing file: " << filepath << std::endl;

    // if (f1)
    //	std::cout<<"Previous file is open."<<std::endl;
    // f1->Close();  Close previous file if already open
    f1 = TFile::Open(filepath.c_str());
    if (!f1 || f1->IsZombie()) {
      std::cerr << "Error opening file: " << filepath << std::endl;
      continue;
    }

    f1->GetObject("Events", t1);
    if (!t1) {
      std::cerr << "Error: TTree 'Events' not found in " << filepath
                << std::endl;
      continue;
    }

    test_beam *obj = new test_beam(t1);
    t1->SetBranchStatus("*", 0);
    t1->SetBranchStatus("HGCDigi_tot", 1);

    n_events = t1->GetEntries();
    std::cout << "Number of events: " << n_events << std::endl;

    // Filling histograms for calculations:
    for (int i = 0; i < n_events; i++) {
      if (i % 1000 == 0)
        std::cout << "Processing event " << i << " / " << n_events << std::endl;
      t1->GetEntry(i);
      for (int m = 0; m < 8; m++) {
        for (int c = 0; c < 222; c++) {
          if (m == 7 && c >= 111) {
            continue;
          } else if (obj->HGCDigi_tot[222 * m + c] == 0) {
            continue;
          } else {
            h_TOT[m][c]->Fill(obj->HGCDigi_tot[222 * m + c]);
          }
        }
      }
    }

    for (int m = 0; m < 8; m++) {
      for (int c = 0; c < 222; c++) {
        if (m == 7 && c >= 111) {
          continue;
        } else {
          TOT_events[file_number][m][c] = h_TOT[m][c]->GetEntries();
          TOT_mean[file_number][m][c] = h_TOT[m][c]->GetMean();
          h_TOT[m][c]->Reset();
        }
      }
    }

    f1->Close();
    file_number++;
    delete obj;
  }

  // Save results into a ROOT file
  TFile *outputFile = TFile::Open(
      "/data1/home_ss2024/ss2024_05/HGCAL/2024/root_files/TOT_analysis.root",
      "RECREATE");
  if (!outputFile || outputFile->IsZombie()) {
    std::cerr << "Error: Could not create output ROOT file." << std::endl;
    return 1;
  }

  TTree *outTree = new TTree("TOTTree", "Tree storing TOT analysis data");

  int event_TOT_events[8 * 222];
  double event_TOT_mean[8 * 222];

  outTree->Branch("TOT_events", event_TOT_events, "TOT_events[1776]/I");
  outTree->Branch("TOT_mean", event_TOT_mean, "TOT_mean[1776]/D");

  for (int f = 0; f < file_number; f++) {
    for (int m = 0; m < 8; m++) {
      for (int c = 0; c < 222; c++) {
        event_TOT_events[m * 222 + c] = TOT_events[f][m][c];
        event_TOT_mean[m * 222 + c] = TOT_mean[f][m][c];
      }
    }
    outTree->Fill();
  }

  outTree->Write();
  outputFile->Close();

  std::cout << "Analysis results saved to TOT_analysis.root" << std::endl;
  return 0;
}
