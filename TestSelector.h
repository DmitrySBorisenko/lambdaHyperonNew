//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Sep  7 13:07:03 2018 by ROOT version 6.14/04
// from TTree mtree/Hadron EMC + TOF tree
// found on file: 310723.root
//////////////////////////////////////////////////////////

#ifndef TestSelector_h
#define TestSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector
// #include "NamedBins.h"


class TestSelector : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Int_t> run = {fReader, "run"};
   TTreeReaderValue<Float_t> bbcz = {fReader, "bbcz"};
   TTreeReaderValue<Float_t> cent = {fReader, "cent"};
   TTreeReaderValue<Int_t> rh = {fReader, "rh"};
   TTreeReaderArray<Float_t> phir = {fReader, "phir"};
   TTreeReaderArray<Float_t> time = {fReader, "time"};
   TTreeReaderArray<Float_t> qr0 = {fReader, "qr0"};
   TTreeReaderArray<Float_t> etar = {fReader, "etar"};
   TTreeReaderArray<Short_t> armr = {fReader, "armr"};
   TTreeReaderArray<Short_t> ring = {fReader, "ring"};
   TTreeReaderArray<Int_t> chid = {fReader, "chid"};
   TTreeReaderValue<Int_t> mh = {fReader, "mh"};
   TTreeReaderArray<Float_t> alpha = {fReader, "alpha"};
   TTreeReaderArray<Short_t> dcarm = {fReader, "dcarm"};
   TTreeReaderArray<Float_t> p = {fReader, "p"};
   TTreeReaderArray<Short_t> charge = {fReader, "charge"};
   TTreeReaderArray<Float_t> phi0 = {fReader, "phi0"};
   TTreeReaderArray<Float_t> the0 = {fReader, "the0"};
   TTreeReaderArray<Float_t> phi = {fReader, "phi"};
   TTreeReaderArray<Float_t> ecore = {fReader, "ecore"};
   TTreeReaderArray<Float_t> plemc = {fReader, "plemc"};
   TTreeReaderArray<Float_t> ecent = {fReader, "ecent"};
   TTreeReaderArray<Float_t> temc = {fReader, "temc"};
   TTreeReaderArray<Float_t> temcpi = {fReader, "temcpi"};
   TTreeReaderArray<Float_t> temcp = {fReader, "temcp"};
   TTreeReaderArray<Float_t> temck = {fReader, "temck"};
   TTreeReaderArray<Short_t> sect = {fReader, "sect"};
   TTreeReaderArray<Float_t> isPiemc = {fReader, "isPiemc"};
   TTreeReaderArray<Float_t> isPemc = {fReader, "isPemc"};
   TTreeReaderArray<Float_t> isKemc = {fReader, "isKemc"};
   TTreeReaderArray<Int_t> idtwr = {fReader, "idtwr"};
   TTreeReaderArray<Float_t> sigtof = {fReader, "sigtof"};
   TTreeReaderArray<Float_t> sigpc3 = {fReader, "sigpc3"};
   TTreeReaderArray<Float_t> sigemc = {fReader, "sigemc"};
   TTreeReaderArray<Float_t> res = {fReader, "res"};
   TTreeReaderArray<Float_t> ttof = {fReader, "ttof"};
   TTreeReaderArray<Int_t> slat = {fReader, "slat"};
   TTreeReaderArray<Float_t> pltof = {fReader, "pltof"};
   TTreeReaderArray<Float_t> etof = {fReader, "etof"};
   TTreeReaderArray<Float_t> isPi = {fReader, "isPi"};
   TTreeReaderArray<Float_t> isP = {fReader, "isP"};
   TTreeReaderArray<Float_t> isK = {fReader, "isK"};


   TestSelector(TTree * /*tree*/ =0) { }
   virtual ~TestSelector() { }
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

   ClassDef(TestSelector,0);

   Short_t mode = 0;
   const Int_t binsPerUnitPt = 1000 / (2.5 - 0.3);
   static const Int_t size = 80;
   TH1F *pT, *sqMassHistsWest[3], *sqMassHistsEast[3], *forBins;
   Int_t lowBin = 2, highBin = 100;
   TProofOutputFile *proofFile;
   TFile *outputFile;  
   TTree *prepared;
   Int_t prMh;
   Float_t prCent;
   Float_t prP[size], prThe[size], prPhi[size], prBeta[size], prSqM[size]; 
   Short_t prCh[size], prArm[size]; 

   Int_t getEquiBin(TH1F *h, Int_t l, Int_t r, Float_t coef = 1);
   void setMode(const char* str);
};

#endif

#ifdef TestSelector_cxx
void TestSelector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t TestSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef TestSelector_cxx
