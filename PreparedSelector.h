//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Sep 10 04:02:31 2018 by ROOT version 6.14/04
// from TChain prepared/
//////////////////////////////////////////////////////////

#ifndef PreparedSelector_h
#define PreparedSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector


class PreparedSelector : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Int_t> mh = {fReader, "mh"};
   TTreeReaderValue<Float_t> cent = {fReader, "cent"};
   TTreeReaderArray<Float_t> p = {fReader, "p"};
   TTreeReaderArray<Float_t> the0 = {fReader, "the0"};
   TTreeReaderArray<Float_t> phi0 = {fReader, "phi0"};
   TTreeReaderArray<Float_t> sqM = {fReader, "sqM"};
   TTreeReaderArray<Float_t> beta = {fReader, "beta"};
   TTreeReaderArray<Short_t> dcarm = {fReader, "dcarm"};
   TTreeReaderArray<Short_t> charge = {fReader, "charge"};


   PreparedSelector(TTree * /*tree*/ =0) { }
   virtual ~PreparedSelector() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(PreparedSelector,0);

   static  Float_t minSqPion, maxSqPion, minSqProt, maxSqProt;
   static const Int_t size = 80;

   TProofOutputFile *pof;
   TFile *outputFile;  
   TTree *protons, *pions;
   Int_t protMh;
   Float_t protCent;
   Float_t protP[size], protThe[size], protPhi[size], protBeta[size]; 
   Short_t protArm[size];
   Int_t pionMh;
   Float_t pionCent;
   Float_t pionP[size], pionThe[size], pionPhi[size], pionBeta[size]; 
   Short_t pionArm[size];
     
};

#endif

#ifdef PreparedSelector_cxx
void PreparedSelector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t PreparedSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef PreparedSelector_cxx
