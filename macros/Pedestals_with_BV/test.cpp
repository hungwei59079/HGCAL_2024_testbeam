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

int main(){
	TFile *f1;
   	TTree *t1;
	double Pedestals[6][222], Total_Noise[6][222], Pedestals_Error[6][222];
	double ch_number[222];
	int n_events;

    f1 = TFile::Open("/data1/SepTB2024/Relay1727206089/d560f64b-e3dd-1ffa-98b4-85deaaaa1310/prompt/NANO_1727206092_1.root");
	f1->ls();
    f1->GetObject("Events",t1);
	test_beam obj;
	//test_beam obj = test_beam(t1);
	obj = test_beam(t1);
	t1->Print();

	t1->GetEntry();
	return 0;
}

