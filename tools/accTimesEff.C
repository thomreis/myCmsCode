#define accTimesEff_cxx
#include "accTimesEff.h"
#include <TH1.h>
#include <TF1.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TString.h>
#include "DataFormats/Math/interface/deltaR.h"

using namespace std;

void AccTimesEff::Loop()
{
   TStopwatch timer;
   timer.Start();
   // parameters /////////////////////////////////////////////////////////////
   vector<TString> files;
   files.push_back("file:////user/treis/mcsamples/ZprimeToEMu_M-500_noAccCuts_TuneZ2star_8TeV_madgraph_treis-Summer12_DR53X_PU_S10_START53_V7C1-v1_9999ev.root");
   files.push_back("file:////user/treis/mcsamples/ZprimeToEMu_M-750_noAccCuts_TuneZ2star_8TeV_madgraph_treis-Summer12_DR53X_PU_S10_START53_V7C1-v1_10000ev.root");
   files.push_back("file:////user/treis/mcsamples/ZprimeToEMu_M-1000_noAccCuts_TuneZ2star_8TeV_madgraph_treis-Summer12_DR53X_PU_S10_START53_V7C1-v1_9998ev.root");
   files.push_back("file:////user/treis/mcsamples/ZprimeToEMu_M-1250_noAccCuts_TuneZ2star_8TeV_madgraph_treis-Summer12_DR53X_PU_S10_START53_V7C1-v1_9998ev.root");
   files.push_back("file:////user/treis/mcsamples/ZprimeToEMu_M-1500_noAccCuts_TuneZ2star_8TeV_madgraph_treis-Summer12_DR53X_PU_S10_START53_V7C1-v1_9997ev.root");
   files.push_back("file:////user/treis/mcsamples/ZprimeToEMu_M-1750_noAccCuts_TuneZ2star_8TeV_madgraph_treis-Summer12_DR53X_PU_S10_START53_V7C1-v1_9997ev.root");
   files.push_back("file:////user/treis/mcsamples/ZprimeToEMu_M-2000_noAccCuts_TuneZ2star_8TeV_madgraph_treis-Summer12_DR53X_PU_S10_START53_V7C1-v1_9999ev.root");
   files.push_back("file:////user/treis/mcsamples/ZprimeToEMu_M-2500_noAccCuts_TuneZ2star_8TeV_madgraph_treis-Summer12_DR53X_PU_S10_START53_V7C1-v1_9999ev.root");
   files.push_back("file:////user/treis/mcsamples/ZprimeToEMu_M-3000_noAccCuts_TuneZ2star_8TeV_madgraph_treis-Summer12_DR53X_PU_S10_START53_V7C1-v1_10000ev.root");
   files.push_back("file:////user/treis/mcsamples/ZprimeToEMu_M-3500_noAccCuts_TuneZ2star_8TeV_madgraph_treis-Summer12_DR53X_PU_S10_START53_V7C1-v1_9898ev.root");
   files.push_back("file:////user/treis/mcsamples/ZprimeToEMu_M-4000_noAccCuts_TuneZ2star_8TeV_madgraph_treis-Summer12_DR53X_PU_S10_START53_V7C1-v1_9998ev.root");
   files.push_back("file:////user/treis/mcsamples/ZprimeToEMu_M-5000_noAccCuts_TuneZ2star_8TeV_madgraph_treis-Summer12_DR53X_PU_S10_START53_V7C1-v1_9966ev.root");
   string outfileName = "accTimesEffHistos";

   // output file formats
   const bool saveSpec = 0;
   const bool saveAsPdf = 0;
   const bool saveAsPng = 1;
   const bool saveAsRoot = 0;
   TString plotDir = "./plots/";

   int font = 42; //62
   // selection cuts /////////////////////////////////////////////////////////
   float elePtCut = 35.;
   float muPtCut = 35.;
   float minInvMass = 0.;

   TH1::SetDefaultSumw2(kTRUE);

   ///////////////////////////////////////////////////////////////////////////

   ///////////////////////////////////////////////////////////////////////////

   TH1F *hGenEvts = new TH1F("hGenEvts", "hGenEvts", 145, 0., 5010.);
   hGenEvts->GetXaxis()->SetTitle("M_{Z'}^{truth}");
   hGenEvts->GetXaxis()->SetTitleFont(font);
   hGenEvts->GetXaxis()->SetLabelFont(font);
   hGenEvts->GetYaxis()->SetTitle("Events");
   hGenEvts->GetYaxis()->SetTitleFont(font);
   hGenEvts->GetYaxis()->SetLabelFont(font);
   hGenEvts->GetYaxis()->SetTitleOffset(1.2);
   hGenEvts->SetLineColor(kBlack);
   hGenEvts->SetLineWidth(2);
   hGenEvts->SetMarkerStyle(20);
   hGenEvts->SetMarkerColor(kBlack);
   TH1F *hGenEvtsEleInAcc = (TH1F*)hGenEvts->Clone("hGenEvtsEleInAcc");
   TH1F *hGenEvtsEleInAccEB = (TH1F*)hGenEvts->Clone("hGenEvtsEleInAccEB");
   TH1F *hGenEvtsEleInAccEE = (TH1F*)hGenEvts->Clone("hGenEvtsEleInAccEE");
   TH1F *hGenEvtsMuInAcc = (TH1F*)hGenEvts->Clone("hGenEvtsMuInAcc");
   TH1F *hGenEvtsInAcc = (TH1F*)hGenEvts->Clone("hGenEvtsInAcc");
   TH1F *hTrgEvts = (TH1F*)hGenEvts->Clone("hTrgEvts");
   hTrgEvts->SetTitle("hTrgEvts");
   TH1F *hRecoEvts = (TH1F*)hGenEvts->Clone("hRecoEvts");
   hRecoEvts->SetTitle("hRecoEvts");
   TH1F *hRecoEvtsEB = (TH1F*)hRecoEvts->Clone("hRecoEvtsEB");
   TH1F *hRecoEvtsEE = (TH1F*)hRecoEvts->Clone("hRecoEvtsEE");
   TH1F *hRecoEleEvts = (TH1F*)hRecoEvts->Clone("hRecoEleEvts");
   TH1F *hRecoEleEvtsEB = (TH1F*)hRecoEvts->Clone("hRecoEleEvtsEB");
   TH1F *hRecoEleEvtsEE = (TH1F*)hRecoEvts->Clone("hRecoEleEvtsEE");
   TH1F *hRecoMuEvts = (TH1F*)hRecoEvts->Clone("hRecoMuEvts");
   TH1F *hRecoNoTrgEvts = (TH1F*)hGenEvts->Clone("hRecoNoTrgEvts");
   hRecoNoTrgEvts->SetTitle("hRecoNoTrgEvts");
   TH1F *hRecoNoTrgEvtsEB = (TH1F*)hRecoNoTrgEvts->Clone("hRecoNoTrgEvtsEB");
   TH1F *hRecoNoTrgEvtsEE = (TH1F*)hRecoNoTrgEvts->Clone("hRecoNoTrgEvtsEE");
   TH1F *hRecoNoTrgEleEvts = (TH1F*)hRecoNoTrgEvts->Clone("hRecoNoTrgEleEvts");
   TH1F *hRecoNoTrgEleEvtsEB = (TH1F*)hRecoNoTrgEvts->Clone("hRecoNoTrgEleEvtsEB");
   TH1F *hRecoNoTrgEleEvtsEE = (TH1F*)hRecoNoTrgEvts->Clone("hRecoNoTrgEleEvtsEE");
   TH1F *hRecoNoTrgMuEvts = (TH1F*)hRecoNoTrgEvts->Clone("hRecoNoTrgMuEvts");
   TH1F* hAcc;
   TH1F* hAccEle;
   TH1F* hAccEleEB;
   TH1F* hAccEleEE;
   TH1F* hAccMu;
   TH1F* hAccTimesTrgEff;
   TH1F* hTrgEff;
   TH1F* hAccTimesEff;
   TH1F* hAccTimesEffEB;
   TH1F* hAccTimesEffEE;
   TH1F* hAccTimesEffEle;
   TH1F* hAccTimesEffEleEB;
   TH1F* hAccTimesEffEleEE;
   TH1F* hAccTimesEffMu;
   TH1F* hAccTimesEffNoTrg;
   //TH1F* hAccTimesEffNoTrgEB;
   //TH1F* hAccTimesEffNoTrgEE;
   TH1F* hAccTimesEffNoTrgEle;
   TH1F* hAccTimesEffNoTrgEleEB;
   TH1F* hAccTimesEffNoTrgEleEE;
   TH1F* hAccTimesEffNoTrgMu;
   TH1F* hEffAftTrg;
   TH1F* hEffAftTrgEle;
   TH1F* hEffAftTrgEleEB;
   TH1F* hEffAftTrgEleEE;
   TH1F* hEffAftTrgMu;
   TH1F* hTrgRecoVsReco;
   //TH1F* hTrgRecoVsRecoEB;
   //TH1F* hTrgRecoVsRecoEE;
   TH1F* hTrgRecoVsRecoEle;
   TH1F* hTrgRecoVsRecoEleEB;
   TH1F* hTrgRecoVsRecoEleEE;
   TH1F* hTrgRecoVsRecoMu;

   // output file
   stringstream ssOutfile;
   ssOutfile << outfileName << ".root";
   TFile *output = new TFile(ssOutfile.str().c_str(), "recreate");

   ///////////////////////////////////////////////////////////////////////////
   //LOOP OVER FILES 
   ///////////////////////////////////////////////////////////////////////////
   for (unsigned int p = 0; p < files.size(); ++p) {
      TFile* input = new TFile(files[p]);
      TTree *thetree = (TTree*)input->Get("gsfcheckerjob/tree");
      Init(thetree);
      Long64_t nentries = fChain->GetEntriesFast();
      cout << nentries << " events" << endl;

      unsigned int evCounter = 0;
      /////////////////////////////////////////////////////////////////////////////////////////////
      //LOOP OVER EVENTS
      /////////////////////////////////////////////////////////////////////////////////////////////
      //nentries = 10000;
      for (Long64_t jentry = 0; jentry < nentries; ++jentry) {
         Long64_t ientry = LoadTree(jentry);
         if (ientry < 0) break;
         fChain->GetEntry(jentry);
         // if (Cut(ientry) < 0) continue;
         //if (jentry % 50000 == 0) cout << "Processing event " << jentry << endl;
         thetree->GetEntry(jentry);

         // fill the gen histograms
         hGenEvts->Fill(genelemom_mass[0]);

         // fill the acc histograms
         if (hardGenEle_pt[0] > elePtCut) {
            if (fabs(hardGenEle_eta[0]) < 1.442) {
               hGenEvtsEleInAcc->Fill(genelemom_mass[0]);
               hGenEvtsEleInAccEB->Fill(genelemom_mass[0]);
            }
            else if (fabs(hardGenEle_eta[0]) > 1.56 && fabs(hardGenEle_eta[0]) < 2.5) {
               hGenEvtsEleInAcc->Fill(genelemom_mass[0]);
               hGenEvtsEleInAccEE->Fill(genelemom_mass[0]);
            }
         }
         if (hardGenMu_pt[0] > muPtCut && fabs(hardGenMu_eta[0]) < 2.4) {
            hGenEvtsMuInAcc->Fill(genelemom_mass[0]);
            if (fabs(hardGenEle_eta[0]) < 1.442 || (fabs(hardGenEle_eta[0]) > 1.56 && fabs(hardGenEle_eta[0]) < 2.5)) {
               if (hardGenEle_pt[0] > elePtCut) hGenEvtsInAcc->Fill(genelemom_mass[0]);
            }
         }

         // trigger?
         if (HLT_Mu22_Photon22_CaloIdL) hTrgEvts->Fill(genelemom_mass[0]);

         // at least one gsf electron and one muon above the threshold
         if (gsf_size < 1 || muon_size < 1) continue;

         vector<int> GSF_passHEEP;
         vector<int> MU_passGOOD;
         /////////////////////////////////////////////////////////////////////////////////////////////
         //loop over electrons
         for (int j = 0; j < gsf_size; ++j) {
            //cleaning: fake electrons from muons
            bool fakeEle = false;
            for (int k = 0; k < muon_size; ++k) {
               float DeltaR = deltaR(gsf_eta[j], gsf_phi[j], muon_eta[k], muon_phi[k]);
               if (DeltaR < 0.1) {
                  fakeEle = true;
                  break;
               }
            }
            if (fakeEle) continue;

            if (PassHEEP(j)) GSF_passHEEP.push_back(j);
         }

         //loop over muons
         for (int j = 0; j < muon_size; ++j) {
            if (PassHighPtMu(j)) MU_passGOOD.push_back(j);
         }

         if (GSF_passHEEP.size() == 1) {
            if (HLT_Mu22_Photon22_CaloIdL) {
               hRecoEleEvts->Fill(genelemom_mass[0]);
               if (fabs(gsfsc_eta[GSF_passHEEP[0]]) < 1.5) hRecoEleEvtsEB->Fill(genelemom_mass[0]);
               if (fabs(gsfsc_eta[GSF_passHEEP[0]]) > 1.5) hRecoEleEvtsEE->Fill(genelemom_mass[0]);
            }
            hRecoNoTrgEleEvts->Fill(genelemom_mass[0]);
            if (fabs(gsfsc_eta[GSF_passHEEP[0]]) < 1.5) hRecoNoTrgEleEvtsEB->Fill(genelemom_mass[0]);
            if (fabs(gsfsc_eta[GSF_passHEEP[0]]) > 1.5) hRecoNoTrgEleEvtsEE->Fill(genelemom_mass[0]);
         } 
         if (MU_passGOOD.size() == 1) {
            if (HLT_Mu22_Photon22_CaloIdL) hRecoMuEvts->Fill(genelemom_mass[0]);
            hRecoNoTrgMuEvts->Fill(genelemom_mass[0]);
         }

         // veto when there are more than one good candidates
         if (GSF_passHEEP.size() != 1 || MU_passGOOD.size() != 1) continue;

         //HEEP ele + GOOD muon
         TLorentzVector ele1;
         TLorentzVector mu1;

         ele1.SetPtEtaPhiM(gsf_gsfet[GSF_passHEEP[0]], gsf_eta[GSF_passHEEP[0]], gsf_phi[GSF_passHEEP[0]], 0.000511);
         mu1.SetPtEtaPhiM(muon_pt[MU_passGOOD[0]], muon_eta[MU_passGOOD[0]], muon_phi[MU_passGOOD[0]], 0.10566);

         double invMass = (ele1 + mu1).M();

         //MASS CUT
         if (invMass < minInvMass) continue;

         if (HLT_Mu22_Photon22_CaloIdL) {
            hRecoEvts->Fill(genelemom_mass[0]);
            if (fabs(gsfsc_eta[GSF_passHEEP[0]]) < 1.5) hRecoEvtsEB->Fill(genelemom_mass[0]);
            if (fabs(gsfsc_eta[GSF_passHEEP[0]]) > 1.5) hRecoEvtsEE->Fill(genelemom_mass[0]);
         }
         hRecoNoTrgEvts->Fill(genelemom_mass[0]);
         if (fabs(gsfsc_eta[GSF_passHEEP[0]]) < 1.5) hRecoNoTrgEvtsEB->Fill(genelemom_mass[0]);
         if (fabs(gsfsc_eta[GSF_passHEEP[0]]) > 1.5) hRecoNoTrgEvtsEE->Fill(genelemom_mass[0]);
         ++evCounter;
        ///////////////////////////////////////////////////////////////////////
      } //END LOOP OVER EVENTS
        //////////////////////////////////////////////////////////////////////

     //////////////////////////////////////////////////////////////////////////
   } //END LOOP OVER FILES
     //////////////////////////////////////////////////////////////////////////
   hAcc = (TH1F*)hGenEvtsInAcc->Clone("hAcc");
   hAccEle = (TH1F*)hGenEvtsEleInAcc->Clone("hAccEle");
   hAccEleEB = (TH1F*)hGenEvtsEleInAccEB->Clone("hAccEleEB");
   hAccEleEE = (TH1F*)hGenEvtsEleInAccEE->Clone("hAccEleEE");
   hAccMu = (TH1F*)hGenEvtsMuInAcc->Clone("hAccMu");
   hAccTimesTrgEff = (TH1F*)hTrgEvts->Clone("hAccTimesTrgEff");
   hTrgEff = (TH1F*)hTrgEvts->Clone("hTrgEff");
   hAccTimesEff = (TH1F*)hRecoEvts->Clone("hAccTimesEff");
   hAccTimesEffEB = (TH1F*)hRecoEvtsEB->Clone("hAccTimesEffEB");
   hAccTimesEffEE = (TH1F*)hRecoEvtsEE->Clone("hAccTimesEffEE");
   hAccTimesEffEle = (TH1F*)hRecoEleEvts->Clone("hAccTimesEffEle");
   hAccTimesEffEleEB = (TH1F*)hRecoEleEvtsEB->Clone("hAccTimesEffEleEB");
   hAccTimesEffEleEE = (TH1F*)hRecoEleEvtsEE->Clone("hAccTimesEffEleEE");
   hAccTimesEffMu = (TH1F*)hRecoMuEvts->Clone("hAccTimesEffMu");
   hAccTimesEffNoTrg = (TH1F*)hRecoNoTrgEvts->Clone("hAccTimesEffNoTrg");
   //hAccTimesEffNoTrgEB = (TH1F*)hRecoNoTrgEvtsEB->Clone("hAccTimesEffNoTrgEB");
   //hAccTimesEffNoTrgEE = (TH1F*)hRecoNoTrgEvtsEE->Clone("hAccTimesEffNoTrgEE");
   hAccTimesEffNoTrgEle = (TH1F*)hRecoNoTrgEleEvts->Clone("hAccTimesEffNoTrgEle");
   hAccTimesEffNoTrgEleEB = (TH1F*)hRecoNoTrgEleEvtsEB->Clone("hAccTimesEffNoTrgEleEB");
   hAccTimesEffNoTrgEleEE = (TH1F*)hRecoNoTrgEleEvtsEE->Clone("hAccTimesEffNoTrgEleEE");
   hAccTimesEffNoTrgMu = (TH1F*)hRecoNoTrgMuEvts->Clone("hAccTimesEffNoTrgMu");
   hEffAftTrg = (TH1F*)hRecoEvts->Clone("hAccTimesEff");
   hEffAftTrgEle = (TH1F*)hRecoEleEvts->Clone("hAccTimesEffEle");
   hEffAftTrgEleEB = (TH1F*)hRecoEleEvtsEB->Clone("hAccTimesEffEleEB");
   hEffAftTrgEleEE = (TH1F*)hRecoEleEvtsEE->Clone("hAccTimesEffEleEE");
   hEffAftTrgMu = (TH1F*)hRecoMuEvts->Clone("hAccTimesEffMu");
   hTrgRecoVsReco = (TH1F*)hRecoEvts->Clone("hTrgRecoVsReco");
   //hTrgRecoVsRecoEB = (TH1F*)hRecoEvtsEB->Clone("hTrgRecoVsRecoEB");
   //hTrgRecoVsRecoEE = (TH1F*)hRecoEvtsEE->Clone("hTrgRecoVsRecoEE");
   hTrgRecoVsRecoEle = (TH1F*)hRecoEleEvts->Clone("hTrgRecoVsRecoEle");
   hTrgRecoVsRecoEleEB = (TH1F*)hRecoEleEvtsEB->Clone("hTrgRecoVsRecoEleEB");
   hTrgRecoVsRecoEleEE = (TH1F*)hRecoEleEvtsEE->Clone("hTrgRecoVsRecoEleEE");
   hTrgRecoVsRecoMu = (TH1F*)hRecoMuEvts->Clone("hTrgRecoVsRecoMu");
   hAcc->Divide(hGenEvts);
   hAccEle->Divide(hGenEvts);
   hAccEleEB->Divide(hGenEvts);
   hAccEleEE->Divide(hGenEvts);
   hAccMu->Divide(hGenEvts);
   hAccTimesTrgEff->Divide(hGenEvts);
   hTrgEff->Divide(hGenEvtsInAcc);
   hAccTimesEff->Divide(hGenEvts);
   hAccTimesEffEB->Divide(hGenEvts);
   hAccTimesEffEE->Divide(hGenEvts);
   hAccTimesEffEle->Divide(hGenEvts);
   hAccTimesEffEleEB->Divide(hGenEvts);
   hAccTimesEffEleEE->Divide(hGenEvts);
   hAccTimesEffMu->Divide(hGenEvts);
   hAccTimesEffNoTrg->Divide(hGenEvts);
   //hAccTimesEffNoTrgEB->Divide(hGenEvts);
   //hAccTimesEffNoTrgEE->Divide(hGenEvts);
   hAccTimesEffNoTrgEle->Divide(hGenEvts);
   hAccTimesEffNoTrgEleEB->Divide(hGenEvts);
   hAccTimesEffNoTrgEleEE->Divide(hGenEvts);
   hAccTimesEffNoTrgMu->Divide(hGenEvts);
   hEffAftTrg->Divide(hTrgEvts);
   hEffAftTrgEle->Divide(hTrgEvts);
   hEffAftTrgEleEB->Divide(hTrgEvts);
   hEffAftTrgEleEE->Divide(hTrgEvts);
   hEffAftTrgMu->Divide(hTrgEvts);
   hTrgRecoVsReco->Divide(hRecoNoTrgEvts);
   //hTrgRecoVsRecoEB->Divide(hRecoNoTrgEvtsEB);
   //hTrgRecoVsRecoEE->Divide(hRecoNoTrgEvtsEE);
   hTrgRecoVsRecoEle->Divide(hRecoNoTrgEleEvts);
   hTrgRecoVsRecoEleEB->Divide(hRecoNoTrgEleEvtsEB);
   hTrgRecoVsRecoEleEE->Divide(hRecoNoTrgEleEvtsEE);
   hTrgRecoVsRecoMu->Divide(hRecoNoTrgMuEvts);

   // plot
   TCanvas *accTimesEffPlot = new TCanvas("accTimesEffPlot", "acc x eff", 100, 100, 600, 600);
   TPad *accTimesEffPad = new TPad("accTimesEffPad", "acc x eff pad", 0., 0., 1., 1.);
   accTimesEffPad->SetBottomMargin(0.12);
   accTimesEffPad->SetBorderMode(0);
   accTimesEffPad->SetBorderSize(2);
   accTimesEffPad->SetFrameBorderMode(0);
   accTimesEffPad->SetFillColor(0);
   accTimesEffPad->SetFrameFillColor(0);
   accTimesEffPad->SetLeftMargin(0.11);
   accTimesEffPad->SetRightMargin(0.09);
   accTimesEffPad->SetTopMargin(0.08);
   accTimesEffPad->SetTickx(1);
   accTimesEffPad->SetTicky(1);
   accTimesEffPad->Draw();
   accTimesEffPad->cd();

   gStyle->SetTitleFont(font);
   gStyle->SetLabelFont(font);
   gStyle->SetLegendFont(font);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   gStyle->SetOptFit(1111);
   gStyle->SetTitleXOffset(1.);
   gStyle->SetTitleYOffset(1.3);
   gPad->SetTicks(1, 1);
   gPad->SetGrid(1, 1);

   TH1F* hAccTimesEff2 = (TH1F*)hAccTimesEff->Clone("hAccTimesEff2");

   TF1 *fitFunc = new TF1("fitFunc", "[0] + [1]/ (x + [2]) + [3]*x", 10., 5010.);
   //TF1 *fitFuncEB = new TF1("fitFuncEB", "[0] + [1]/ (x + [2])", 10., 5010.);
   TF1 *fitFuncEB = new TF1("fitFuncEB", "[0] + [1]/ (x + [2]) + [3]*x", 10., 5010.);
   TF1 *fitFuncEE = new TF1("fitFuncEE", "[0] + [1]/ (x*x + [2])", 10., 5010.);
   fitFunc->SetLineColor(kBlue);
   fitFuncEB->SetLineColor(kBlue);
   fitFuncEE->SetLineColor(kBlue);
   hAccTimesEff->Fit("fitFunc", "", "", 480., 5010.);
   hAccTimesEffEB->Fit("fitFuncEB", "", "", 480., 5010.);
   hAccTimesEffEE->Fit("fitFuncEE", "", "", 480., 5010.);
   cout << "Chi^2 / NDF: " << fitFunc->GetChisquare() << " / " << fitFunc->GetNDF() << ", prob: " << fitFunc->GetProb() << endl;
   cout << "Chi^2 / NDF EB: " << fitFuncEB->GetChisquare() << " / " << fitFuncEB->GetNDF() << ", prob: " << fitFuncEB->GetProb() << endl;
   cout << "Chi^2 / NDF EE: " << fitFuncEE->GetChisquare() << " / " << fitFuncEE->GetNDF() << ", prob: " << fitFuncEE->GetProb() << endl;

   hAccTimesEff->GetYaxis()->SetTitle("acc x eff");
   hAccTimesEff->GetYaxis()->SetRangeUser(0., 1.);
   hAccTimesEff->Draw();
   TLatex *tex = new TLatex(0.22, 0.21, "P(M|p0,p1,p2,p3) = p0 + #frac{p1}{M+p2} + p3*M");
   tex->SetNDC();
   tex->SetTextFont(font);
   tex->SetLineWidth(2);
   tex->SetTextSize(0.03);
   tex->Draw();
   tex->DrawLatex(0.109, 0.935, "CMS Simulation, 8 TeV");
   tex->DrawLatex(0.17, 0.85, "trg + electron + muon");

   TCanvas *accTimesEffPlotEB = new TCanvas("accTimesEffPlotEB", "acc x eff, barrel electron + muon", 100, 100, 600, 600);
   TPad *accTimesEffPadEB = (TPad*)accTimesEffPad->Clone("accTimesEffPadEB");
   accTimesEffPadEB->Draw(); 
   accTimesEffPadEB->cd();
   hAccTimesEffEB->GetYaxis()->SetTitle("acc x eff");
   hAccTimesEffEB->GetYaxis()->SetRangeUser(0., 1.);
   hAccTimesEffEB->Draw();
   tex->DrawLatex(0.109, 0.935, "CMS Simulation, 8 TeV");
   //tex->DrawLatex(0.46, 0.21, "P(M|p0,p1,p2) = p0 + #frac{p1}{M+p2}");
   tex->DrawLatex(0.22, 0.21, "P(M|p0,p1,p2,p3) = p0 + #frac{p1}{M+p2} + p3*M");
   tex->DrawLatex(0.17, 0.85, "trg + barrel electron + muon");

   TCanvas *accTimesEffPlotEE = new TCanvas("accTimesEffPlotEE", "acc x eff, endcap electron + muon", 100, 100, 600, 600);
   TPad *accTimesEffPadEE = (TPad*)accTimesEffPad->Clone("accTimesEffPadEE");
   accTimesEffPadEE->Draw(); 
   accTimesEffPadEE->cd();
   hAccTimesEffEE->GetYaxis()->SetTitle("acc x eff");
   hAccTimesEffEE->GetYaxis()->SetRangeUser(0., 1.);
   hAccTimesEffEE->Draw();
   tex->DrawLatex(0.109, 0.935, "CMS Simulation, 8 TeV");
   tex->DrawLatex(0.45, 0.38, "P(M|p0,p1,p2) = p0 + #frac{p1}{M^{2}+p2}");
   tex->DrawLatex(0.17, 0.85, "trg + endcap electron + muon");

   TCanvas *accTimesEffObjPlot = new TCanvas("accTimesEffObjPlot", "acc x eff, objects", 100, 100, 600, 600);
   TPad *accTimesEffObjPad = (TPad*)accTimesEffPad->Clone("accTimesEffObjPad");
   accTimesEffObjPad->Draw(); 
   accTimesEffObjPad->cd();
   hAccTimesEffEle->GetYaxis()->SetTitle("acc x eff");
   hAccTimesEffEle->GetYaxis()->SetRangeUser(0., 1.);
   hAccTimesEffEle->SetMarkerStyle(kFullSquare);
   hAccTimesEffEle->SetMarkerColor(kViolet);
   hAccTimesEffEle->SetLineColor(kViolet);
   hAccTimesEffEle->Draw();
   hAccTimesEffEleEB->SetMarkerStyle(kFullTriangleUp);
   hAccTimesEffEleEB->SetMarkerColor(kRed);
   hAccTimesEffEleEB->SetLineColor(kRed);
   hAccTimesEffEleEB->Draw("same");
   hAccTimesEffEleEE->SetMarkerStyle(kFullTriangleDown);
   hAccTimesEffEleEE->SetMarkerColor(kBlue);
   hAccTimesEffEleEE->SetLineColor(kBlue);
   hAccTimesEffEleEE->Draw("same");
   hAccTimesEffMu->SetMarkerStyle(34);
   hAccTimesEffMu->SetMarkerColor(kGreen+1);
   hAccTimesEffMu->SetLineColor(kGreen+1);
   hAccTimesEffMu->Draw("same");
   hAccTimesEff2->Draw("same");
   TLegend* legend = new TLegend(0.592, 0.279, 0.881, 0.467);
   legend->SetTextFont(font);
   legend->SetTextSize(0.03);
   legend->SetBorderSize(0);
   legend->SetLineColor(1);
   legend->SetLineStyle(1);
   legend->SetLineWidth(1);
   legend->SetFillColor(19);
   legend->SetFillStyle(0);
   legend->AddEntry(hAccTimesEff, "total acc x eff");
   legend->AddEntry(hAccTimesEffMu, "muons");
   legend->AddEntry(hAccTimesEffEle, "all electrons");
   legend->AddEntry(hAccTimesEffEleEB, "barrel electrons");
   legend->AddEntry(hAccTimesEffEleEE, "endcap electrons");
   legend->Draw("same");
   tex->DrawLatex(0.109, 0.935, "CMS Simulation, 8 TeV");
   tex->DrawLatex(0.17, 0.85, "trg: HLT_Mu22_Photon22_CaloIdL");

   // acc x eff with no trg applied
   TCanvas *accTimesEffNoTrgObjPlot = new TCanvas("accTimesEffNoTrgObjPlot", "acc x eff, no trigger, objects", 100, 100, 600, 600);
   TPad *accTimesEffNoTrgObjPad = (TPad*)accTimesEffPad->Clone("accTimesEffNoTrgObjPad");
   accTimesEffNoTrgObjPad->Draw(); 
   accTimesEffNoTrgObjPad->cd();
   hAccTimesEffNoTrgEle->GetYaxis()->SetTitle("acc x eff");
   hAccTimesEffNoTrgEle->GetYaxis()->SetRangeUser(0., 1.);
   hAccTimesEffNoTrgEle->SetMarkerStyle(kFullSquare);
   hAccTimesEffNoTrgEle->SetMarkerColor(kViolet);
   hAccTimesEffNoTrgEle->SetLineColor(kViolet);
   hAccTimesEffNoTrgEle->Draw();
   hAccTimesEffNoTrgEleEB->SetMarkerStyle(kFullTriangleUp);
   hAccTimesEffNoTrgEleEB->SetMarkerColor(kRed);
   hAccTimesEffNoTrgEleEB->SetLineColor(kRed);
   hAccTimesEffNoTrgEleEB->Draw("same");
   hAccTimesEffNoTrgEleEE->SetMarkerStyle(kFullTriangleDown);
   hAccTimesEffNoTrgEleEE->SetMarkerColor(kBlue);
   hAccTimesEffNoTrgEleEE->SetLineColor(kBlue);
   hAccTimesEffNoTrgEleEE->Draw("same");
   hAccTimesEffNoTrgMu->SetMarkerStyle(34);
   hAccTimesEffNoTrgMu->SetMarkerColor(kGreen+1);
   hAccTimesEffNoTrgMu->SetLineColor(kGreen+1);
   hAccTimesEffNoTrgMu->Draw("same");
   hAccTimesEffNoTrg->Draw("same");
   TLegend* accXeffNoTrg = (TLegend*)legend->Clone("effAftTrgLegend");
   accXeffNoTrg->Clear();
   accXeffNoTrg->AddEntry(hAccTimesEffNoTrg, "total acc x eff");
   accXeffNoTrg->AddEntry(hAccTimesEffNoTrgMu, "muons");
   accXeffNoTrg->AddEntry(hAccTimesEffNoTrgEle, "all electrons");
   accXeffNoTrg->AddEntry(hAccTimesEffNoTrgEleEB, "barrel electrons");
   accXeffNoTrg->AddEntry(hAccTimesEffNoTrgEleEE, "endcap electrons");
   accXeffNoTrg->Draw("same");
   tex->DrawLatex(0.109, 0.935, "CMS Simulation, 8 TeV");
   tex->DrawLatex(0.17, 0.85, "trg: none");

   // efficiency on triggered events
   TCanvas *effAftTrgPlot = new TCanvas("effAftTrgPlot", "efficiency after trigger", 100, 100, 600, 600);
   TPad *effAftTrgPad = (TPad*)accTimesEffPad->Clone("effAftTrgPad");
   effAftTrgPad->Draw(); 
   effAftTrgPad->cd();
   hEffAftTrgEle->GetYaxis()->SetTitle("eff");
   hEffAftTrgEle->GetYaxis()->SetRangeUser(0., 1.);
   hEffAftTrgEle->SetMarkerStyle(kFullSquare);
   hEffAftTrgEle->SetMarkerColor(kViolet);
   hEffAftTrgEle->SetLineColor(kViolet);
   hEffAftTrgEle->Draw();
   hEffAftTrgEleEB->SetMarkerStyle(kFullTriangleUp);
   hEffAftTrgEleEB->SetMarkerColor(kRed);
   hEffAftTrgEleEB->SetLineColor(kRed);
   hEffAftTrgEleEB->Draw("same");
   hEffAftTrgEleEE->SetMarkerStyle(kFullTriangleDown);
   hEffAftTrgEleEE->SetMarkerColor(kBlue);
   hEffAftTrgEleEE->SetLineColor(kBlue);
   hEffAftTrgEleEE->Draw("same");
   hEffAftTrgMu->SetMarkerStyle(34);
   hEffAftTrgMu->SetMarkerColor(kGreen+1);
   hEffAftTrgMu->SetLineColor(kGreen+1);
   hEffAftTrgMu->Draw("same");
   hEffAftTrg->Draw("same");
   TLegend* effAftTrgLegend = (TLegend*)legend->Clone("effAftTrgLegend");
   effAftTrgLegend->Clear();
   effAftTrgLegend->AddEntry(hEffAftTrg, "total eff after trigger");
   effAftTrgLegend->AddEntry(hEffAftTrgMu, "muons");
   effAftTrgLegend->AddEntry(hEffAftTrgEle, "all electrons");
   effAftTrgLegend->AddEntry(hEffAftTrgEleEB, "barrel electrons");
   effAftTrgLegend->AddEntry(hEffAftTrgEleEE, "endcap electrons");
   effAftTrgLegend->Draw("same");
   tex->DrawLatex(0.109, 0.935, "CMS Simulation, 8 TeV");
   tex->DrawLatex(0.14, 0.15, "trg: HLT_Mu22_Photon22_CaloIdL");

   // acceptance
   TCanvas *accPlot = new TCanvas("accPlot", "acc", 100, 100, 600, 600);
   TPad *accPad = (TPad*)accTimesEffPad->Clone("accPad");
   accPad->Draw(); 
   accPad->cd();
   hAcc->GetYaxis()->SetTitle("acc");
   hAcc->GetYaxis()->SetRangeUser(0., 1.);
   hAccEle->SetMarkerStyle(kFullSquare);
   hAccEle->SetMarkerColor(kViolet);
   hAccEle->SetLineColor(kViolet);
   hAccEle->Draw();
   hAccEleEB->SetMarkerStyle(kFullTriangleUp);
   hAccEleEB->SetMarkerColor(kRed);
   hAccEleEB->SetLineColor(kRed);
   hAccEleEB->Draw("same");
   hAccEleEE->SetMarkerStyle(kFullTriangleDown);
   hAccEleEE->SetMarkerColor(kBlue);
   hAccEleEE->SetLineColor(kBlue);
   hAccEleEE->Draw("same");
   hAccMu->SetMarkerStyle(34);
   hAccMu->SetMarkerColor(kGreen+1);
   hAccMu->SetLineColor(kGreen+1);
   hAccMu->Draw("same");
   hAcc->Draw("same");
   TLegend* accLegend = (TLegend*)legend->Clone("accLegend");
   accLegend->Clear();
   accLegend->AddEntry(hAcc, "total acceptance");
   accLegend->AddEntry(hAccMu, "muons");
   accLegend->AddEntry(hAccEle, "all electrons");
   accLegend->AddEntry(hAccEleEB, "barrel electrons");
   accLegend->AddEntry(hAccEleEE, "endcap electrons");
   accLegend->Draw("same");
   tex->DrawLatex(0.109, 0.935, "CMS Simulation, 8 TeV");

   // reco with trg vs. reco
   TCanvas *trgRecoVsRecoPlot = new TCanvas("trgRecoVsRecoPlot", "reco with trg vs. reco", 100, 100, 600, 600);
   TPad *trgRecoVsRecoPad = (TPad*)accTimesEffPad->Clone("trgRecoVsRecoPad");
   trgRecoVsRecoPad->Draw(); 
   trgRecoVsRecoPad->cd();
   hTrgRecoVsRecoEle->GetYaxis()->SetTitle("eff");
   hTrgRecoVsRecoEle->GetYaxis()->SetRangeUser(0., 1.);
   hTrgRecoVsRecoEle->SetMarkerStyle(kFullSquare);
   hTrgRecoVsRecoEle->SetMarkerColor(kViolet);
   hTrgRecoVsRecoEle->SetLineColor(kViolet);
   hTrgRecoVsRecoEle->Draw();
   //hTrgRecoVsRecoEleEB->SetMarkerStyle(kFullTriangleUp);
   //hTrgRecoVsRecoEleEB->SetMarkerColor(kRed);
   //hTrgRecoVsRecoEleEB->SetLineColor(kRed);
   //hTrgRecoVsRecoEleEB->Draw("same");
   //hTrgRecoVsRecoEleEE->SetMarkerStyle(kFullTriangleDown);
   //hTrgRecoVsRecoEleEE->SetMarkerColor(kBlue);
   //hTrgRecoVsRecoEleEE->SetLineColor(kBlue);
   //hTrgRecoVsRecoEleEE->Draw("same");
   hTrgRecoVsRecoMu->SetMarkerStyle(34);
   hTrgRecoVsRecoMu->SetMarkerColor(kGreen+1);
   hTrgRecoVsRecoMu->SetLineColor(kGreen+1);
   hTrgRecoVsRecoMu->Draw("same");
   hTrgRecoVsReco->Draw("same");
   TLegend* trgRecoVsRecoLegend = (TLegend*)legend->Clone("trgRecoVsRecoLegend");
   trgRecoVsRecoLegend->Clear();
   trgRecoVsRecoLegend->AddEntry(hTrgRecoVsReco, "total");
   trgRecoVsRecoLegend->AddEntry(hTrgRecoVsRecoMu, "muons");
   trgRecoVsRecoLegend->AddEntry(hTrgRecoVsRecoEle, "electrons");
   //trgRecoVsRecoLegend->AddEntry(hTrgRecoVsRecoEleEB, "barrel electrons");
   //trgRecoVsRecoLegend->AddEntry(hTrgRecoVsRecoEleEE, "endcap electrons");
   trgRecoVsRecoLegend->Draw("same");
   tex->DrawLatex(0.109, 0.935, "CMS Simulation, 8 TeV");

   TCanvas *accTimesTrgEffPlot = new TCanvas("accTimesTrgEffPlot", "acc x trigger eff", 100, 100, 600, 600);
   TPad *accTimesTrgEffPad = (TPad*)accTimesEffPad->Clone("accTimesTrgEffPad");
   accTimesTrgEffPad->Draw(); 
   accTimesTrgEffPad->cd();
   hAccTimesTrgEff->GetYaxis()->SetTitle("acc x trg eff");
   hAccTimesTrgEff->GetYaxis()->SetRangeUser(0., 1.);
   hAccTimesTrgEff->SetMarkerStyle(20);
   hAccTimesTrgEff->SetMarkerColor(kCyan);
   hAccTimesTrgEff->SetLineColor(kCyan);
   hAccTimesTrgEff->Draw();
   hTrgEff->SetMarkerStyle(21);
   hTrgEff->SetMarkerColor(kMagenta);
   hTrgEff->SetLineColor(kMagenta);
   hTrgEff->Draw("same");
   TLegend* trgLegend = (TLegend*)legend->Clone("trgLegend");
   trgLegend->Clear();
   trgLegend->AddEntry(hTrgEff, "trigger eff in acc");
   trgLegend->AddEntry(hAccTimesTrgEff, "acc x trigger eff");
   trgLegend->Draw("same");
   tex->DrawLatex(0.109, 0.935, "CMS Simulation, 8 TeV");

   // safe in various file formats
   if (saveSpec) {
     if (saveAsPdf) {
        accTimesTrgEffPlot->Print(plotDir + accTimesTrgEffPlot->GetName() + ".pdf", "pdf");
        accTimesEffPlot->Print(plotDir + accTimesEffPlot->GetName() + ".pdf", "pdf");
        accTimesEffPlotEB->Print(plotDir + accTimesEffPlotEB->GetName() + ".pdf", "pdf");
        accTimesEffPlotEE->Print(plotDir + accTimesEffPlotEE->GetName() + ".pdf", "pdf");
        accTimesEffObjPlot->Print(plotDir + accTimesEffObjPlot->GetName() + ".pdf", "pdf");
        accTimesEffNoTrgObjPlot->Print(plotDir + accTimesEffNoTrgObjPlot->GetName() + ".pdf", "pdf");
        effAftTrgPlot->Print(plotDir + effAftTrgPlot->GetName() + ".pdf", "pdf");
        trgRecoVsRecoPlot->Print(plotDir + trgRecoVsRecoPlot->GetName() + ".pdf", "pdf");
        accPlot->Print(plotDir + accPlot->GetName() + ".pdf", "pdf");
     }
     if (saveAsPng) {
        accTimesTrgEffPlot->Print(plotDir + accTimesTrgEffPlot->GetName() + ".png", "png");
        accTimesEffPlot->Print(plotDir + accTimesEffPlot->GetName() + ".png", "png");
        accTimesEffPlotEB->Print(plotDir + accTimesEffPlotEB->GetName() + ".png", "png");
        accTimesEffPlotEE->Print(plotDir + accTimesEffPlotEE->GetName() + ".png", "png");
        accTimesEffObjPlot->Print(plotDir + accTimesEffObjPlot->GetName() + ".png", "png");
        accTimesEffNoTrgObjPlot->Print(plotDir + accTimesEffNoTrgObjPlot->GetName() + ".png", "png");
        effAftTrgPlot->Print(plotDir + effAftTrgPlot->GetName() + ".png", "png");
        trgRecoVsRecoPlot->Print(plotDir + trgRecoVsRecoPlot->GetName() + ".png", "png");
        accPlot->Print(plotDir + accPlot->GetName() + ".png", "png");
     }
     if (saveAsRoot) {
        accTimesTrgEffPlot->Print(plotDir + accTimesTrgEffPlot->GetName() + ".root", "root");
        accTimesEffPlot->Print(plotDir + accTimesEffPlot->GetName() + ".root", "root");
        accTimesEffPlotEB->Print(plotDir + accTimesEffPlotEB->GetName() + ".root", "root");
        accTimesEffPlotEE->Print(plotDir + accTimesEffPlotEE->GetName() + ".root", "root");
        accTimesEffObjPlot->Print(plotDir + accTimesEffObjPlot->GetName() + ".root", "root");
        accTimesEffNoTrgObjPlot->Print(plotDir + accTimesEffNoTrgObjPlot->GetName() + ".root", "root");
        effAftTrgPlot->Print(plotDir + effAftTrgPlot->GetName() + ".root", "root");
        trgRecoVsRecoPlot->Print(plotDir + trgRecoVsRecoPlot->GetName() + ".root", "root");
        accPlot->Print(plotDir + accPlot->GetName() + ".root", "root");
     }
   }

   // write histos to file
   output->cd();
   hGenEvts->Write();
   hGenEvtsEleInAcc->Write();
   hGenEvtsEleInAccEB->Write();
   hGenEvtsEleInAccEE->Write();
   hGenEvtsMuInAcc->Write();
   hGenEvtsInAcc->Write();
   hTrgEvts->Write();
   hRecoEvts->Write();
   hRecoEvtsEB->Write();
   hRecoEvtsEE->Write();
   hRecoEleEvts->Write();
   hRecoEleEvtsEB->Write();
   hRecoEleEvtsEE->Write();
   hRecoMuEvts->Write();
   hAccEle->Write();
   hAccEleEB->Write();
   hAccEleEE->Write();
   hAccMu->Write();
   hAccTimesTrgEff->Write();
   hTrgEff->Write();
   hAccTimesEff->Write();
   hAccTimesEffEB->Write();
   hAccTimesEffEE->Write();
   hAccTimesEffEle->Write();
   hAccTimesEffEleEB->Write();
   hAccTimesEffEleEE->Write();
   hAccTimesEffMu->Write();
   hEffAftTrg->Write();
   hEffAftTrgEle->Write();
   hEffAftTrgEleEB->Write();
   hEffAftTrgEleEE->Write();
   hEffAftTrgMu->Write();
   fitFunc->Write();
   fitFuncEB->Write();
   fitFuncEE->Write();

   output->Close();
   timer.Stop();
   timer.Print();
}

