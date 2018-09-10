

//	mode=1	make pT hist by data from one input file 
//			and define appropriate tree ranges of pT
const Short_t mode = 1;	
const Short_t filesAmount = 2;
const Float_t protM = 0.93827;
const Float_t pionM = 0.13957;
const Short_t size = 80;

TTree *protons, *pions, *charged;
//hists of inv mass for each centrality range
TH1F *invMassWestLowPt[3], *invMassEastLowPt[3];
TH1F *invMassWestHighPt[3], *invMassEastHighPt[3];
Int_t protMh;
Float_t protCent, protP[size], protThe[size], protPhi[size], protBeta[size]; 
Short_t protArm[size];
Int_t pionMh;
Float_t pionP[size], pionThe[size], pionPhi[size], pionBeta[size]; 
Short_t pionArm[size];

void fillChain(TChain *ch){
	Int_t i = 0;
	const char *dirPath = "/home/profile4me/lambdaIndent/input/";
	TSystemDirectory *dir = new TSystemDirectory("dir", dirPath);
	dir->Print();
	TList *filesList = dir->GetListOfFiles();
	TIter it(filesList);
	TSystemFile *file;
	while ((file =(TSystemFile*)it.Next()) && i<filesAmount) 
	{
		if (file->IsDirectory()) continue;
		cout << "Adding to chain\n";
		ch->Add(TString(dirPath)+TString(file->GetName()).Data()) ;
		i++;
	}
	cout << "Amount of events: " << ch->GetEntries() << endl;
}

//[0,20] - 0, [20,40] - 1, [40,60] - 2
void countInvMass(Short_t centRange, Long64_t nEntries, TTree *protons, TTree *pions) {
	for (Long64_t i=0; i<nEntries; i++) {
   		protons->GetEntry(i);
   		pions->GetEntry(i);
   		if (centRange == 0 && protCent > 20) continue;
   		if (centRange == 1 && (protCent < 20 || protCent > 40)) continue;
   		if (centRange == 2 && (protCent < 40 || protCent > 60)) continue; 
   		for (Int_t j=0; j<protMh; j++) {
   			TVector3 v3_prot(1,1,1);
			v3_prot.SetMag(protP[j]);
			v3_prot.SetTheta(protThe[j]);
			v3_prot.SetPhi(protPhi[j]);
   			for (Int_t k=0; k<pionMh; k++) {
   				if (protArm[j] != pionArm[k]) continue;
				TVector3 v3_pion(1,1,1);
				v3_pion.SetMag(pionP[j]);
				v3_pion.SetTheta(pionThe[j]);
				v3_pion.SetPhi(pionPhi[j]);
				//calculation of inv mass
				Float_t totalMass = pionM + protM;
				TVector3 totalP = v3_pion + v3_prot;
				Float_t invMass = TMath::Sqrt(totalMass*totalMass + totalP.Mag()*totalP.Mag());
				// cout << "invMass: " << invMass << endl;
				// cout << "totalP.Perp: " << totalP.Perp() << endl;
				if (protArm[j]) {
					if (0 < totalP.Perp() && totalP.Perp() < 0.5) {
						// cout << "Filling by " << invMass << endl;
						invMassWestLowPt[centRange]->Fill(invMass);
					} else if (0.5 < totalP.Perp() && totalP.Perp() < 1.5) {
						invMassWestHighPt[centRange]->Fill(invMass);
					}
   				} else {
					if (0 < totalP.Perp() && totalP.Perp() < 0.5) {
						invMassEastLowPt[centRange]->Fill(invMass);
					} else if (0.5 < totalP.Perp() && totalP.Perp() < 1.5) {
						invMassEastHighPt[centRange]->Fill(invMass);
					}
				}
   			}
   		}
   	}
}

