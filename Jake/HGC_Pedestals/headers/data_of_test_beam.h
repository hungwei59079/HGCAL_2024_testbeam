//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Oct 21 11:19:12 2024 by ROOT version 6.24/06
// from TTree Events/Events
// found on file: NANO_1726225194_1.root
//////////////////////////////////////////////////////////

#ifndef data_of_test_beam_h
#define data_of_test_beam_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class data_of_test_beam {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   UInt_t          run;
   UInt_t          luminosityBlock;
   ULong64_t       event;
   UInt_t          bunchCrossing;
   UInt_t          orbitNumber;
   Int_t           nHGCDigi;
   Bool_t          HGCDigi_isSiPM[1332];   //[nHGCDigi]
   UShort_t        HGCDigi_tctp[1332];   //[nHGCDigi]
   UShort_t        HGCDigi_adc[1332];   //[nHGCDigi]
   UShort_t        HGCDigi_adcm1[1332];   //[nHGCDigi]
   UShort_t        HGCDigi_tot[1332];   //[nHGCDigi]
   UShort_t        HGCDigi_toa[1332];   //[nHGCDigi]
   UShort_t        HGCDigi_cm[1332];   //[nHGCDigi]
   UShort_t        HGCDigi_flags[1332];   //[nHGCDigi]
   UShort_t        HGCDigi_channel[1332];   //[nHGCDigi]
   UShort_t        HGCDigi_fedId[1332];   //[nHGCDigi]
   UShort_t        HGCDigi_fedReadoutSeq[1332];   //[nHGCDigi]
   Int_t           HGCDigi_chType[1332];   //[nHGCDigi]
   Int_t           HGCDigi_modI1[1332];   //[nHGCDigi]
   Int_t           HGCDigi_modI2[1332];   //[nHGCDigi]
   Int_t           HGCDigi_chI1[1332];   //[nHGCDigi]
   Int_t           HGCDigi_chI2[1332];   //[nHGCDigi]
   Int_t           nHGCHit;
   Bool_t          HGCHit_zSide[1332];   //[nHGCHit]
   UShort_t        HGCHit_flags[1332];   //[nHGCHit]
   Int_t           HGCHit_layer[1332];   //[nHGCHit]
   Float_t         HGCHit_x[1332];   //[nHGCHit]
   Float_t         HGCHit_y[1332];   //[nHGCHit]
   Double_t        HGCHit_energy[1332];   //[nHGCHit]
   Double_t        HGCHit_time[1332];   //[nHGCHit]
   UInt_t          HGCMetaData_trigTime;
   UInt_t          HGCMetaData_trigWidth;
   UInt_t          HGCMetaData_trigType;
   UInt_t          HGCMetaData_trigSubType;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_luminosityBlock;   //!
   TBranch        *b_event;   //!
   TBranch        *b_bunchCrossing;   //!
   TBranch        *b_orbitNumber;   //!
   TBranch        *b_nHGCDigi;   //!
   TBranch        *b_HGCDigi_isSiPM;   //!
   TBranch        *b_HGCDigi_tctp;   //!
   TBranch        *b_HGCDigi_adc;   //!
   TBranch        *b_HGCDigi_adcm1;   //!
   TBranch        *b_HGCDigi_tot;   //!
   TBranch        *b_HGCDigi_toa;   //!
   TBranch        *b_HGCDigi_cm;   //!
   TBranch        *b_HGCDigi_flags;   //!
   TBranch        *b_HGCDigi_channel;   //!
   TBranch        *b_HGCDigi_fedId;   //!
   TBranch        *b_HGCDigi_fedReadoutSeq;   //!
   TBranch        *b_HGCDigi_chType;   //!
   TBranch        *b_HGCDigi_modI1;   //!
   TBranch        *b_HGCDigi_modI2;   //!
   TBranch        *b_HGCDigi_chI1;   //!
   TBranch        *b_HGCDigi_chI2;   //!
   TBranch        *b_nHGCHit;   //!
   TBranch        *b_HGCHit_zSide;   //!
   TBranch        *b_HGCHit_flags;   //!
   TBranch        *b_HGCHit_layer;   //!
   TBranch        *b_HGCHit_x;   //!
   TBranch        *b_HGCHit_y;   //!
   TBranch        *b_HGCHit_energy;   //!
   TBranch        *b_HGCHit_time;   //!
   TBranch        *b_HGCMetaData_trigTime;   //!
   TBranch        *b_HGCMetaData_trigWidth;   //!
   TBranch        *b_HGCMetaData_trigType;   //!
   TBranch        *b_HGCMetaData_trigSubType;   //!

   data_of_test_beam(TTree *tree=0);
   virtual ~data_of_test_beam();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef data_of_test_beam_cxx