bool AccTimesEff::PassHEEP(const int &n)
{
   // HEEPv4.1
   // barrel
   if (fabs(gsfsc_eta[n]) < 1.442
       && gsf_gsfet[n] > bar_et
       && gsf_isecaldriven[n]
       && gsf_hovere[n] < bar_hoE
       && fabs(gsf_deltaeta[n]) < bar_DEta
       && fabs(gsf_deltaphi[n]) < bar_DPhi
       && ((gsf_e2x5overe5x5[n] > bar_e2x5e5x5) || (gsf_e1x5overe5x5[n] > bar_e1x5e5x5))
       && (gsf_ecaliso[n] + gsf_hcaliso1[n]) < (bar_isoEcalHcal1_1 + bar_isoEcalHcal1_2 * gsf_gsfet[n] + bar_isoEcalHcalRho * rho)
       && gsf_trackiso[n] < bar_isoTrack
       && gsf_nLostInnerHits[n] <= bar_missInnerHits
       && fabs(gsf_dxy_firstPVtx[n]) <= bar_dxy
      ) return true;

   // endcap
   if ((fabs(gsfsc_eta[n]) > 1.56 && fabs(gsfsc_eta[n]) < 2.5)
       && gsf_gsfet[n] > end_et
       && gsf_isecaldriven[n]
       && gsf_hovere[n] < end_hoE
       && fabs(gsf_deltaeta[n]) < end_DEta
       && fabs(gsf_deltaphi[n]) < end_DPhi
       && gsf_sigmaIetaIeta[n] < end_sigmaietaieta
       && ((gsf_gsfet[n] < 50. && (gsf_ecaliso[n] + gsf_hcaliso1[n]) < (end_isoEcalHcal1_1_1 + end_isoEcalHcalRho * rho))
          || (gsf_gsfet[n] >= 50. && (gsf_ecaliso[n] + gsf_hcaliso1[n]) < (end_isoEcalHcal1_1_2 + end_isoEcalHcal1_2 * gsf_gsfet[n] + end_isoEcalHcalRho * rho)))
       && gsf_trackiso[n] < end_isoTrack
       && gsf_nLostInnerHits[n] <= end_missInnerHits
       && fabs(gsf_dxy_firstPVtx[n]) <= end_dxy
      ) return true;
   return false;
}

bool AccTimesEff::PassHighPtMu(const int &n)
{
   if (muon_pt[n] > muon_pt_min
       && muon_ptError[n] / muon_pt[n] < muon_dptOverPt
       && fabs(muon_eta[n]) < muon_etaMax
       && muon_nhitstrack[n] >= muon_nHitsMinGlobal
       && muon_nhitspixel[n] >= muon_nHitsMinPixel
       && muon_nhitsmuons[n] >= muon_nHitsMinMuon
       && muon_nlayerswithhits[n] >= muon_nLayersMin
       && fabs(muon_dxy_firstPVtx[n]) < muon_impactParamMaxXY
       //&& fabs(muon_dz_firstPVtx[n]) < muon_impactParamMaxZ
       && muon_nSegmentMatch[n] >= muon_nSegMatchMin
       && muon_isTrackerMuon[n]
       && muon_trackIso03[n] / muon_pt[n] < muon_relIsoCutMax
      ) return true;

   return false;
}


