#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"
#include "THStack.h"
#include "TColor.h"
#include "TString.h"
#include "TLegend.h"
#include "TParameter.h"
#include "TLatex.h"
#include "TStyle.h"
#include "THashList.h"

TH1F * MakeHistoFromBranch(TFile *input, const char *treeName, const char *brName, int signs, int region, const char *cutVariable, float cutLow, float cutHigh, vector<float> &binning, unsigned int flags, bool normToBinWidth = false, float userScale = 1.);

// class for plotting of control variable plots
class ContVarPlot {
  public:
    TString fName;
    TString fTitle;
    TString fXaxisTitle;
    float fXmin;
    float fXmax;
    int fNbins;
    bool fPlotQcd;
    bool fLogPlot;
    bool fOverFlow;
    bool fUnderFlow;

    ContVarPlot(const char* name, const char* title, const char* xAxisTitle, float xmin, float xmax, int nBins, bool plotQcd=1, bool logPlot=0, bool overFlow=0, bool underFlow=0) : fName(name), fTitle(title), fXaxisTitle(xAxisTitle), fXmin(xmin), fXmax(xmax), fNbins(nBins), fPlotQcd(plotQcd), fLogPlot(logPlot), fOverFlow(overFlow), fUnderFlow(underFlow) { } 
    ~ContVarPlot() { }
};

