#define TestSelector_cxx
// The class definition in TestSelector.h has been generated automatically
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
// root> T->Process("TestSelector.C")
// root> T->Process("TestSelector.C","some options")
// root> T->Process("TestSelector.C+")
//


#include "TestSelector.h"
#include <TH2.h>
#include <TStyle.h>

void TestSelector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   

   TFile ff("interConnect.root");
   TNamed* modeEntry = (TNamed*)ff.Get("modeEntry");
   ff.Close();

   setMode(modeEntry->GetTitle());
   Info("BEGIN", "getted mode: %s", modeEntry->GetTitle());
   if (mode == 1) {
      TFile ff("/home/profile4me/OneMoreDir/interConnect.root");
      forBins = (TH1F*)ff.Get("forBins");
      lowBin = forBins->GetBinContent(1);
      highBin = forBins->GetBinContent(2);
      ff.Close();
      Info("BEGIN", "lowBin = %d, highBin = %d", lowBin, highBin);
   }
}

void TestSelector::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   
   TFile ff("/home/profile4me/OneMoreDir/interConnect.root");
   TNamed* modeEntry = (TNamed*)ff.Get("modeEntry");
   ff.Close();


   setMode(modeEntry->GetTitle());

   switch (mode) {
      case 0: {   
         pT = new TH1F("pT", "", 1000, 0.3, 2.5);
         fOutput->Add(pT);
         break;
      }
      case 1: {
         for (int i=0; i<3; i++) {
            sqMassHistsWest[i] = new TH1F(TString("sqMassHistWest") + TString::Itoa(i, 10), "", 1000, 0, 1);
            sqMassHistsEast[i] = new TH1F(TString("sqMassHistEast") + TString::Itoa(i, 10), "", 1000, 0, 1);
            fOutput->Add(sqMassHistsWest[i]);
            fOutput->Add(sqMassHistsEast[i]);
         }
         TFile ff("/home/profile4me/OneMoreDir/interConnect.root");
         forBins = (TH1F*)ff.Get("forBins");
         lowBin = forBins->GetBinContent(1);
         highBin = forBins->GetBinContent(2);
         ff.Close();

         proofFile = new TProofOutputFile("/home/profile4me/OneMoreDir/prepared.root");
         outputFile = proofFile->OpenFile("RECREATE");
         prepared = new TTree("prepared", "");
         prepared->Branch("mh", &prMh, "mh/I");
         prepared->Branch("cent", &prCent, "cent/F");
         prepared->Branch("p", prP, "p[mh]/F");
         prepared->Branch("the0", prThe, "the0[mh]/F");
         prepared->Branch("phi0", prPhi, "phi0[mh]/F");
         prepared->Branch("sqM", prSqM, "sqM[mh]/F");
         prepared->Branch("beta", prBeta, "beta[mh]/F");
         prepared->Branch("dcarm", prArm, "dcarm[mh]/S");
         prepared->Branch("charge", prCh, "charge[mh]/S");

         break;
      }
   }
}

Bool_t TestSelector::Process(Long64_t entry)
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
   switch (mode) {
      case 0: {      
         //going through particles in event
         for (Int_t i=0; i<*mh.Get(); i++) {
            TVector3 v3(1,1,1);
            v3.SetMag(p.At(i));
            v3.SetTheta(the0.At(i));
            v3.SetPhi(phi0.At(i));
            pT->Fill(v3.Perp());
         }
         break;
      }
  
      case 1: {
         prMh = 0;
         prCent = *cent.Get();
         for (Int_t i=0; i<*mh.Get(); i++) {
            //counting sq mass
            if (pltof.At(i) < 0) continue;
            Float_t beta = pltof.At(i) / ttof.At(i) / TMath::C() * 10000000;
            if (beta > 1) continue;
            Float_t sqM = p.At(i) * p.At(i) * (1.0 / beta / beta - 1);
            //initializing of arrays for writting to prepared tree
            prP[prMh] = p.At(i);
            prThe[prMh] = the0.At(i);
            prPhi[prMh] = phi0.At(i);
            prSqM[prMh] = sqM;
            prBeta[prMh] = beta;
            prArm[prMh] = dcarm.At(i);
            prCh[prMh] = charge.At(i); 
            prMh++;

            //counting perp pT
            TVector3 v3(1,1,1);
            v3.SetMag(p.At(i));
            v3.SetTheta(the0.At(i));
            v3.SetPhi(phi0.At(i));
            // pT->Fill(v3.Perp());
            Int_t pTBin = (v3.Perp() - 0.3) * binsPerUnitPt; 
            //filling of appropriate hist
            if (pTBin < 0) {

            } else if (pTBin < lowBin) {
               if (dcarm.At(i)) {
                  sqMassHistsWest[0]->Fill(sqM);
               } else {
                  sqMassHistsEast[0]->Fill(sqM);
               }
            } else if (pTBin < highBin) {
               if (dcarm.At(i)) {
                  sqMassHistsWest[1]->Fill(sqM);
               } else {
                  sqMassHistsEast[1]->Fill(sqM);
               }
            } else if (pTBin < 1000) {
               if (dcarm.At(i)) {
                  sqMassHistsWest[2]->Fill(sqM);
               } else {
                  sqMassHistsEast[2]->Fill(sqM);
               }
            } else {

            }
         }
         prepared->Fill();
         break;
      }
   }
   return kTRUE;
}

void TestSelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   if (mode == 1) {
      outputFile->cd();
      prepared->Write();
      fOutput->Add(proofFile);
      outputFile->Close();
   }
}

void TestSelector::Terminate()
{


   switch (mode) {
      case 0: {
         Info("TERMINATE", "in Terminate");
         pT = (TH1F*)fOutput->FindObject("pT");
         pT->Draw();
         //define appropriate intervals of pT that contain equal amount of particles
         lowBin = getEquiBin(pT, 0, 1000, 2);
         highBin = getEquiBin(pT, lowBin, 1000);
         Info("TERMINATE", "defined bins: lowBin = %d, highBin = %d", lowBin, highBin);
         //writting result to interConnect.root 
         TFile ff("interConnect.root", "UPDATE");
         forBins = new TH1F("forBins", "", 2, 0, 2);
         for (Int_t i=0; i<lowBin; i++) {
            forBins->Fill(0.5);
         }
         for (Int_t i=0; i<highBin; i++) {
            forBins->Fill(1.5);
         }
         forBins->Write();
         ff.Close();
         break;
      }
      case 1: {
         TCanvas *c2 = new TCanvas("c2", "squared mass", 1100, 600);
         c2->Divide(2,2);
         Float_t limits[3][2] = {
            {0.3, 0.3f + 1.0f * lowBin / binsPerUnitPt},
            {0.3f + 1.0f * lowBin / binsPerUnitPt, 0.3f + 1.0f * highBin / binsPerUnitPt},
            {0.3f + 1.0f * highBin / binsPerUnitPt, 2.5}
         };
         for (int i=0; i<3; i++) {
            c2->cd(i+1);
            sqMassHistsWest[i] = (TH1F*)fOutput->FindObject(TString("sqMassHistWest") + TString::Itoa(i, 10));
            sqMassHistsWest[i]->SetLineColor(6);
            sqMassHistsWest[i]->SetXTitle("squared mass, GeV^2");
            sqMassHistsWest[i]->SetYTitle("number of particles");
            sqMassHistsWest[i]->SetTitle(to_string(limits[i][0]).substr(0,4)+TString("<pT<")+to_string(limits[i][1]).substr(0,4));
            sqMassHistsWest[i]->Draw();

            sqMassHistsEast[i] = (TH1F*)fOutput->FindObject(TString("sqMassHistEast") + TString::Itoa(i, 10));
            sqMassHistsEast[i]->SetLineColor(4);
            sqMassHistsEast[i]->Draw("same");
         }
         TF1 *pionFit = new TF1("pionFit", "gaus", 0, 0.04);
         TF1 *protFit = new TF1("protFit", "gaus", 0.8, 1);
         sqMassHistsWest[2]->Fit(pionFit, "R");
         sqMassHistsWest[2]->Fit(protFit, "R+");
         TLegend *legend = new TLegend(0.1,0.7,0.48,0.9);
         legend->SetHeader("Header","C");             // option "C" allows to center the header
         legend->AddEntry("sqMassHistsWest0","West amr","l");
         legend->AddEntry("sqMassHistsEast0","East arm","l");
         legend->Draw();

         c2->SaveAs("images/sqMass_in_three_PtRanges.png");
         Float_t minPion = pionFit->GetParameter(1) - pionFit->GetParameter(2) * 2;
         if (minPion < 0) minPion = 0;
         Float_t maxPion = pionFit->GetParameter(1) + pionFit->GetParameter(2) * 2;
         Float_t minProt = protFit->GetParameter(1) - pionFit->GetParameter(2) * 2;
         Float_t maxProt = protFit->GetParameter(1) + pionFit->GetParameter(2) * 2;
         Info("TERMINATE", "minSqPionMass = %f, maxSqPionMass = %f", minPion, maxPion);
         Info("TERMINATE", "minSqProtMass = %f, maxSqProtMass = %f", minProt, maxProt);
         break;
      }
      case 2: {

         break;
      }
   }


   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

}

Int_t TestSelector::getEquiBin(TH1F *h, Int_t l, Int_t r, Float_t coef) {
   Int_t res = (l + r) /2, delta = (r-l)/2;
   Long64_t lSum = h->Integral(l, res), rSum = h->Integral(res, r), delta1, delta2 ;
   while (delta > 1) {
      Info("TERMINATE", "lSum = %lld, rSum = %lld, delta = %d", lSum, rSum, delta);
      delta /=2; 
      lSum = h->Integral(l, res -delta);
      rSum = h->Integral(res-delta, r);
      delta1 = TMath::Abs(rSum / coef - lSum);
      lSum = h->Integral(l, res+delta);
      rSum = h->Integral(res + delta, r);
      delta2 = TMath::Abs(rSum / coef - lSum);
      res = (delta1 < delta2) ? res - delta : res + delta;
   }
   return res;
}

void TestSelector::setMode(const char* str) {
   if (!strcmp(str, "mode0")) {
      mode = 0;
   } else if (!strcmp(str, "mode1")) {
      mode = 1;
   }
}