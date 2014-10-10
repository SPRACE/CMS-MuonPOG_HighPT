#include <math>
#include <TMath.h>


void createCanvas( TCanvas* &, string );
void saveCanvas( TCanvas *, string );
void changeBin( TH1D *, TH1D *, TH1D *, TH1D *, TH1D *, TH1D *,
		TH1D* &, TH1D* &, TH1D* &,  TH1D* &, TH1D* &, TH1D* &, string );

//==================================================================
void ComputeEfficiencies () {

  string commonDir = "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v9_StacksWithNewTrees_Dec_18_2013/Folders_of_Control_Plots_03Apr2014/";
  //  string dirDen = "Plots_DataMC_Distributions_v22_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_v10_10Jun2014";
  //  string dirNum = "Plots_DataMC_Distributions_v23_Tight2012_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_v10_25Sep2014";
  string dirDen = "Plots_DataMC_Distributions_v24_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_WithBestPair_v10_26Sep2014";
  string dirNum = "Plots_DataMC_Distributions_v25_Tight2012_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_WithBestPair_v10_26Sep2014";
  /*
  string filesDen[] = {
    "Histograms_distribution_TP_ID_v22_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_v10_NewTuneP_eta.root",
    "Histograms_distribution_TP_ID_v22_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_v10_NewTuneP_phi.root",
    "Histograms_distribution_TP_ID_v22_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_v10_NewTuneP_pt.root",
    "Histograms_distribution_TP_ID_v22_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_v10_tag_nVertices.root",
    "Histograms_distribution_TP_ID_v22_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_v10_pair_newTuneP_mass.root"
  };

  string filesNum[] = {
    "Histograms_distribution_TP_ID_v23_Tight2012_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_v10_NewTuneP_eta.root",
    "Histograms_distribution_TP_ID_v23_Tight2012_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_v10_NewTuneP_phi.root",
    "Histograms_distribution_TP_ID_v23_Tight2012_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_v10_NewTuneP_pt.root",
    "Histograms_distribution_TP_ID_v23_Tight2012_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_v10_tag_nVertices.root",
    "Histograms_distribution_TP_ID_v23_Tight2012_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_v10_pair_newTuneP_mass.root"
  };
  */
  string filesDen[] = {
    "Histograms_distribution_TP_ID_v25_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_WithBestPair_v10_NewTuneP_eta.root",
    "Histograms_distribution_TP_ID_v25_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_WithBestPair_v10_NewTuneP_phi.root",
    "Histograms_distribution_TP_ID_v25_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_WithBestPair_v10_NewTuneP_pt.root",
    "Histograms_distribution_TP_ID_v25_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_WithBestPair_v10_tag_nVertices.root",
    "Histograms_distribution_TP_ID_v25_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_WithBestPair_v10_pair_newTuneP_mass.root"
  };

  string filesNum[] = {
    "Histograms_distribution_TP_ID_v25_Tight2012_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_WithBestPair_v10_NewTuneP_eta.root",
    "Histograms_distribution_TP_ID_v25_Tight2012_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_WithBestPair_v10_NewTuneP_phi.root",
    "Histograms_distribution_TP_ID_v25_Tight2012_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_WithBestPair_v10_NewTuneP_pt.root",
    "Histograms_distribution_TP_ID_v25_Tight2012_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_WithBestPair_v10_tag_nVertices.root",
    "Histograms_distribution_TP_ID_v25_Tight2012_Chi2NDF1.0_MET50_pTbalance0.4_dz0.05_BaselineSelections_WithBestPair_v10_pair_newTuneP_mass.root"
  };

  string varsName[] = {
    "#eta",
    "#phi [rad]",
    "p_{T} [GeV/c]",
    "Number of Vertices",
    "m_{#mu#mu} [GeV/c^{2}]"
  };

  string variables[] = {
    "eta",
    "phi",
    "pt",
    "vertex",
    "mass"
  };

  // Check if the "filesDen" contains the same number of files in "filesNum"
  int NFilesDen  = sizeof(filesDen)  / sizeof(string);
  int NFilesNum  = sizeof(filesNum)  / sizeof(string);
  int NVariables = sizeof(varsName)  / sizeof(string);
  int NVars      = sizeof(variables) / sizeof(string);
  if (!((NFilesDen == NFilesNum) && (NFilesDen == NVariables) && (NFilesDen == NVars))) {
    cout << "*" << endl;
    cout << "**************************************************************************" << endl;
    cout << "*" << endl;
    cout << "*\t ERROR: denominator and numerator must have the same number of input files." << endl;
    cout << "*\t Denominator has " << NFilesDen  << " files."     << endl;
    cout << "*\t Numerator   has " << NFilesNum  << " files."     << endl;
    cout << "*\t There are "       << NVariables << " variables." << endl;
    cout << "*" << endl;
    cout << "**************************************************************************" << endl;
    gSystem->Exit(0);
  }

  const int nVars    = NVariables;
  const int nSamples = 18;

  char   name[100];

  TFile *fDen[nVars];
  TFile *fNum[nVars];

  TH1D  *hDen[nVars][nSamples];
  TH1D  *hNum[nVars][nSamples];

  TH1D  *hDenDY[nVars];
  TH1D  *hNumDY[nVars];
  TH1D  *hDenData[nVars];
  TH1D  *hNumData[nVars];
  TH1D  *hDenAllMC[nVars];
  TH1D  *hNumAllMC[nVars];

  TH1D  *hDenDYbinned[nVars];
  TH1D  *hNumDYbinned[nVars];
  TH1D  *hDenDatabinned[nVars];
  TH1D  *hNumDatabinned[nVars];
  TH1D  *hDenAllMCbinned[nVars];
  TH1D  *hNumAllMCbinned[nVars];

  TH1D  *hAuxiliar1[nVars];
  TH1D  *hAuxiliar2[nVars];
  TH1D  *hAuxiliarDY[nVars];
  TH1D  *hAuxiliarData[nVars];
  TH1D  *hAuxiliarAllMC[nVars];
  TH1D  *hRatioDataAllMC[nVars];

  TGraphAsymmErrors *hSF[nVars];
  TGraphAsymmErrors *hEffDY[nVars];
  TGraphAsymmErrors *hEffData[nVars];
  TGraphAsymmErrors *hEffAllMC[nVars];
  TGraphAsymmErrors *hEffDataFinal[nVars];

  for (int i = 0; i < nVars; i++) {
    cout << "**************************************************************************" << endl;
    cout << "*\t Variable: " << variables[i].c_str() <<  endl;

    fDen[i] = TFile::Open( (commonDir + "/" + dirDen + "/" + filesDen[i] ).c_str() );
    fNum[i] = TFile::Open( (commonDir + "/" + dirNum + "/" + filesNum[i] ).c_str() );

    cout << "*\t Denominator: " << fDen[i]->GetName() << endl;
    cout << "*\t Numerator  : " << fNum[i]->GetName() << endl;

    for (int j = 0; j < nSamples; j++) {
      sprintf(name, "%s", fDen[i]->GetListOfKeys()->At(j)->GetName());
      hDen[i][j] = (TH1D*)fDen[i]->Get(name);
      sprintf(name, "%s_clone", fDen[i]->GetListOfKeys()->At(j)->GetName());
      hDen[i][j]->SetName(name);

      sprintf(name, "%s", fNum[i]->GetListOfKeys()->At(j)->GetName());
      hNum[i][j] = (TH1D*)fNum[i]->Get(name);
      sprintf(name, "%s_clone", fNum[i]->GetListOfKeys()->At(j)->GetName());
      hNum[i][j]->SetName(name);

      if (j == 0) {
	hDenDY[i]    = (TH1D*)hDen[i][j]->Clone();
	hNumDY[i]    = (TH1D*)hNum[i][j]->Clone();
	hDenData[i]  = (TH1D*)hDen[i][j]->Clone();
	hNumData[i]  = (TH1D*)hNum[i][j]->Clone();
	hDenAllMC[i] = (TH1D*)hDen[i][j]->Clone();
	hNumAllMC[i] = (TH1D*)hNum[i][j]->Clone();

	hDenDY[i]->Reset();
	hNumDY[i]->Reset();
	hDenData[i]->Reset();
	hNumData[i]->Reset();
	hDenAllMC[i]->Reset();
	hNumAllMC[i]->Reset();

	hDenData[i]->Add(hDen[i][j]);
	hNumData[i]->Add(hNum[i][j]);
      }
      else {
	hDenAllMC[i]->Add(hDen[i][j]);
	hNumAllMC[i]->Add(hNum[i][j]);
      }

      if ((j > 0) && (j < 9)) {
	hDenDY[i]->Add(hDen[i][j]);
	hNumDY[i]->Add(hNum[i][j]);
      }
    } // for (int j = 0; j < nSamples; j++)
  } // End of for (int i = 0; i < nVars; i++)


  TCanvas *canvas[nVars];

  for (int i = 0; i < nVars; i++) {
    cout << "**************************************************************************" << endl;
    cout << "**************************************************************************" << endl;
    cout << "*\t Variable: " << variables[i].c_str() <<  endl;

    sprintf(name, "canvas_%d", i);
    createCanvas(canvas[i], name);

    // Set pads
    const double leftMargin     = 0.15;
    const double rightMargin    = 0.05;
    const double topMargin      = 0.075;
    const double bottomMargin   = 0.125;
    const Float_t bottomMargin2 = 0.3; // In case of a second pad (Data/MC ratio)
    
    // - top pad for efficiencies
    // - bottom pad for scale factors
    const Float_t padXmin  = 0.00;  // minimum x position of the pad1
    const Float_t padXmax  = 1.00;  // maximum x position of the pad1
    const Float_t padYmin1 = 0.40;  // minimum y position of the pad1
    const Float_t padYmax1 = 0.99;  // minimum y position of the pad1
    const Float_t padYmin2 = 0.00;  // minimum y position of the pad2
    const Float_t padYmax2 = 0.40;  // minimum y position of the pad2

    // Efficiencies
    string padName = "pad_eff";
    pad1 = new TPad(padName.c_str(), padName.c_str(), padXmin, padYmin1, padXmax, padYmax1);
    pad1->Draw();
    pad1->SetBottomMargin(0.);  //  pad1->SetBottomMargin(bottomMargin);
    pad1->SetLeftMargin(leftMargin);
    pad1->SetRightMargin(rightMargin);
    pad1->SetTopMargin(topMargin);
    
    // SFs = Data / MC ratio
    padName = "pad_ratio";
    pad2 = new TPad(padName.c_str(), padName.c_str(), padXmin, padYmin2, padXmax, padYmax2);
    pad2->Draw();
    pad2->SetTopMargin(0.);
    pad2->SetLeftMargin(leftMargin);
    pad2->SetRightMargin(rightMargin);
    pad2->SetBottomMargin(bottomMargin2);
    
    // Efficiencies
    pad1->cd();
    pad1->SetGrid();
    pad1->SetTicks();

    if ((variables[i] == "pt") || (variables[i] == "mass")) pad1->SetLogx();

    /*
    hEffDY[i]    = (TH1D*)hNumDY[i]->Clone();
    hEffData[i]  = (TH1D*)hNumData[i]->Clone();
    hEffAllMC[i] = (TH1D*)hNumAllMC[i]->Clone();

    hEffDY[i]->Reset();
    hEffData[i]->Reset();
    hEffAllMC[i]->Reset();

    hEffDY[i]->GetYaxis()->SetTitle("Efficiency");
    hEffDY[i]->GetXaxis()->SetTitle( varsName[i].c_str() );

    hEffDY[i]->Divide(hNumDY[i], hDenDY[i]);
    hEffData[i]->Divide(hNumData[i], hDenData[i]);
    hEffAllMC[i]->Divide(hNumAllMC[i], hDenAllMC[i]);
    */

    cout << "*\t Integrals-Numerator(DY)       = " << hNumDY[i]->Integral()    << endl;
    cout << "*\t Integrals-Numerator(Data)     = " << hNumData[i]->Integral()  << endl;
    cout << "*\t Integrals-Numerator(All MC)   = " << hNumAllMC[i]->Integral() << endl;

    cout << "*\t Integrals-Denominator(DY)     = " << hDenDY[i]->Integral()    << endl;
    cout << "*\t Integrals-Denominator(Data)   = " << hDenData[i]->Integral()  << endl;
    cout << "*\t Integrals-Denominator(All MC) = " << hDenAllMC[i]->Integral() << endl;

    changeBin(hNumDY[i],          hDenDY[i],
	      hNumData[i],        hDenData[i],
	      hNumAllMC[i],       hDenAllMC[i],
	      hNumDYbinned[i],    hDenDYbinned[i],
	      hNumDatabinned[i],  hDenDatabinned[i],
	      hNumAllMCbinned[i], hDenAllMCbinned[i],
	      variables[i]);

    //    cout << "((((( DY )))))" << endl;
    //    hEffDY[i]    = new TGraphAsymmErrors(hNumDYbinned[i],    hDenDYbinned[i],    "n");
    cout << "((((( Data )))))" << endl;
    hEffData[i]  = new TGraphAsymmErrors(hNumDatabinned[i],  hDenDatabinned[i],  "n");
    cout << "((((( All MC )))))" << endl;
    hEffAllMC[i] = new TGraphAsymmErrors(hNumAllMCbinned[i], hDenAllMCbinned[i], "n");
    cout << "((((( DY )))))" << endl;
    if ( variables[i] != "vertex" )
      hEffDY[i]    = new TGraphAsymmErrors(hNumDYbinned[i],    hDenDYbinned[i],    "n");
    else {
      hEffDY[i]    = (TGraphAsymmErrors*)hEffData[i]->Clone();
      hEffDY[i]->Divide(hNumDYbinned[i], hDenDYbinned[i], "n");
    }


    hAuxiliar1[i]      = (TH1D*)hDenDYbinned[i]->Clone();
    hAuxiliar2[i]      = (TH1D*)hDenDYbinned[i]->Clone();
    hAuxiliarDY[i]     = (TH1D*)hDenDYbinned[i]->Clone();
    hAuxiliarData[i]   = (TH1D*)hDenDYbinned[i]->Clone();
    hAuxiliarAllMC[i]  = (TH1D*)hDenDYbinned[i]->Clone();
    hRatioDataAllMC[i] = (TH1D*)hDenDYbinned[i]->Clone();

    hAuxiliar1[i]->Reset();
    hAuxiliar2[i]->Reset();
    hAuxiliarDY[i]->Reset();
    hAuxiliarData[i]->Reset();
    hAuxiliarAllMC[i]->Reset();
    hRatioDataAllMC[i]->Reset();

    hAuxiliarDY[i]->Divide(hNumDYbinned[i],       hDenDYbinned[i]);
    hAuxiliarData[i]->Divide(hNumDatabinned[i],   hDenDatabinned[i]);
    hAuxiliarAllMC[i]->Divide(hNumAllMCbinned[i], hDenAllMCbinned[i]);

    hRatioDataAllMC[i]->Divide(hAuxiliarData[i], hAuxiliarAllMC[i]);

    hEffDataFinal[i] = new TGraphAsymmErrors(hDenDYbinned[i]->GetNbinsX());

    /*
    for (int j = 1; j < hDenDYbinned[i]->GetNbinsX(); j++) {
      hAuxiliar1[i]->SetBinContent(j, 1);
      hAuxiliar1[i]->SetBinError(j, 0);
    }
    hAuxiliar2[i]->Divide(hAuxiliar1[i], hRatioDataAllMC[i]);
    */

    for (int j = 0; j < hDenDYbinned[i]->GetNbinsX(); j++) {

      double effDY        = hEffDY[i]->GetY()[j];
      double effData      = hEffData[i]->GetY()[j];
      double effAllMC     = hEffAllMC[i]->GetY()[j];

      double eff          = effDY * effData / effAllMC;

      double errDYLow     = hEffDY[i]->GetErrorYlow(j);
      double errDataLow   = hEffData[i]->GetErrorYlow(j);
      double errAllMCLow  = hEffAllMC[i]->GetErrorYlow(j);
      double errDYHigh    = hEffDY[i]->GetErrorYhigh(j);
      double errDataHigh  = hEffData[i]->GetErrorYhigh(j);
      double errAllMCHigh = hEffAllMC[i]->GetErrorYhigh(j);

      double errLow       = ((effDY == 0) || (effData == 0)) ? 0 :
	TMath::Hypot(TMath::Hypot((errDYLow/effDY), (errDataLow/effData)), (errAllMCLow/effAllMC));
      double errHigh      = ((effDY == 0) || (effData == 0)) ?
	(TMath::Hypot((errDYHigh/effDY), (errDataHigh/effData)) / effAllMC):
	TMath::Hypot(TMath::Hypot((errDYHigh/effDY), (errDataHigh/effData)), (errAllMCHigh/effAllMC));

      hEffDataFinal[i]->SetPoint( j, hEffDY[i]->GetX()[j], eff);

      hEffDataFinal[i]->SetPointError( j, hEffDY[i]->GetErrorXlow(j), hEffDY[i]->GetErrorXhigh(j), errLow, errHigh );

      cout << "bin = " << j << ", eff = " << eff << " +" << errHigh << " -" << errLow << endl;
    }

    //    hEffDataFinal[i] = new TGraphAsymmErrors(hAuxiliarDY[i], hRatioDataAllMC[i], "n");

    hEffDY[i]->SetLineColor(kBlue);
    hEffDY[i]->SetMarkerColor(kBlue);
    hEffData[i]->SetLineColor(kBlack);
    hEffDataFinal[i]->SetLineColor(kGreen+2);
    hEffData[i]->SetMarkerColor(kBlack);
    hEffAllMC[i]->SetLineColor(kRed);
    hEffAllMC[i]->SetMarkerColor(kRed);
    hEffDataFinal[i]->SetMarkerColor(kGreen+2);

    hEffDY[i]->SetMarkerStyle(8);
    hEffData[i]->SetMarkerStyle(8);
    hEffAllMC[i]->SetMarkerStyle(8);
    hEffDataFinal[i]->SetMarkerStyle(8);

    hEffDY[i]->SetMarkerSize(0.8);
    hEffData[i]->SetMarkerSize(0.8);
    hEffAllMC[i]->SetMarkerSize(0.8);
    hEffDataFinal[i]->SetMarkerSize(0.8);

    TMultiGraph *mg = new TMultiGraph();
    mg->Add(hEffDY[i]);
    mg->Add(hEffData[i]);
    mg->Add(hEffAllMC[i]);
    mg->Add(hEffDataFinal[i]);

    mg->Draw("AP");

    mg->GetYaxis()->SetRangeUser(0.05, 1);
    mg->SetTitle("        CMS Preliminary, L = 7.05 fb^{-1}, #sqrt{s} = 8 TeV");
    mg->GetXaxis()->SetTitle( varsName[i].c_str() );
    mg->GetXaxis()->SetTitleSize(0.05);
    mg->GetYaxis()->SetTitle("Efficiency");
    mg->GetYaxis()->SetTitleOffset(1.0);
    mg->GetYaxis()->SetTitleSize(0.07);
    mg->GetYaxis()->SetLabelSize(0.06);

    int xMin = hDenDYbinned[i]->GetBinLowEdge(1);
    int xMax = hDenDYbinned[i]->GetBinLowEdge(hDenDYbinned[i]->GetNbinsX() + 1);
    cout << "----------->>>> xMin = " << xMin << ", xMax = " << xMax << endl;
    mg->GetXaxis()->SetRangeUser(xMin, xMax);

    TLegend *l = new TLegend(0.18, 0.05, 0.45, 0.5);
    l->SetBorderSize(1);
    l->SetFillColor(0);
    l->SetTextSize(0.04);
    l->SetMargin(0.25);
    l->AddEntry(hEffDY[i],        "Only DY Samples", "pl");
    l->AddEntry(hEffAllMC[i],     "All MC Samples",  "pl");
    l->AddEntry(hEffData[i],      "Data-RunC",       "pl");
    l->AddEntry(hEffDataFinal[i], "Corrected Data",  "pl");
    l->Draw();

    // Scale factors
    pad2->cd();
    pad2->SetGrid();
    pad2->SetTicks();

    if ((variables[i] == "pt") || (variables[i] == "mass")) pad2->SetLogx();

    hSF[i] = (TGraphAsymmErrors*)hEffDY[i]->Clone();

    for (int j = 0; j < hDenDYbinned[i]->GetNbinsX(); j++) {
      double sf     = hEffData[i]->GetY()[j] / hEffAllMC[i]->GetY()[j];
      double sfLow  = TMath::Hypot( (hEffData[i]->GetErrorYlow(j)   / hEffData[i]->GetY()[j]),
				    (hEffAllMC[i]->GetErrorYlow(j)  / hEffAllMC[i]->GetY()[j]) );
      double sfHigh = TMath::Hypot( (hEffData[i]->GetErrorYhigh(j)  / hEffData[i]->GetY()[j]),
				    (hEffAllMC[i]->GetErrorYhigh(j) / hEffAllMC[i]->GetY()[j]) );

      hSF[i]->SetPoint( j, hEffDY[i]->GetX()[j], sf );
      hSF[i]->SetPointError( j, hEffDY[i]->GetErrorXlow(j), hEffDY[i]->GetErrorXhigh(j), sfLow, sfHigh );
    }
    /*
    for (int j = 0; j < hDenDYbinned[i]->GetNbinsX(); j++) {

      double sf     = hEffDataFinal[i]->GetY()[j] / hEffDY[i]->GetY()[j];
      double sfLow  = TMath::Hypot( (hEffDataFinal[i]->GetErrorYlow(j) / hEffDataFinal[i]->GetY()[j]),
				    (hEffDY[i]->GetErrorYlow(j)     / hEffDY[i]->GetY()[j]) );
      double sfHigh = TMath::Hypot( (hEffDataFinal[i]->GetErrorYhigh(j) / hEffDataFinal[i]->GetY()[j]),
				    (hEffDY[i]->GetErrorYhigh(j)     / hEffDY[i]->GetY()[j]) );

      hSF[i]->SetPoint( j, hEffDY[i]->GetX()[j], sf );
      hSF[i]->SetPointError( j, hEffDY[i]->GetErrorXlow(j), hEffDY[i]->GetErrorXhigh(j), sfLow, sfHigh );
    }
    */
    hSF[i]->SetMarkerStyle(8);
    hSF[i]->SetMarkerSize(0.8);

    TMultiGraph *mg2 = new TMultiGraph();
    mg2->Add(hSF[i]);

    mg2->Draw("AP");

    mg2->SetTitle("");
    mg2->GetYaxis()->SetRangeUser(0.75, 1.25);
    mg2->GetYaxis()->SetNdivisions(509);
    mg2->GetXaxis()->SetTitle( varsName[i].c_str() );
    mg2->GetXaxis()->SetTitleSize(0.12);
    mg2->GetXaxis()->SetLabelSize(0.08);
    mg2->GetYaxis()->CenterTitle();
    mg2->GetYaxis()->SetTitleOffset(0.75);
    mg2->GetYaxis()->SetTitleSize(0.09);
    mg2->GetYaxis()->SetLabelSize(0.08);
    mg2->GetYaxis()->SetDecimals();
    mg2->GetYaxis()->SetTitle("Scale Factor");
    //    mg2->GetYaxis()->SetTitle("#frac{Correct Data}{All MC Samples}");

    xMin = hDenDYbinned[i]->GetBinLowEdge(1);
    xMax = hDenDYbinned[i]->GetBinLowEdge(hDenDYbinned[i]->GetNbinsX() + 1);
    cout << "----------->>>> xMin = " << xMin << ", xMax = " << xMax << endl;
    mg2->GetXaxis()->SetRangeUser(xMin, xMax);

    TLegend *l2 = new TLegend(0.2, 0.75, 0.55, 0.95);
    l2->SetBorderSize(1);
    l2->SetFillColor(0);
    l2->SetTextSize(0.07);
    l2->SetMargin(0.25);
    l2->AddEntry(hSF[i], "SF = #frac{Data-RunC}{All MC Samples}", "pl");
    l2->Draw();

    // Save canvas
    saveCanvas(canvas[i], "Efficiency_" + variables[i] );  
  }

}