void macro_MakeTestPlot(unsigned int var = 0, int sig = 0, unsigned int reg = 0)
{ 
  // parameters //////////////////////////////////////////////////////////////
  TFile input("./emuSpec_19703pb-1.root", "open");
  input.cd();

  TParameter<float> *lumi = (TParameter<float> *)input.Get("lumi");

  const bool plotPull = 1; // plot (data-bkg)/bkg
  const bool pullGridY = 1; // grid lines on y axis of pull plot
  const bool prelim = 1; // print Preliminary

  float yRangeMinRatio = -0.7;
  float yRangeMaxRatio = 0.7;

  // plot style
  int ttbarColour = TColor::GetColor("#ff6666");
  int zttColour = TColor::GetColor("#ff4d4d");
  int wwColour = TColor::GetColor("#ff3333");
  int wzColour = TColor::GetColor("#ff0f0f");
  int zzColour = TColor::GetColor("#eb0000");
  int twColour = TColor::GetColor("#db0000");
  int zmmColour=  TColor::GetColor("#66b3ff");
  int zeeColour=  TColor::GetColor("#99ccff");
  int wjetColour=  TColor::GetColor("#ffd324");
  int jetBkgColour = TColor::GetColor("#ffff66");

  // output file formats
  const bool saveSpec = 0;
  const bool saveAsPdf = 0;
  const bool saveAsPng = 1;
  const bool saveAsRoot = 0;
  const char *fileNameExtra = "";
  //const char *fileNameExtra = "madgraphTTbar_";
  const char *plotDir = "./plottest/";

  int font = 42; //62
  ////////////////////////////////////////////////////////////////////////////

  // to keep the histogram when the file is closed
  TH1::AddDirectory(kFALSE);
  TH1::SetDefaultSumw2(kTRUE);

  std::vector<ContVarPlot> testPlots;
  testPlots.reserve(40);
  // flags: plotQCD | logPlot | underflow in first bin | overflow in last bin
  testPlots.push_back(ContVarPlot("mass", "e-mu invariant mass", "m(e#mu) [GeV]", 0., 1500., 75, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("pfMet", "PF MET", "E^{T}_{miss} [GeV]", 0., 500., 50, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("nVtx", "Number of primary vertices", "# PV", 0., 50., 50, 1, 0, 1, 1));
  testPlots.push_back(ContVarPlot("rho", "rho", "#rho", 0., 50., 50, 1, 0, 1, 1));
  testPlots.push_back(ContVarPlot("numOfJetsPt20", "Number of jets > 20 GeV", "# jets_{p_{T}>20}", 0., 20., 20, 1, 0, 1, 1));
  testPlots.push_back(ContVarPlot("numOfJetsPt30", "Number of jets > 30 GeV", "# jets_{p_{T}>30}", 0., 15., 15, 1, 0, 1, 1));
  testPlots.push_back(ContVarPlot("eDxyMinusMuDxy", "|Dxy_e - Dxy_mu|", "|#Deltaxy_{e}-#Deltaxy_{#mu}|", 0., 0.06, 40, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("eDzMinusMuDz", "|Dz_e - Dz_mu|", "|#Deltaz_{e}-#Deltaz_{#mu}|", 0., 0.5, 40, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("dEta", "|eta_e - eta_mu|", "|#eta_{e}-#eta_{#mu}|", 0., 5., 50, 1, 0, 1, 1));
  testPlots.push_back(ContVarPlot("dPhi", "|phi_e - phi_mu|", "|#varphi_{e}-#varphi_{#mu}|", 0., 3.2, 64, 1, 0, 1, 1));
  testPlots.push_back(ContVarPlot("eleEt", "Electron Et", "E_{T}^{e} [GeV]", 0., 500., 50, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("eleEta", "Electron eta", "#eta_{e}", -2.5, 2.5, 50, 1, 0, 1, 1));
  testPlots.push_back(ContVarPlot("elePhi", "Electron phi", "#phi_{e}", -3.2, 3.2, 64, 1, 0, 1, 1));
  testPlots.push_back(ContVarPlot("eleDEta", "Electron dEta", "#Delta#eta", -0.007, 0.007, 29, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("eleDPhi", "Electron dPhi", "#Delta#varphi", -0.06, 0.06, 50, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("eleHOE", "Electron H/E", "H/E", 0., 0.051, 51, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("eleE1x5overE5x5", "Electron E1x5/E5x5", "E1x5/E5x5", 0., 1.5, 30, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("eleE2x5overE5x5", "Electron E2x5/E5x5", "E2x5/E5x5", 0., 1.5, 30, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("eleSigmaIEIE", "Electron #sigma_i#etai#eta", "", 0., 0.04, 40, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("eleEcalIso", "Electron ECAL iso", "ECAL iso", 0., 15., 30, 1, 0, 1, 1));
  testPlots.push_back(ContVarPlot("eleHcalIso1", "Electron HCAL iso1", "HCAL iso1", 0., 15., 30, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("eleHcalIso2", "Electron HCAL iso2", "HCAL iso2", 0., 15., 30, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("eleTrkIso", "Electron track iso", "trk_{iso}", 0., 5.1, 51, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("eleLostHits", "Electron number of lost hits", "", 0., 2., 2, 1, 0, 1, 1));
  testPlots.push_back(ContVarPlot("eleDXYFstPVtx", "Electron Dxy at first PV", "#Deltaxy_{e}", -0.05, 0.05, 40, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("eleDZFstPVtx", "Electron Dz at first PV", "#Deltaz_{e}", -1., 1., 80, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("etEleOPtMu", "Et_e / pt_mu", "E^{e}_{T} / p^{#mu}_{T}", 0., 13., 26, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("lepPtPlusOPtMinus", "lepton pt+ / pt-", "p^{+}_{T}/p^{-}_{T}", 0., 10., 20, 0, 1, 1, 1));
  testPlots.push_back(ContVarPlot("eChTimesMuCh", "charge_e * charge_mu", "charge_{e}*charge_{#mu}", -1., 2., 3, 0, 1, 0, 0));
  testPlots.push_back(ContVarPlot("muPt", "Muon pt", "p_{T}^{#mu} [GeV]", 0., 500., 50, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("muPtErr", "Muon pt error", "p_{T}^{#mu} [GeV]", 0., 50., 50, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("muEta", "Muon eta", "#eta_{#mu}", -2.5, 2.5, 50, 1, 0, 1, 1));
  testPlots.push_back(ContVarPlot("muPhi", "Muon phi", "#varphi_{#mu}", -3.2, 3.2, 64, 1, 0, 1, 1));
  testPlots.push_back(ContVarPlot("muHitLayers", "Muon layers with hits", "# layers hits", 4., 20., 16, 1, 0, 1, 1));
  testPlots.push_back(ContVarPlot("muTrkHits", "Muon tracker hits", "# tracker hits", 0., 40., 40, 1, 0, 1, 1));
  testPlots.push_back(ContVarPlot("muPxlHits", "Muon pixel hits", "# pixel hits", 0., 10., 10, 1, 0, 1, 1));
  testPlots.push_back(ContVarPlot("muMuHits", "Muon system hits", "# #mu system hits", 0., 55., 55, 1, 0, 1, 1));
  testPlots.push_back(ContVarPlot("muDXYFstPVtx", "Muon Dxy at first PV", "#Deltaxy_{#mu}", -0.2, 0.2, 80, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("muDZFstPVtx", "Muon Dz at first PV", "#Deltaz_{#mu}", -0.1, 0.1, 40, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("muNSeg", "Muon number of segments", "# segments", 0., 7., 7, 1, 0, 1, 1));
  testPlots.push_back(ContVarPlot("muTrkIso03", "Muon track iso 03", "#mu trk iso", 0., 20., 40, 1, 1, 1, 1));
  testPlots.push_back(ContVarPlot("muIsoCombRel", "Muon combined iso / pt", "#mu (iso_{em}+iso_{had}+iso_{trk})/p_{T}", 0., 3.5, 35, 1, 1, 1, 1));

  TString sign[7] = {"_e-mu+", "_e+mu-", "_OS", "", "_SS", "_++", "_--"};
  TString nameSign[7] = {" e-mu+", " e+mu-", " OS", "", " SS", " ++", " --"};
  TString region[3] = {"", "_EB", "_EE"};
  TString nameReg[3] = {"", " EB", " EE"};

  // plot a list with possible test histograms
  if (var > testPlots.size()) var = 0;
  if (var == 0) {
    cout << "Use macro_MakeTestPlot.C(x, y, z) with x {1-" << testPlots.size() << "} being the \n";
    cout << "number of the test histogram to plot, y {-3 - +3} selects the charge with \n";
    cout << "the scheme e-mu: -+, +-, OS, ALL, SS, ++, -- and z {0-2} selects the \n";
    cout << "detector EB+EE, EB or EE events for the electron." << endl;
    cout << "-----------------------------------" << endl;
    for (unsigned int i = 0; i < testPlots.size(); ++i) {
      cout << testPlots[i].fTitle;
      unsigned int j = 35 - testPlots[i].fTitle.Sizeof();
      if (i > 8) --j;
      for (; j > 0; --j)
        cout << " ";
      cout << i + 1 << endl;
    }
    cout << "-----------------------------------" << endl;
    cout << "Use PlotRange(y, z, x-start, x-stop) to plot a range of test histograms." << endl;
    input.Close();
    return;
  }
  --var;
  // sanity checks for sign and region input
  if (abs(sig) > 3) sig = 0;
  if (reg > 2) reg = 0;

  std::vector<TH1F *> emuTest_data;
  std::vector<TH1F *> emuTest_ttbar;
  std::vector<TH1F *> emuTest_ztautau;
  std::vector<TH1F *> emuTest_ww;
  std::vector<TH1F *> emuTest_wz;
  std::vector<TH1F *> emuTest_zz;
  std::vector<TH1F *> emuTest_tw;
  std::vector<TH1F *> emuTest_zmumu;
  std::vector<TH1F *> emuTest_zee;
  std::vector<TH1F *> emuTest_wjets;
  std::vector<TH1F *> emuTest_qcd;

  input.cd();

  // configure plot style
  TString testVar = testPlots[var].fName;
  bool plotQcd = testPlots[var].fPlotQcd;
  bool logPlot = testPlots[var].fLogPlot;
  const bool overflowBin = testPlots[var].fOverFlow;
  const bool underflowBin = testPlots[var].fUnderFlow; 

  // make the correct binning
  std::vector<float> binning;
  float lowBin = testPlots[var].fXmin;
  float highBin = testPlots[var].fXmax;
  int nBins = testPlots[var].fNbins;
  for (float bin = lowBin; bin <= highBin; bin += (highBin - lowBin) / nBins)
    binning.push_back(bin);

  float totMcWeight = 1.;
  THashList *mcWeights = (THashList *)input.Get("mcWeights");
  TParameter<float> *mcWeight = (TParameter<float> *)mcWeights->FindObject("ttbar");
  TParameter<float> *mcWeight700to1000 = (TParameter<float> *)mcWeights->FindObject("ttbar700to1000");
  TParameter<float> *mcWeight1000up = (TParameter<float> *)mcWeights->FindObject("ttbar1000up");

  // determine qcd contribution
  TH1F *ssData;
  TH1F *ssBg;
  TH1F *qcdContrib;
  if (plotQcd) {
    ssData = MakeHistoFromBranch(&input, "emuTree_data", testVar, 1, reg, "", 0., 0., binning, 0x100);
    ssBg = MakeHistoFromBranch(&input, "emuTree_ttbar", testVar, 1, reg, "genMTtbar", 0., 700., binning, 0x1DF);
    totMcWeight = 1. / (1 / mcWeight->GetVal() + 1 / mcWeight700to1000->GetVal());
    ssBg->Add(MakeHistoFromBranch(&input, "emuTree_ttbar", testVar, 1, reg, "genMTtbar", 700., 1000., binning, 0x19F), totMcWeight);
    ssBg->Add(MakeHistoFromBranch(&input, "emuTree_ttbar700to1000", testVar, 1, reg, "genMTtbar", 700., 1000., binning, 0x19F), totMcWeight);
    totMcWeight = 1. / (1 / mcWeight->GetVal() + 1 / mcWeight1000up->GetVal());
    ssBg->Add(MakeHistoFromBranch(&input, "emuTree_ttbar", testVar, 1, reg, "genMTtbar", 1000., 1000000000., binning, 0x19F), totMcWeight);
    ssBg->Add(MakeHistoFromBranch(&input, "emuTree_ttbar1000up", testVar, 1, reg, "genMTtbar", 1000., 1000000000., binning, 0x19F), totMcWeight);
    //TH1F *ssBg = MakeHistoFromBranch(&input, "emuTree_ttbar", testVar, 1, reg, "", 0., 0., binning, 0x1DF);
    //TH1F *ssBg = MakeHistoFromBranch(&input, "emuTree_ttbarto2l", testVar, 1, reg, "", 0., 0., binning, 0x1DF);
    ssBg->Add(MakeHistoFromBranch(&input, "emuTree_ztautau", testVar, 1, reg, "", 0., 0., binning, 0x1DF));
    ssBg->Add(MakeHistoFromBranch(&input, "emuTree_ww", testVar, 1, reg, "", 0., 0., binning, 0x1DF));
    ssBg->Add(MakeHistoFromBranch(&input, "emuTree_wz", testVar, 1, reg, "", 0., 0., binning, 0x1DF));
    ssBg->Add(MakeHistoFromBranch(&input, "emuTree_zz", testVar, 1, reg, "", 0., 0., binning, 0x1DF));
    ssBg->Add(MakeHistoFromBranch(&input, "emuTree_tw", testVar, 1, reg, "", 0., 0., binning, 0x1DF));
    ssBg->Add(MakeHistoFromBranch(&input, "emuTree_zmumu", testVar, 1, reg, "", 0., 0., binning, 0x1DF));
    ssBg->Add(MakeHistoFromBranch(&input, "emuTree_zee", testVar, 1, reg, "", 0., 0., binning, 0x1DF));
    ssBg->Add(MakeHistoFromBranch(&input, "emuTree_wjets", testVar, 1, reg, "", 0., 0., binning, 0x1DF));
    qcdContrib = (TH1F *)ssData->Clone("qcdContrib_SS");
    qcdContrib->Add(ssBg, -1);
    for (int i = 0; i < qcdContrib->GetNbinsX() + 2; ++i) {
      if (qcdContrib->GetBinContent(i) < 0) qcdContrib->SetBinContent(i, 0.);
    }
    cout << "Expected SS QCD events: " << ssData->Integral() - ssBg->Integral();
    cout << "; Derived SS QCD events: " << qcdContrib->Integral();
    cout << "; scale factor: " << (ssData->Integral() - ssBg->Integral()) / qcdContrib->Integral()<< endl;
    qcdContrib->Scale((ssData->Integral() - ssBg->Integral()) / qcdContrib->Integral());

    emuTest_qcd.push_back((TH1F *)qcdContrib->Clone(testVar + sign[sig] + nameReg[reg] + "qcd"));
    if (sig == 0) emuTest_qcd.back()->Scale(2.);
    else if (abs(sig) > 1) emuTest_qcd.back()->Scale(0.5);
  }

  // get the histograms
  TH1F *dataOverBgHist = (TH1F *)MakeHistoFromBranch(&input, "emuTree_data", testVar, sig, reg, "", 0., 0., binning, 0x100);
  emuTest_data.push_back((TH1F *)dataOverBgHist->Clone());
  TH1F *ttbarComb = MakeHistoFromBranch(&input, "emuTree_ttbar", testVar, sig, reg, "genMTtbar", 0., 700., binning, 0x1DF);
  totMcWeight = 1. / (1 / mcWeight->GetVal() + 1 / mcWeight700to1000->GetVal());
  ttbarComb->Add(MakeHistoFromBranch(&input, "emuTree_ttbar", testVar, sig, reg, "genMTtbar", 700., 1000., binning, 0x19F), totMcWeight);
  ttbarComb->Add(MakeHistoFromBranch(&input, "emuTree_ttbar700to1000", testVar, sig, reg, "genMTtbar", 700., 1000., binning, 0x19F), totMcWeight);
  totMcWeight = 1. / (1 / mcWeight->GetVal() + 1 / mcWeight1000up->GetVal());
  ttbarComb->Add(MakeHistoFromBranch(&input, "emuTree_ttbar", testVar, sig, reg, "genMTtbar", 1000., 1000000000., binning, 0x19F), totMcWeight);
  ttbarComb->Add(MakeHistoFromBranch(&input, "emuTree_ttbar1000up", testVar, sig, reg, "genMTtbar", 1000., 1000000000., binning, 0x19F), totMcWeight);
  emuTest_ttbar.push_back(ttbarComb);
  //emuTest_ttbar.push_back(MakeHistoFromBranch(&input, "emuTree_ttbar", testVar, sig, reg, "", 0., 0., binning, 0x1DF));
  //emuTest_ttbar.push_back(MakeHistoFromBranch(&input, "emuTree_ttbarto2l", testVar, sig, reg, "", 0., 0., binning, 0x1DF));
  emuTest_ztautau.push_back(MakeHistoFromBranch(&input, "emuTree_ztautau", testVar, sig, reg, "", 0., 0., binning, 0x1DF));
  emuTest_ww.push_back(MakeHistoFromBranch(&input, "emuTree_ww", testVar, sig, reg, "", 0., 0., binning, 0x1DF));
  emuTest_wz.push_back(MakeHistoFromBranch(&input, "emuTree_wz", testVar, sig, reg, "", 0., 0., binning, 0x1DF));
  emuTest_zz.push_back(MakeHistoFromBranch(&input, "emuTree_zz", testVar, sig, reg, "", 0., 0., binning, 0x1DF));
  emuTest_tw.push_back(MakeHistoFromBranch(&input, "emuTree_tw", testVar, sig, reg, "", 0., 0., binning, 0x1DF));
  emuTest_zmumu.push_back(MakeHistoFromBranch(&input, "emuTree_zmumu", testVar, sig, reg, "", 0., 0., binning, 0x1DF));
  emuTest_zee.push_back(MakeHistoFromBranch(&input, "emuTree_zee", testVar, sig, reg, "", 0., 0., binning, 0x1DF));
  emuTest_wjets.push_back(MakeHistoFromBranch(&input, "emuTree_wjets", testVar, sig, reg, "", 0., 0., binning, 0x1DF));

  sig+=3;

  // add overflow to last bin
  if (overflowBin) {
    dataOverBgHist->SetBinContent(dataOverBgHist->GetNbinsX(), dataOverBgHist->GetBinContent(dataOverBgHist->GetNbinsX()) + dataOverBgHist->GetBinContent(dataOverBgHist->GetNbinsX() + 1));
    emuTest_data.back()->SetBinContent(emuTest_data.back()->GetNbinsX(), emuTest_data.back()->GetBinContent(emuTest_data.back()->GetNbinsX()) + emuTest_data.back()->GetBinContent(emuTest_data.back()->GetNbinsX() + 1));
    emuTest_ttbar.back()->SetBinContent(emuTest_ttbar.back()->GetNbinsX(), emuTest_ttbar.back()->GetBinContent(emuTest_ttbar.back()->GetNbinsX()) + emuTest_ttbar.back()->GetBinContent(emuTest_ttbar.back()->GetNbinsX() + 1));
    emuTest_ztautau.back()->SetBinContent(emuTest_ztautau.back()->GetNbinsX(), emuTest_ztautau.back()->GetBinContent(emuTest_ztautau.back()->GetNbinsX()) + emuTest_ztautau.back()->GetBinContent(emuTest_ztautau.back()->GetNbinsX() + 1));
    emuTest_ww.back()->SetBinContent(emuTest_ww.back()->GetNbinsX(), emuTest_ww.back()->GetBinContent(emuTest_ww.back()->GetNbinsX()) + emuTest_ww.back()->GetBinContent(emuTest_ww.back()->GetNbinsX() + 1));
    emuTest_wz.back()->SetBinContent(emuTest_wz.back()->GetNbinsX(), emuTest_wz.back()->GetBinContent(emuTest_wz.back()->GetNbinsX()) + emuTest_wz.back()->GetBinContent(emuTest_wz.back()->GetNbinsX() + 1));
    emuTest_zz.back()->SetBinContent(emuTest_zz.back()->GetNbinsX(), emuTest_zz.back()->GetBinContent(emuTest_zz.back()->GetNbinsX()) + emuTest_zz.back()->GetBinContent(emuTest_zz.back()->GetNbinsX() + 1));
    emuTest_tw.back()->SetBinContent(emuTest_tw.back()->GetNbinsX(), emuTest_tw.back()->GetBinContent(emuTest_tw.back()->GetNbinsX()) + emuTest_tw.back()->GetBinContent(emuTest_tw.back()->GetNbinsX() + 1));
    emuTest_zmumu.back()->SetBinContent(emuTest_zmumu.back()->GetNbinsX(), emuTest_zmumu.back()->GetBinContent(emuTest_zmumu.back()->GetNbinsX()) + emuTest_zmumu.back()->GetBinContent(emuTest_zmumu.back()->GetNbinsX() + 1));
    emuTest_zee.back()->SetBinContent(emuTest_zee.back()->GetNbinsX(), emuTest_zee.back()->GetBinContent(emuTest_zee.back()->GetNbinsX()) + emuTest_zee.back()->GetBinContent(emuTest_zee.back()->GetNbinsX() + 1));
    emuTest_wjets.back()->SetBinContent(emuTest_wjets.back()->GetNbinsX(), emuTest_wjets.back()->GetBinContent(emuTest_wjets.back()->GetNbinsX()) + emuTest_wjets.back()->GetBinContent(emuTest_wjets.back()->GetNbinsX() + 1));
    if (plotQcd) emuTest_qcd.back()->SetBinContent(emuTest_qcd.back()->GetNbinsX(), emuTest_qcd.back()->GetBinContent(emuTest_qcd.back()->GetNbinsX()) + emuTest_qcd.back()->GetBinContent(emuTest_qcd.back()->GetNbinsX() + 1));

    dataOverBgHist->SetBinError(dataOverBgHist->GetNbinsX(), sqrt(dataOverBgHist->GetBinContent(dataOverBgHist->GetNbinsX())));
    emuTest_data.back()->SetBinError(emuTest_data.back()->GetNbinsX(), sqrt(emuTest_data.back()->GetBinContent(emuTest_data.back()->GetNbinsX())));
    emuTest_ttbar.back()->SetBinError(emuTest_ttbar.back()->GetNbinsX(), sqrt(emuTest_ttbar.back()->GetBinContent(emuTest_ttbar.back()->GetNbinsX())));
    emuTest_ztautau.back()->SetBinError(emuTest_ztautau.back()->GetNbinsX(), sqrt(emuTest_ztautau.back()->GetBinContent(emuTest_ztautau.back()->GetNbinsX())));
    emuTest_ww.back()->SetBinError(emuTest_ww.back()->GetNbinsX(), sqrt(emuTest_ww.back()->GetBinContent(emuTest_ww.back()->GetNbinsX())));
    emuTest_wz.back()->SetBinError(emuTest_wz.back()->GetNbinsX(), sqrt(emuTest_wz.back()->GetBinContent(emuTest_wz.back()->GetNbinsX())));
    emuTest_zz.back()->SetBinError(emuTest_zz.back()->GetNbinsX(), sqrt(emuTest_zz.back()->GetBinContent(emuTest_zz.back()->GetNbinsX())));
    emuTest_tw.back()->SetBinError(emuTest_tw.back()->GetNbinsX(), sqrt(emuTest_tw.back()->GetBinContent(emuTest_tw.back()->GetNbinsX())));
    emuTest_zmumu.back()->SetBinError(emuTest_zmumu.back()->GetNbinsX(), sqrt(emuTest_zmumu.back()->GetBinContent(emuTest_zmumu.back()->GetNbinsX())));
    emuTest_zee.back()->SetBinError(emuTest_zee.back()->GetNbinsX(), sqrt(emuTest_zee.back()->GetBinContent(emuTest_zee.back()->GetNbinsX())));
    emuTest_wjets.back()->SetBinError(emuTest_wjets.back()->GetNbinsX(), sqrt(emuTest_wjets.back()->GetBinContent(emuTest_wjets.back()->GetNbinsX())));
    if (plotQcd) emuTest_qcd.back()->SetBinError(emuTest_qcd.back()->GetNbinsX(), sqrt(emuTest_qcd.back()->GetBinContent(emuTest_qcd.back()->GetNbinsX())));
  }
  // add underflow to first bin
  if (underflowBin) {
    dataOverBgHist->SetBinContent(1, dataOverBgHist->GetBinContent(1) + dataOverBgHist->GetBinContent(0));
    emuTest_data.back()->SetBinContent(1, emuTest_data.back()->GetBinContent(1) + emuTest_data.back()->GetBinContent(0));
    emuTest_ttbar.back()->SetBinContent(1, emuTest_ttbar.back()->GetBinContent(1) + emuTest_ttbar.back()->GetBinContent(0));
    emuTest_ztautau.back()->SetBinContent(1, emuTest_ztautau.back()->GetBinContent(1) + emuTest_ztautau.back()->GetBinContent(0));
    emuTest_ww.back()->SetBinContent(1, emuTest_ww.back()->GetBinContent(1) + emuTest_ww.back()->GetBinContent(0));
    emuTest_wz.back()->SetBinContent(1, emuTest_wz.back()->GetBinContent(1) + emuTest_wz.back()->GetBinContent(0));
    emuTest_zz.back()->SetBinContent(1, emuTest_zz.back()->GetBinContent(1) + emuTest_zz.back()->GetBinContent(0));
    emuTest_tw.back()->SetBinContent(1, emuTest_tw.back()->GetBinContent(1) + emuTest_tw.back()->GetBinContent(0));
    emuTest_zmumu.back()->SetBinContent(1, emuTest_zmumu.back()->GetBinContent(1) + emuTest_zmumu.back()->GetBinContent(0));
    emuTest_zee.back()->SetBinContent(1, emuTest_zee.back()->GetBinContent(1) + emuTest_zee.back()->GetBinContent(0));
    emuTest_wjets.back()->SetBinContent(1, emuTest_wjets.back()->GetBinContent(1) + emuTest_wjets.back()->GetBinContent(0));
    if (plotQcd) emuTest_qcd.back()->SetBinContent(1, emuTest_qcd.back()->GetBinContent(1) + emuTest_qcd.back()->GetBinContent(0));

    dataOverBgHist->SetBinError(1, sqrt(dataOverBgHist->GetBinContent(1)));
    emuTest_data.back()->SetBinError(1, sqrt(emuTest_data.back()->GetBinContent(1)));
    emuTest_ttbar.back()->SetBinError(1, sqrt(emuTest_ttbar.back()->GetBinContent(1)));
    emuTest_ztautau.back()->SetBinError(1, sqrt(emuTest_ztautau.back()->GetBinContent(1)));
    emuTest_ww.back()->SetBinError(1, sqrt(emuTest_ww.back()->GetBinContent(1)));
    emuTest_wz.back()->SetBinError(1, sqrt(emuTest_wz.back()->GetBinContent(1)));
    emuTest_zz.back()->SetBinError(1, sqrt(emuTest_zz.back()->GetBinContent(1)));
    emuTest_tw.back()->SetBinError(1, sqrt(emuTest_tw.back()->GetBinContent(1)));
    emuTest_zmumu.back()->SetBinError(1, sqrt(emuTest_zmumu.back()->GetBinContent(1)));
    emuTest_zee.back()->SetBinError(1, sqrt(emuTest_zee.back()->GetBinContent(1)));
    emuTest_wjets.back()->SetBinError(1, sqrt(emuTest_wjets.back()->GetBinContent(1)));
    if (plotQcd) emuTest_qcd.back()->SetBinError(1, sqrt(emuTest_qcd.back()->GetBinContent(1)));
  }

  // make a histogram stack with the bg 
  THStack *bgStack = new THStack("bgStack" + sign[sig] + region[reg], testVar + sign[sig] + nameReg[reg]);
  if (plotQcd) bgStack->Add(emuTest_qcd.back());
  bgStack->Add(emuTest_wjets.back());
  bgStack->Add(emuTest_zee.back());
  bgStack->Add(emuTest_zmumu.back());
  bgStack->Add(emuTest_tw.back());
  bgStack->Add(emuTest_zz.back());
  bgStack->Add(emuTest_wz.back());
  bgStack->Add(emuTest_ww.back());
  bgStack->Add(emuTest_ztautau.back());
  bgStack->Add(emuTest_ttbar.back());

  // bkg histogram for (data-bkg)/bkg plot
  TH1F *bgHist;
  if (plotQcd) {
    bgHist = (TH1F *)emuTest_qcd.back()->Clone();
    bgHist->Add(emuTest_wjets.back());
  } else {
    bgHist = (TH1F *)emuTest_wjets.back()->Clone();
  }
  bgHist->Add(emuTest_zee.back());
  bgHist->Add(emuTest_zmumu.back());
  bgHist->Add(emuTest_tw.back());
  bgHist->Add(emuTest_zz.back());
  bgHist->Add(emuTest_wz.back());
  bgHist->Add(emuTest_ww.back());
  bgHist->Add(emuTest_ztautau.back());
  bgHist->Add(emuTest_ttbar.back());

  //cout << "underflow (data/bkg): " << emuTest_data.back()->GetBinContent(0) << "/" << bgHist->GetBinContent(0) << "  overflow: " << emuTest_data.back()->GetBinContent(emuTest_data.back()->GetNbinsX()+1) << "/" << bgHist->GetBinContent(bgHist->GetNbinsX()+1) << endl;

  TCanvas *emuPlot;
  TPad *specPad;
  if (plotPull) {
    emuPlot = new TCanvas("emuPlot" + testVar + sign[sig] + region[reg], "emu Spectrum" + testVar + nameSign[sig] + nameReg[reg], 100, 100, 900, 740);
    specPad = new TPad("specPad" + testVar + + sign[sig] + region[reg], "emu Spectrum" + testVar + nameSign[sig] + nameReg[reg], 0., 0.33, 1., 1.);
    specPad->SetBottomMargin(0.06);
  } else {
    emuPlot = new TCanvas("emuPlot" + testVar + sign[sig] + region[reg], "emu Spectrum" + testVar + nameSign[sig] + nameReg[reg], 100, 100, 900, 600);
    specPad = new TPad("specPad" + testVar + sign[sig] + region[reg], "emu Spectrum" + testVar + nameSign[sig] + nameReg[reg], 0., 0., 1., 1.);
    specPad->SetBottomMargin(0.12);
  }
  specPad->SetBorderMode(0);
  specPad->SetBorderSize(2);
  specPad->SetFrameBorderMode(0);
  specPad->SetFillColor(0);
  specPad->SetFrameFillColor(0);
  if (logPlot) specPad->SetLogy();
  specPad->SetLeftMargin(0.11);
  specPad->SetRightMargin(0.09);
  specPad->SetTopMargin(0.08);
  specPad->SetTickx(1);
  specPad->SetTicky(1);
  specPad->Draw();
  specPad->cd();

  gStyle->SetTitleFont(font);
  gStyle->SetLabelFont(font);
  gStyle->SetLegendFont(font);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetTitleXOffset(1.);
  gStyle->SetTitleYOffset(1.3);
  gPad->SetTicks(1, 1);

  // make sure that data and bkg are visible on plot
  if (emuTest_data.back()->GetMaximum() > bgHist->GetMaximum()) {
    if (!logPlot) bgStack->SetMaximum(emuTest_data.back()->GetMaximum() * 1.1);
    else bgStack->SetMaximum(emuTest_data.back()->GetMaximum() * 1.3);
  }

  //// plot spectrum
  emuTest_ttbar.back()->SetFillColor(ttbarColour);
  emuTest_ttbar.back()->SetLineColor(kBlack);
  emuTest_ttbar.back()->SetLineWidth(2);
  emuTest_ztautau.back()->SetFillColor(zttColour);
  emuTest_ztautau.back()->SetMarkerColor(zttColour);
  emuTest_ztautau.back()->SetLineColor(kBlack);
  emuTest_ztautau.back()->SetLineWidth(2);
  emuTest_ww.back()->SetFillColor(wwColour);
  emuTest_ww.back()->SetMarkerColor(wwColour);
  emuTest_ww.back()->SetLineColor(kBlack);
  emuTest_ww.back()->SetLineWidth(2);
  emuTest_wz.back()->SetFillColor(wzColour);
  emuTest_wz.back()->SetMarkerColor(wzColour);
  emuTest_wz.back()->SetLineColor(kBlack);
  emuTest_wz.back()->SetLineWidth(2);
  emuTest_zz.back()->SetFillColor(zzColour);
  emuTest_zz.back()->SetMarkerColor(zzColour);
  emuTest_zz.back()->SetLineColor(kBlack);
  emuTest_zz.back()->SetLineWidth(2);
  emuTest_tw.back()->SetFillColor(twColour);
  emuTest_tw.back()->SetMarkerColor(twColour);
  emuTest_tw.back()->SetLineColor(kBlack);
  emuTest_tw.back()->SetLineWidth(2);
  emuTest_zmumu.back()->SetFillColor(zmmColour);
  emuTest_zmumu.back()->SetMarkerColor(zmmColour);
  emuTest_zmumu.back()->SetLineColor(kBlack);
  emuTest_zmumu.back()->SetLineWidth(2);
  emuTest_zee.back()->SetFillColor(zeeColour);
  emuTest_zee.back()->SetMarkerColor(zeeColour);
  emuTest_zee.back()->SetLineColor(kBlack);
  emuTest_zee.back()->SetLineWidth(2);
  emuTest_wjets.back()->SetFillColor(wjetColour);
  emuTest_wjets.back()->SetMarkerColor(wjetColour);
  emuTest_wjets.back()->SetLineColor(kBlack);
  emuTest_wjets.back()->SetLineWidth(2);
  if (plotQcd) {
    emuTest_qcd.back()->SetFillColor(jetBkgColour);
    emuTest_qcd.back()->SetMarkerColor(jetBkgColour);
    emuTest_qcd.back()->SetLineColor(kBlack);
    emuTest_qcd.back()->SetLineWidth(2);
  }

  bgStack->Draw("hist");
  if (!plotPull) bgStack->GetXaxis()->SetTitle(testPlots[var].fXaxisTitle);
  bgStack->GetXaxis()->SetTitleFont(font);
  bgStack->GetXaxis()->SetTitleSize(0.047);
  bgStack->GetXaxis()->SetLabelSize(0.05);
  bgStack->GetXaxis()->SetLabelFont(font);
  bgStack->GetXaxis()->SetMoreLogLabels();
  bgStack->GetXaxis()->SetNoExponent();
  bgStack->GetYaxis()->SetTitle("Events");
  bgStack->GetYaxis()->SetTitleFont(font);
  bgStack->GetYaxis()->SetTitleSize(0.047);
  bgStack->GetYaxis()->SetTitleOffset(1.1);
  bgStack->GetYaxis()->SetLabelFont(font);
  bgStack->GetYaxis()->SetLabelSize(0.05);
  if (!logPlot) bgStack->SetMinimum(0.);
  else bgStack->SetMinimum(0.5);
  bgStack->Draw("hist");

  emuTest_data.back()->SetLineWidth(2);
  emuTest_data.back()->SetLineColor(kBlack);
  emuTest_data.back()->SetMarkerStyle(8);
  emuTest_data.back()->SetMarkerSize(0.8);
  emuTest_data.back()->Draw("same");
  
  // legent and labels
  TLegend legend(0.731, 0.446, 0.921, 0.885);
  legend.SetTextFont(font);
  legend.SetTextSize(0.042);
  legend.SetBorderSize(0);
  legend.SetLineColor(1);
  legend.SetLineStyle(1);
  legend.SetLineWidth(1);
  legend.SetFillColor(19);
  legend.SetFillStyle(0);
  legend.AddEntry(emuTest_data.back(), "DATA");
  legend.AddEntry(emuTest_ttbar.back(), "t#bar{t}", "F");
  legend.AddEntry(emuTest_ztautau.back(), "#gamma/Z#rightarrow#tau#tau", "F");
  legend.AddEntry(emuTest_ww.back(), "WW", "F");
  legend.AddEntry(emuTest_wz.back(), "WZ", "F");
  legend.AddEntry(emuTest_zz.back(), "ZZ", "F");
  legend.AddEntry(emuTest_tw.back(), "tW", "F");
  legend.AddEntry(emuTest_zmumu.back(), "#gamma/Z#rightarrow#mu#mu", "F");
  legend.AddEntry(emuTest_zee.back(), "#gamma/Z #rightarrow ee", "F");
  legend.AddEntry(emuTest_wjets.back(), "W+jets", "F");
  if (plotQcd) legend.AddEntry(emuTest_qcd.back(), "jets (data)", "F");
  legend.SetBorderSize(0);
  legend.DrawClone("same");
  
  TLatex *tex = new TLatex();
  tex->SetNDC();
  tex->SetTextFont(font);
  tex->SetLineWidth(2);
  tex->SetTextSize(0.042);
  if (prelim) tex->DrawLatex(0.596, 0.937, "CMS Preliminary, 8 TeV, 19.7 fb^{-1}");
  else tex->DrawLatex(0.596, 0.937, "CMS, 8 TeV, 19.7 fb^{-1}");
  //if (m == 1) tex->DrawLatex(0.430, 0.849, "e in barrel");
  //if (m == 2) tex->DrawLatex(0.430, 0.849, "e in endcap");
  stringstream sStream;
  sStream << testPlots[var].fTitle << nameSign[sig].Data() << nameReg[reg].Data();
  tex->DrawLatex(0.109, 0.937, sStream.str().c_str());

  // plot a (data - bg)/bg histogram below the spectrum
  if (plotPull) {
    dataOverBgHist->Add(bgHist, -1.);
    dataOverBgHist->Divide(bgHist);

    float fontScaleBot = 1.;
    TPad *pullPad = new TPad("pullPad" + testVar + + sign[sig] + region[reg], "(data - bg) / bg" + testVar + nameSign[sig] + nameReg[reg], 0., 0., 1., 0.33);
    emuPlot->cd();
    fontScaleBot = specPad->GetHNDC() / pullPad->GetHNDC();
    pullPad->Draw();
    pullPad->cd();
    pullPad->SetBorderMode(0);
    pullPad->SetBorderSize(2);
    pullPad->SetFrameBorderMode(0);
    pullPad->SetFillColor(0);
    pullPad->SetFrameFillColor(0);
    pullPad->SetTopMargin(0.);
    pullPad->SetBottomMargin(0.22);
    pullPad->SetLeftMargin(0.11);
    pullPad->SetRightMargin(0.09);
    pullPad->SetTickx(1);
    pullPad->SetTicky(1);
    if (pullGridY) pullPad->SetGridy();

    dataOverBgHist->SetLineWidth(1);
    dataOverBgHist->SetLineColor(kBlack);
    dataOverBgHist->SetMarkerStyle(20);
    dataOverBgHist->SetMarkerSize(1.1);

    dataOverBgHist->GetXaxis()->SetTitle(testPlots[var].fXaxisTitle);
    dataOverBgHist->GetXaxis()->SetTitleFont(font);
    dataOverBgHist->GetXaxis()->SetTitleSize(0.047 * fontScaleBot);
    dataOverBgHist->GetXaxis()->SetTitleOffset(1.);
    dataOverBgHist->GetXaxis()->SetLabelFont(font);
    dataOverBgHist->GetXaxis()->SetLabelSize(0.05 * fontScaleBot);
    dataOverBgHist->GetXaxis()->SetMoreLogLabels();
    dataOverBgHist->GetXaxis()->SetNoExponent();
    //dataOverBgHist->GetXaxis()->SetRangeUser(xRangeMinRatio, xRangeMaxRatio);
    dataOverBgHist->GetYaxis()->SetTitle("(data-bkg)/bkg");
    dataOverBgHist->GetYaxis()->SetTitleFont(font);
    dataOverBgHist->GetYaxis()->SetTitleSize(0.047 * fontScaleBot);
    dataOverBgHist->GetYaxis()->SetTitleOffset(1.1 / fontScaleBot);
    dataOverBgHist->GetYaxis()->SetLabelFont(font);
    dataOverBgHist->GetYaxis()->SetLabelSize(0.05 * fontScaleBot);
    dataOverBgHist->GetYaxis()->SetRangeUser(yRangeMinRatio, yRangeMaxRatio);

    dataOverBgHist->Draw();
  }

  // safe in various file formats
  if (saveSpec) {
    sStream.str("");
    sStream << plotDir << "emuControlSpec_" << testVar << sign[sig] << region[reg] << "_" << fileNameExtra << lumi->GetVal() << "pb-1";
    TString saveFileName = sStream.str();
    if (saveAsPdf) emuPlot->Print(saveFileName + ".pdf", "pdf");
    if (saveAsPng) emuPlot->Print(saveFileName + ".png", "png");
    if (saveAsRoot) emuPlot->Print(saveFileName + ".root", "root");
  }

  cout << testVar.Data() << nameSign[sig].Data() << nameReg[reg].Data() << " plotted" << endl;
}

// plot a range of control variables
void PlotRange(int sign = 0, unsigned int region = 0, unsigned int from = 1, unsigned int to = 42)
{
  if (to == 0 || to > 42) to = 42;
  if (from == 0 || from > to) from = 1;
  for (unsigned int i = from; i <= to; ++i)
    macro_MakeTestPlot(i, sign, region);
}

// flags: [pass Trigger | lumi | MC weight | trigger Eff | trigger MC to data SF | lumi SF|ele SF | mu SF | PU reweight]
TH1F *
MakeHistoFromBranch(TFile *input, const char *treeName, const char *brName, int signs, int region, const char *cutVariable, float cutLow, float cutHigh, vector<float> &binning, unsigned int flags, bool normToBinWidth, float userScale)
{
  input->cd();
  // prepare the histogram
  TH1F *histo = new TH1F("dummy", "dummy", 150, 0., 1500.);
  histo->Sumw2();
  histo->SetName(brName);
  histo->SetTitle(brName);
  float *bins = &binning[0];
  histo->GetXaxis()->Set(binning.size() - 1, bins);

  if (flags & 1<<7) userScale *= ((TParameter<float> *)input->Get("lumi"))->GetVal();
  if (flags & 1<<6) {
    THashList *mcWeights = (THashList *)input->Get("mcWeights");
    userScale *= ((TParameter<float> *)mcWeights->FindObject(treeName + 8))->GetVal();
  }
  float lumiScaleFactorEB = ((TParameter<float> *)input->Get("lumiScaleFactorEB"))->GetVal();
  float lumiScaleFactorEE = ((TParameter<float> *)input->Get("lumiScaleFactorEE"))->GetVal();

  //cout << "Scalefactor = " << userScale << endl;

  // get the tree
  TTree *tree;
  tree = (TTree *)input->Get(treeName);

  // get the branch
  float var;
  TBranch *bVar;
  tree->SetBranchAddress(brName, &var, &bVar);

  // get auxillary branches
  bool passTrg;
  float puWeight;
  int eCharge;
  int muCharge;
  int evtRegion;
  float cutVar = 0.;
  float trgEff = 1.;
  float trgEffSf = 1.;
  float eleEffSf = 1.;
  float muEffSf = 1.;
  TBranch *bPassTrg;
  TBranch *bPuWeight;
  TBranch *bECharge;
  TBranch *bMuCharge;
  TBranch *bEvtRegion;
  TBranch *bCutVar;
  tree->SetBranchAddress("passTrg", &passTrg, &bPassTrg);
  tree->SetBranchAddress("puWeight", &puWeight, &bPuWeight);
  tree->SetBranchAddress("eCharge", &eCharge, &bECharge);
  tree->SetBranchAddress("muCharge", &muCharge, &bMuCharge);
  tree->SetBranchAddress("evtRegion", &evtRegion, &bEvtRegion);
  if (cutVariable[0] != '\0') tree->SetBranchAddress(cutVariable, &cutVar, &bCutVar);

  tree->SetBranchStatus("*",0); //disable all branches
  tree->SetBranchStatus(brName,1);
  tree->SetBranchStatus("passTrg",1);
  tree->SetBranchStatus("puWeight",1);
  tree->SetBranchStatus("eCharge",1);
  tree->SetBranchStatus("muCharge",1);
  tree->SetBranchStatus("evtRegion",1);
  if (cutVariable[0] != '\0') tree->SetBranchStatus(cutVariable,1);
  if (flags & 1<<5) {
    tree->SetBranchStatus("trgEff",1);
    tree->SetBranchAddress("trgEff", &trgEff);
  }
  if (flags & 1<<4) {
    tree->SetBranchStatus("trgEffSf",1);
    tree->SetBranchAddress("trgEffSf", &trgEffSf);
  }
  if (flags & 1<<2) {
    tree->SetBranchStatus("eleEffSf",1);
    tree->SetBranchAddress("eleEffSf", &eleEffSf);
  }
  if (flags & 1<<1) {
    tree->SetBranchStatus("muEffSf",1);
    tree->SetBranchAddress("muEffSf", &muEffSf);
  }

  Long64_t nEntries = (*tree).GetEntries();
  for (unsigned int i = 0; i < nEntries; ++i) {
    tree->GetEntry(i);

    // trigger fired?
    if ((flags & 1<<8) && passTrg == false) continue;

    float scaleFactor = userScale;
    if (flags & 1<<5) scaleFactor *= trgEff;
    if (flags & 1<<4) scaleFactor *= trgEffSf;
    if (flags & 1<<2) scaleFactor *= eleEffSf;
    if (flags & 1<<1) scaleFactor *= muEffSf;
    // set lumi and electron scalefactor according to detector region
    if (evtRegion == 0 && flags & 1<<3) scaleFactor *= lumiScaleFactorEB;
    if (evtRegion == 1 && flags & 1<<3) scaleFactor *= lumiScaleFactorEE;

    // PU reweight
    if (flags & 1) scaleFactor *= puWeight;

    // get only the desired charge combination. Scheme e-mu -3 to +3: -+, +-, OS, ALL, SS, ++, --
    if (signs < 0 && (eCharge * muCharge) > 0) continue; // OS
    if (signs > 0 && (eCharge * muCharge) < 0) continue; // SS
    if (abs(signs) == 3 && eCharge > 0) continue; // e-mu+ or e-mu-
    if (abs(signs) == 2 && eCharge < 0) continue; // e+mu- or e+mu+

    // get only desired detector region events. Scheme: EB+EE=0, EB=1, EE=2
    if (region == 1 && evtRegion == 1) continue;
    if (region == 2 && evtRegion == 0) continue;

    // user defined cut
    if (cutVariable[0] != '\0')
      if (cutVar < cutLow || cutVar >= cutHigh) continue;

    if (normToBinWidth) scaleFactor /= histo->GetBinWidth(histo->FindBin(var));
    histo->Fill(var, scaleFactor);
  }

  //cout << "integral: " << histo->Integral() << "       overflow: " << histo->GetBinContent(histo->GetNbinsX() + 1) << endl;
  return histo;
}

