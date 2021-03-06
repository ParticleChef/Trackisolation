#define test_cxx
#include "test.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <algorithm>

using namespace std;

void test::Loop()
{
//   In a ROOT session, you can do:
//      root> .L test.C
//      root> test t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   vector<float> v1;

   TH1F *h1 = new TH1F("h1","",100,0,1);

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
     
      Float_t nomi = 0.;
      Float_t denomi = 0.;

      v1.clear();
      if( !ntnEg3 ) continue;
      if( recon_pT->size() == 0 ) continue;
      //cout << recon_pT->size() << endl; 
  
      for(Int_t i = 0; i < recon_pT->size(); i++) v1.push_back(recon_pT->at(i));
      //for(Int_t i = 0; i < recon_pT->size(); i++) cout << recon_pT->at(i) << endl;
      //cout << endl;
      sort(v1.begin(), v1.end());
      //cout << "sorting vector" << endl;
      //for(Int_t i = 0; i < v1.size(); i++) cout << v1.at(i) << endl;
      //cout << "===================" << endl;
      //cout << endl;
      
      for(Int_t j = 0; j < v1.size();   j++) denomi += v1.at(j);
      for(Int_t k = 0; k < v1.size()-1; k++) nomi += v1.at(k);

      Float_t ratio = nomi/denomi;

      h1->Fill(ratio);
     
   }

   TFile *output = new TFile("plot.root","RECREATE");
   h1->Write();
   output->Close();
}