void main() {
	switch (mode) {
		//find appropriate ranges for pT
		//and create pT hists
		case 0: {
			//rewrite interConnect.root
			TFile ff("interConnect.root", "RECREATE");
			TNamed *modeEntry = new TNamed("modeEntry", "mode0");
			modeEntry->Write();
			ff.Close();
			
			TProof *proof = TProof::Open("");
			TChain *chain = new TChain("mtree");
			chain->Add("310723.root");
			cout << "Proof counting with " << chain->GetEntries() << " size\n";
			chain->SetProof();
			chain->Process("TestSelector.C");	
			break;
		}

		//make tree "prepared" in file prepared.root,
		//create sq mass hists and fit them
		case 1: {
			//update interConnect.root with new modeEntry
			//and TNamed child having lowBin and highBin field
			TFile ff("interConnect.root", "UPDATE");
			ff.Delete("modeEntry");
			TNamed *modeEntry = new TNamed("modeEntry", "mode1");
			modeEntry->Write();
			ff.Close();
		
			TProof *proof = TProof::Open("");
			TChain *chain = new TChain("mtree");
			fillChain(chain);
			chain->SetProof();
			chain->Process("TestSelector.C");	
			break;
		}

		//make trees "protons" and "pions" in file output.root
		case 2: {
			TProof *proof = TProof::Open("");
			TChain *chain = new TChain("prepared");
			chain->Add("prepared.root");
			chain->SetProof();
			chain->Process("PreparedSelector.C");	
			break;
		}

		//make inv mass hists 
		//we variate centrality: [0-20], [20-40], [40-60]
		//			pT:	[0-0.5], [0.5-1.5]
		//		and arm
		case 3: {
			TFile outputFile("output.root", "UPDATE");
			protons = (TTree*)outputFile.Get("protons");
			pions = (TTree*)outputFile.Get("pions");
			
   			protons->SetBranchAddress("mh", &protMh);
   			protons->SetBranchAddress("cent", &protCent);
   			protons->SetBranchAddress("p", protP);
   			protons->SetBranchAddress("the0", protThe);
   			protons->SetBranchAddress("phi0", protPhi);
   			protons->SetBranchAddress("beta", protBeta);
   			protons->SetBranchAddress("dcarm", protArm);
   			pions->SetBranchAddress("mh", &pionMh);
   			pions->SetBranchAddress("p", pionP);
   			pions->SetBranchAddress("the0", pionThe);
   			pions->SetBranchAddress("phi0", pionPhi);
   			pions->SetBranchAddress("beta", pionBeta);
   			pions->SetBranchAddress("dcarm", pionArm);
   			
   			Long64_t nEntries = protons->GetEntries();
   			cout << "number of events: " << nEntries << endl;
   				

   			for (int i=0; i<3; i++) {
   				//WEST LOW PT
   				invMassWestLowPt[i] = new TH1F(TString("invMassWestLowPt")+TString::Itoa(i, 10),
   					"invariant mass for pairs proton-pion", 500, 1, 1.6);
   				invMassWestLowPt[i]->SetXTitle("inv mass pairs proton-pion, GeV");
   				invMassWestLowPt[i]->SetYTitle("number of particles");

   				//EAST LOW PT
   				invMassEastLowPt[i] = new TH1F(TString("invMassEastLowPt")+TString::Itoa(i, 10)
   					, "invariant mass for pairs proton-pion", 500, 1, 1.6);
   				invMassEastLowPt[i]->SetXTitle("inv mass pairs proton-pion, GeV");
   				invMassEastLowPt[i]->SetYTitle("number of particles");

   				//WEST HIGH PT
   				invMassWestHighPt[i] = new TH1F(TString("invMassWestHighPt")+TString::Itoa(i, 10),
   					"invariant mass for pairs proton-pion", 500, 1, 1.6);
   				//EAST HIGH PT
   				invMassEastHighPt[i] = new TH1F(TString("invMassEastHighPt")+TString::Itoa(i, 10),
   					"invariant mass for pairs proton-pion", 500, 1, 1.6);
   				countInvMass(i, nEntries, protons, pions);
   			}
   			
   			gROOT->SetBatch(kTRUE);
   			TCanvas *c2 = new TCanvas("c2", "inv mass", 1100, 600);
   			c2->Divide(2,2);
   			
   			for (int i=0; i<3; i++) {
   				c2->cd(i+1);
   				invMassWestLowPt[i]->SetLineColor(1);
   				invMassWestLowPt[i]->Draw("same");
   				
   				invMassEastLowPt[i]->SetLineColor(2);
   				invMassEastLowPt[i]->Draw("same");
				
				invMassWestHighPt[i]->SetLineColor(3);
   				invMassWestHighPt[i]->Draw("same");
   				
   				invMassEastHighPt[i]->SetLineColor(4);
   				invMassEastHighPt[i]->Draw("same");
			}
   			// c2->cd(2);
   			// invMassEast->Draw("same");
			/*TLegend *legend = new TLegend(0.1,0.7,0.48,0.9);
         	legend->SetHeader("Header","C");             // option "C" allows to center the header
         	legend->AddEntry("invMassWest","West amr","l");
         	legend->AddEntry("sqMassHistsEast0","East arm","l");
         	legend->Draw();*/

			c2->Write();
   			outputFile.Close();
			break;

		}

		case 4: {
			TFile anotherFile("prepared.root");
			charged = (TTree*)anotherFile.Get("prepared");
			TFile outputFile("output.root", "UPDATE");
			protons = (TTree*)outputFile.Get("protons");
			
   			protons->SetBranchAddress("mh", &protMh);
   			protons->SetBranchAddress("cent", &protCent);
   			protons->SetBranchAddress("p", protP);
   			protons->SetBranchAddress("the0", protThe);
   			protons->SetBranchAddress("phi0", protPhi);
   			protons->SetBranchAddress("beta", protBeta);
   			protons->SetBranchAddress("dcarm", protArm);
   			charged->SetBranchAddress("mh", &pionMh);
   			charged->SetBranchAddress("p", pionP);
   			charged->SetBranchAddress("the0", pionThe);
   			charged->SetBranchAddress("phi0", pionPhi);
   			charged->SetBranchAddress("beta", pionBeta);
   			charged->SetBranchAddress("dcarm", pionArm);
   			
   			Long64_t nEntries = protons->GetEntries();
   			cout << "number of events: " << nEntries << endl;
   				

   			for (int i=0; i<3; i++) {
   				//WEST LOW PT
   				invMassWestLowPt[i] = new TH1F(TString("invMassWestLowPt")+TString::Itoa(i, 10),
   					"invariant mass for pairs proton-pion", 500, 1, 1.6);
   				invMassWestLowPt[i]->SetXTitle("inv mass pairs proton-pion, GeV");
   				invMassWestLowPt[i]->SetYTitle("number of particles");

   				//EAST LOW PT
   				invMassEastLowPt[i] = new TH1F(TString("invMassEastLowPt")+TString::Itoa(i, 10)
   					, "invariant mass for pairs proton-pion", 500, 1, 1.6);
   				invMassEastLowPt[i]->SetXTitle("inv mass pairs proton-pion, GeV");
   				invMassEastLowPt[i]->SetYTitle("number of particles");

   				//WEST HIGH PT
   				invMassWestHighPt[i] = new TH1F(TString("invMassWestHighPt")+TString::Itoa(i, 10),
   					"invariant mass for pairs proton-pion", 500, 1, 1.6);
   				//EAST HIGH PT
   				invMassEastHighPt[i] = new TH1F(TString("invMassEastHighPt")+TString::Itoa(i, 10),
   					"invariant mass for pairs proton-pion", 500, 1, 1.6);
   				countInvMass(i, nEntries, protons, charged);
   			}
   			
   			gROOT->SetBatch(kTRUE);
   			TCanvas *c3 = new TCanvas("c3", "inv mass", 1100, 600);
   			c3->Divide(2,2);
   			
   			for (int i=0; i<3; i++) {
   				c3->cd(i+1);
   				invMassWestLowPt[i]->SetLineColor(1);
   				invMassWestLowPt[i]->Draw("same");
   				
   				invMassEastLowPt[i]->SetLineColor(2);
   				invMassEastLowPt[i]->Draw("same");
				
				invMassWestHighPt[i]->SetLineColor(3);
   				invMassWestHighPt[i]->Draw("same");
   				
   				invMassEastHighPt[i]->SetLineColor(4);
   				invMassEastHighPt[i]->Draw("same");
			}
   			// c2->cd(2);
   			// invMassEast->Draw("same");
			
			c3->Write();
   			outputFile.Close();
			break;

		}
	}

}

