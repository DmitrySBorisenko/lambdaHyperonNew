#define PreparedSelector_cxx
// The class definition in PreparedSelector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("PreparedSelector.C")
// root> T->Process("PreparedSelector.C","some options")
// root> T->Process("PreparedSelector.C+")
//


#include "PreparedSelector.h"
#include <TH2.h>
#include <TStyle.h>

void PreparedSelector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
}

void PreparedSelector::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   pof = new TProofOutputFile("/home/profile4me/OneMoreDir/output.root");
   outputFile = pof->OpenFile("RECREATE");
   protons = new TTree("protons", "");
   pions = new TTree("pions", "");
   protons->Branch("mh", &protMh, "mh/I");
   protons->Branch("cent", &protCent, "cent/F");
   protons->Branch("p", protP, "p[mh]/F");
   protons->Branch("the0", protThe, "the0[mh]/F");
   protons->Branch("phi0", protPhi, "phi0[mh]/F");
   protons->Branch("beta", protBeta, "beta[mh]/F");
   protons->Branch("dcarm", protArm, "dcarm[mh]/S");
   pions->Branch("mh", &pionMh, "mh/I");
   pions->Branch("cent", &pionCent, "cent/F");
   pions->Branch("p", pionP, "p[mh]/F");
   pions->Branch("the0", pionThe, "the0[mh]/F");
   pions->Branch("phi0", pionPhi, "phi0[mh]/F");
   pions->Branch("beta", pionBeta, "beta[mh]/F");
   pions->Branch("dcarm", pionArm, "dcarm[mh]/S");
}

Bool_t PreparedSelector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   fReader.SetEntry(entry);

   protMh = pionMh = 0;
   for (Int_t i=0; i<*mh.Get(); i++) {
      if (minSqPion < sqM.At(i) && sqM.At(i) < maxSqPion && charge.At(i) < 0) {
         pionCent = *cent.Get();
         pionP[pionMh] = p.At(i);
         pionThe[pionMh] = the0.At(i);
         pionPhi[pionMh] = phi0.At(i);
         pionBeta[pionMh] = beta.At(i);
         pionArm[pionMh] = dcarm.At(i);
         pionMh++;
      }
      if (minSqProt < sqM.At(i) && sqM.At(i) < maxSqProt && charge.At(i) > 0)  {
         protCent = *cent.Get();
         protP[protMh] = p.At(i);
         protThe[protMh] = the0.At(i);
         protPhi[protMh] = phi0.At(i);
         protBeta[protMh] = beta.At(i);
         protArm[protMh] = dcarm.At(i);
         protMh++;
      }
   }
   protons->Fill();
   pions->Fill();

   return kTRUE;
}

void PreparedSelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

   outputFile->cd();
   protons->Write();
   pions->Write();
   fOutput->Add(pof);
   outputFile->Close();

}

void PreparedSelector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

}

Float_t PreparedSelector::minSqPion = 0;
Float_t PreparedSelector::maxSqPion = 0.045245;
Float_t PreparedSelector::minSqProt = 0.859491f;
Float_t PreparedSelector::maxSqProt = 0.910872f;