//==================================================================
void createCanvas( TCanvas* &c, string name ) {

  c = new TCanvas(name.c_str(), name.c_str(), 600, 600);
  c->cd();
  gStyle->SetOptStat(kFALSE);

  /*
  // Set pads
  double leftMargin   = 0.15;
  double rightMargin  = 0.05;
  double topMargin    = 0.075;
  double bottomMargin = 0.125;
  gPad->SetLeftMargin(leftMargin);
  gPad->SetRightMargin(rightMargin);
  gPad->SetTopMargin(topMargin);
  gPad->SetBottomMargin(bottomMargin);
  */
} // End of void createCanvas( TCanvas* &c, char name )


//==================================================================
void saveCanvas( TCanvas *can, string name ) {

  char canvasName[100];

  sprintf(canvasName, "%s.root", name.c_str());
  can->SaveAs(canvasName);

  sprintf(canvasName, "%s.pdf", name.c_str());
  can->SaveAs(canvasName);

  sprintf(canvasName, "%s.png", name.c_str());
  can->SaveAs(canvasName);

  sprintf(canvasName, "%s.C", name.c_str());
  can->SaveAs(canvasName);

} // End of void saveCanvas( TCanvas *c, string name )


//==================================================================
void changeBin(TH1D *hNumDY,           TH1D *hDenDY,
	       TH1D *hNumData,         TH1D *hDenData,
	       TH1D *hNumAllMC,        TH1D *hDenAllMC,
	       TH1D* &hNumDYbinned,    TH1D* &hDenDYbinned,
	       TH1D* &hNumDatabinned,  TH1D* &hDenDatabinned,
	       TH1D* &hNumAllMCbinned, TH1D* &hDenAllMCbinned,
	       string variable) {

    if (variable == "eta") {
      double edges[] = {-2.4, -2.1, -1.6, -1.2, -0.9, -0.6, -0.3, -0.2, 0.2, 0.3, 0.6, 0.9, 1.2, 1.6, 2.1, 2.4};
    }
    else if (variable == "phi") {
      double edges[] = {-3.0, -2.4, -2.0, -1.4, -1.0, -0.4, 0.0, 0.4, 1.0, 1.4, 2.0, 2.4, 3.0};
    }
    //      {double edges[] = {-3, -2.5, -2, -1.5, -1, -0.5, 0, 0.5, 1, 1.5, 2, 2.5, 3};};
    else if (variable == "pt") {
      double edges[] = {40, 50, 60, 70, 80, 90, 140, 300, 1000};
    }
    //      {double edges[] = {45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 140, 300, 1000};};
    else if (variable == "vertex") {
      double edges[] = {0.0, 2.0, 4.0, 6.0, 8.0, 10.0, 12.0, 14.0, 16.0, 18.0, 20.0, 22.0, 24.0, 26.0, 28.0, 30.0, 32.0, 34.0, 36.0, 38.0, 40.0};
    }
    //      {double edges[] = {0.5, 2.5, 4.5, 6.5, 8.5, 10.5, 12.5, 14.5, 16.5, 18.5, 20.5, 22.5, 24.5, 26.5, 28.5, 30.5, 32.5, 34.5, 36.5, 38.5, 40.5, 42.5, 44.5, 46.5, 48.5, 49.5};};
    else if (variable == "mass") {
      double edges[] = {60, 120, 210, 510, 3000};
    }
    //      {double edges[] = {60, 120, 200, 500, 3000};};
    else {
      cout << "*\t WARNING: This macro recognizes only some variable names:" << endl;
      cout << "*\t \"pt\", \"eta\", \"phi\", \"mass\" and \"vertex\"" << endl;
      cout << "*\t Verify if their names are right." << endl;
      gSystem->Exit(0);
    }

    int nBins = (sizeof(edges)/sizeof(double)) - 1;

    char name[100];
    sprintf(name, "%s_numDY", variable);
    hNumDYbinned = new TH1D(name, name, nBins, edges);
    sprintf(name, "%s_demDY", variable);
    hDenDYbinned = new TH1D(name, name, nBins, edges);

    sprintf(name, "%s_numData", variable);
    hNumDatabinned = new TH1D(name, name, nBins, edges);
    sprintf(name, "%s_demData", variable);
    hDenDatabinned = new TH1D(name, name, nBins, edges);

    sprintf(name, "%s_numAllMC", variable);
    hNumAllMCbinned = new TH1D(name, name, nBins, edges);
    sprintf(name, "%s_demAllMC", variable);
    hDenAllMCbinned = new TH1D(name, name, nBins, edges);

    double contentNumDY;
    double contentNumData;
    double contentNumAllMC;
    double contentDenDY;
    double contentDenData;
    double contentDenAllMC;
    double errorNumDY;
    double errorNumData;
    double errorNumAllMC;
    double errorDenDY;
    double errorDenData;
    double errorDenAllMC;

    for (int i = 1; i < nBins + 1; i++) {
      contentNumDY    = 0;
      contentNumData  = 0;
      contentNumAllMC = 0;
      contentDenDY    = 0;
      contentDenData  = 0;
      contentDenAllMC = 0;
      errorNumDY      = 0;
      errorNumData    = 0;
      errorNumAllMC   = 0;
      errorDenDY      = 0;
      errorDenData    = 0;
      errorDenAllMC   = 0;

      for (int j = 1; j < hNumDY->GetNbinsX(); j++) {

	double xMin = hNumDY->GetBinLowEdge(j);
	double xMax = hNumDY->GetBinLowEdge(j+1);
	if ((xMin < edges[i-1]) || (xMax > edges[i])) continue;

	contentNumDY    = contentNumDY    + hNumDY->GetBinContent(j);
	contentDenDY    = contentDenDY    + hDenDY->GetBinContent(j);
	contentNumData  = contentNumData  + hNumData->GetBinContent(j);
	contentDenData  = contentDenData  + hDenData->GetBinContent(j);
	contentNumAllMC = contentNumAllMC + hNumAllMC->GetBinContent(j);
	contentDenAllMC = contentDenAllMC + hDenAllMC->GetBinContent(j);

	errorNumDY      = sqrt((errorNumDY    * errorNumDY)    + pow(hNumDY->GetBinError(j),    2));
	errorDenDY      = sqrt((errorDenDY    * errorDenDY)    + pow(hDenDY->GetBinError(j),    2));
	errorNumData    = sqrt((errorNumData  * errorNumData)  + pow(hNumData->GetBinError(j),  2));
	errorDenData    = sqrt((errorDenData  * errorDenData)  + pow(hDenData->GetBinError(j),  2));
	errorNumAllMC   = sqrt((errorNumAllMC * errorNumAllMC) + pow(hNumAllMC->GetBinError(j), 2));
	errorDenAllMC   = sqrt((errorDenAllMC * errorDenAllMC) + pow(hDenAllMC->GetBinError(j), 2));

      } // End of for (int j = 1; j < hNumDY->GetNbinsX(); j++)

      cout << "..........." << edges[i-1] << "   -->   " << edges[i] << ", effData = " << contentNumData/contentDenData << endl;

      hNumDYbinned->SetBinContent(i,    contentNumDY);
      hDenDYbinned->SetBinContent(i,    contentDenDY);
      hNumDatabinned->SetBinContent(i,  contentNumData);
      hDenDatabinned->SetBinContent(i,  contentDenData);
      hNumAllMCbinned->SetBinContent(i, contentNumAllMC);
      hDenAllMCbinned->SetBinContent(i, contentDenAllMC);

      hNumDYbinned->SetBinError(i,    errorNumDY);
      hDenDYbinned->SetBinError(i,    errorDenDY);
      hNumDatabinned->SetBinError(i,  errorNumData);
      hDenDatabinned->SetBinError(i,  errorDenData);
      hNumAllMCbinned->SetBinError(i, errorNumAllMC);
      hDenAllMCbinned->SetBinError(i, errorDenAllMC);

    } // End of for (int i = 1; i < edges.size() - 1; i++)

}