data_of_test_beam::data_of_test_beam(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("NANO_1726225194_1.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("NANO_1726225194_1.root");
      }
      f->GetObject("Events",tree);

   }
   Init(tree);
}

data_of_test_beam::~data_of_test_beam()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t data_of_test_beam::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t data_of_test_beam::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void data_of_test_beam::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("luminosityBlock", &luminosityBlock, &b_luminosityBlock);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("bunchCrossing", &bunchCrossing, &b_bunchCrossing);
   fChain->SetBranchAddress("orbitNumber", &orbitNumber, &b_orbitNumber);
   fChain->SetBranchAddress("nHGCDigi", &nHGCDigi, &b_nHGCDigi);
   fChain->SetBranchAddress("HGCDigi_isSiPM", HGCDigi_isSiPM, &b_HGCDigi_isSiPM);
   fChain->SetBranchAddress("HGCDigi_tctp", HGCDigi_tctp, &b_HGCDigi_tctp);
   fChain->SetBranchAddress("HGCDigi_adc", HGCDigi_adc, &b_HGCDigi_adc);
   fChain->SetBranchAddress("HGCDigi_adcm1", HGCDigi_adcm1, &b_HGCDigi_adcm1);
   fChain->SetBranchAddress("HGCDigi_tot", HGCDigi_tot, &b_HGCDigi_tot);
   fChain->SetBranchAddress("HGCDigi_toa", HGCDigi_toa, &b_HGCDigi_toa);
   fChain->SetBranchAddress("HGCDigi_cm", HGCDigi_cm, &b_HGCDigi_cm);
   fChain->SetBranchAddress("HGCDigi_flags", HGCDigi_flags, &b_HGCDigi_flags);
   fChain->SetBranchAddress("HGCDigi_channel", HGCDigi_channel, &b_HGCDigi_channel);
   fChain->SetBranchAddress("HGCDigi_fedId", HGCDigi_fedId, &b_HGCDigi_fedId);
   fChain->SetBranchAddress("HGCDigi_fedReadoutSeq", HGCDigi_fedReadoutSeq, &b_HGCDigi_fedReadoutSeq);
   fChain->SetBranchAddress("HGCDigi_chType", HGCDigi_chType, &b_HGCDigi_chType);
   fChain->SetBranchAddress("HGCDigi_modI1", HGCDigi_modI1, &b_HGCDigi_modI1);
   fChain->SetBranchAddress("HGCDigi_modI2", HGCDigi_modI2, &b_HGCDigi_modI2);
   fChain->SetBranchAddress("HGCDigi_chI1", HGCDigi_chI1, &b_HGCDigi_chI1);
   fChain->SetBranchAddress("HGCDigi_chI2", HGCDigi_chI2, &b_HGCDigi_chI2);
   fChain->SetBranchAddress("nHGCHit", &nHGCHit, &b_nHGCHit);
   fChain->SetBranchAddress("HGCHit_zSide", HGCHit_zSide, &b_HGCHit_zSide);
   fChain->SetBranchAddress("HGCHit_flags", HGCHit_flags, &b_HGCHit_flags);
   fChain->SetBranchAddress("HGCHit_layer", HGCHit_layer, &b_HGCHit_layer);
   fChain->SetBranchAddress("HGCHit_x", HGCHit_x, &b_HGCHit_x);
   fChain->SetBranchAddress("HGCHit_y", HGCHit_y, &b_HGCHit_y);
   fChain->SetBranchAddress("HGCHit_energy", HGCHit_energy, &b_HGCHit_energy);
   fChain->SetBranchAddress("HGCHit_time", HGCHit_time, &b_HGCHit_time);
   fChain->SetBranchAddress("HGCMetaData_trigTime", &HGCMetaData_trigTime, &b_HGCMetaData_trigTime);
   fChain->SetBranchAddress("HGCMetaData_trigWidth", &HGCMetaData_trigWidth, &b_HGCMetaData_trigWidth);
   fChain->SetBranchAddress("HGCMetaData_trigType", &HGCMetaData_trigType, &b_HGCMetaData_trigType);
   fChain->SetBranchAddress("HGCMetaData_trigSubType", &HGCMetaData_trigSubType, &b_HGCMetaData_trigSubType);
   Notify();
}

Bool_t data_of_test_beam::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void data_of_test_beam::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t data_of_test_beam::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef data_of_test_beam_cxx
