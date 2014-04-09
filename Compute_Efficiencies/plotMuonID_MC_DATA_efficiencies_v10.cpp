/******************************************************************
  MACRO FILE: plotMuonID_MC_DATA_efficiencies_v10.cpp

  This is ROOT macro file used to compute plots of efficiency
  for different Data and MC samples. As soon as separated ROOT
  files with plots of efficiencies are created for each sample
  (based on counting method from Tag & Probe muons), those files
  can be added here as input files.

  This macro will compute efficiencies for the following cases:
    * Data    (apparent efficiency)
    * Data    (final efficiency)
    * All MC  (apparent efficiency)
    * DY MC   (efficiency from signal)
    * DY+Jets (control plot of efficiency)

  In case of MC (DY MC and All MC), for each bin, it is computed
  the final efficiency from the sum of all MC efficiency weight
  by its own weight:
    Eff = (W1*effMC1 + W2*effMC2 + ... + Wn*effMCn) / (W1 + W2 + ... + Wn)
  Such weights are give by:
    Wn = (number of entries in that bin of sample n) x (MC weight from cross sections and luminosity)

  Versions:
  ________________________________________________________
    * v10 (08, April, 2014):
      - comes from version v9;
      - gets a final Data efficiency taken into account the
        efficiency from DY sample and the apparent efficiencies
	from Data and All MC samples. This final efficiency is
	given by:
           Eff_Data_Final = Eff_DY * (Eff_Data / Eff_All_MC)
      - has a bug coming from the fact that, if "Data efficiencies"
        are higher than "All MC efficiencies", it can lead to a
        final Data efficiency higher than "1".

    * v9  (07, April, 2014):
      - comes from version v8;
      - takes care of properly computing error bars, so now errors
        should be inside the range [0,1] along the Y axis.

******************************************************************/

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <math>

#include <TCanvas.h>
#include <TFile.h>
#include <TStyle.h>
#include <TMath.h>
#include <TLegend.h>
#include <TGraphAsymmErrors.h>

#include "SettingUP_EfficiencyPlots.h"

// This function is used to tell the path of the separated efficiency plots inside the input ROOT files
void set_auxiliar_string ( vector <string>, stringstream &, Int_t );

// This function sets the title in the X axis
void set_Xaxis_label ( vector <string>, stringstream &, Int_t );

// This function is used to get the number of entries per variable per sample per bin.
void get_entries_per_bin ( vector <string>, Int_t, Int_t, Int_t, long & );


// This is the main function
void plotMuonID_MC_DATA_efficiencies_v10 () {

  vector <TFile*> files;
  vector <TCanvas*> canvas;
  vector <Double_t> global_weight;
  vector <string> variable;
  vector <string> output_format;
  vector <Int_t> nBins;
  vector <TLegend*> legend;

  vector <TGraphAsymmErrors*> eff_ID_DATA;
  vector <TGraphAsymmErrors*> eff_ID_MC;

  TGraphAsymmErrors* eff_ID_DATA_Final;
  TGraphAsymmErrors* eff_ID_All_MC;
  TGraphAsymmErrors* eff_ID_DY_MC;

  long entries_per_bin;
  Double_t denominator;
  Double_t eff;
  Double_t effYlow;
  Double_t effYhigh;

  Double_t Data_effs;
  Double_t Data_effs_Low_Uncertainty;
  Double_t Data_effs_High_Uncertainty;

  Double_t weighted_MC_effs;
  Double_t weighted_MC_effs_Low_Uncertainty;
  Double_t weighted_MC_effs_High_Uncertainty;

  Double_t Sum_Weighted_MC_Eff;
  Double_t Sum_Weighted_MC_Eff_Low_Uncertainty;
  Double_t Sum_Weighted_MC_Eff_High_Uncertainty;

  stringstream auxiliar_string;
  stringstream Xaxis_label;


  cout << "number_of_MC_samples: " << number_of_MC_samples
       << ", number_of_DY_samples: " << number_of_DY_samples << endl;

  // Open files
  for ( Int_t i = 0; i < (sizeof (files_path) / sizeof (string)); i++ ) {
    files.push_back( TFile::Open( files_path[i].c_str() ) );
    cout << "Loading file: " << files[i]->GetName() << endl;
  }
  cout << endl;

  // Passing variable names into a vector of strings.
  cout << "\t* Plots of efficiencies will be created for these variables:" << endl;
  for ( Int_t i = 0; i < (sizeof (variable_name) / sizeof (string)); i++ ) {
    variable.push_back( variable_name[i] );
    cout << "\t\t(" << (i+1) << "): " << variable[i].c_str() << endl;
  }
  cout << endl;

  // Passing number of bins into vector of intergers.
  cout << "\t* Number of bins for each variable:" << endl;
  for ( Int_t i = 0; i < (sizeof (number_of_bins) / sizeof (Int_t)); i++ ) {
    nBins.push_back( number_of_bins[i] );
    cout << "\t\t* " << variable_name[i] << ": " << nBins[i] << endl;
  }
  cout << endl;

  // Write scale factor into vector of doubles.
  for ( Int_t i = 0; i < (sizeof (scale_factors) / sizeof (Double_t)); i++ ) {
    global_weight.push_back( scale_factors[i] ); // no scaling
    cout << "Scale factor for file " << files[i+1]->GetName() << " -> " << global_weight[i] << endl;
  }

  // Reading types of output format files will be saved in.
  Int_t nOutputFormats = ( sizeof(format) / sizeof(string) );
  for (Int_t i = 0; i < nOutputFormats; i++) {
    output_format.push_back( format[i] );
  }
  cout << "..................................................................." <<endl;

  //////////////////////////////////////////////////////////////
  // Start looping over variables
  //////////////////////////////////////////////////////////////
  Int_t counter = 0;
  for ( Int_t v = 0; v < variable.size(); v++ ) {
    cout << "*\tVariable " << (v+1) << "\t:\t" << variable[v] << endl;

    // For data we do not need to sum many components.
    set_auxiliar_string ( variable, auxiliar_string, v );
    eff_ID_DATA.push_back( (TGraphAsymmErrors*)files[0]->Get( auxiliar_string.str().c_str() )->FindObject("hxy_cnt_eff") );
    eff_ID_DATA[v]->SetMinimum(0);
    eff_ID_DATA[v]->SetMaximum(1);

    // For final data efficiencies
    eff_ID_DATA_Final = new TGraphAsymmErrors( nBins[v] );
    
    // For MC we need to sum all the contributions with the corresponding weight.
    // The weights are considered with respect to the luminosity of RunC.
    set_auxiliar_string ( variable, auxiliar_string, v );
    for ( Int_t i = 1; i < (number_of_MC_samples + 1); i++) {
      eff_ID_MC.push_back( (TGraphAsymmErrors*)files[i]->Get( auxiliar_string.str().c_str() )->FindObject("hxy_cnt_eff") );
      //      cout << "File " << files[i]->GetName() << endl;

      counter++;
      //      cout << "counter: " << counter << endl;
    }

    // This is the final MC "TGraph" with the combination of all components.
    eff_ID_All_MC = new TGraphAsymmErrors( nBins[v] );
    eff_ID_DY_MC = new TGraphAsymmErrors( nBins[v] );

    // Loop over the all bins of the chosen variable.
    for ( Int_t bin = 0; bin < nBins[v]; bin++ ) {
      // For each bin, it is computed the efficiency from the sum of all MC
      // efficiency weight by its own weight:
      //    (W1*effMC1 + W2*effMC2 + ... + Wn*effMCn) / (W1 + W2 + ... + Wn)
      // "DY_Jets-LL_M_50" sample is not included here.
      Sum_Weighted_MC_Eff = 0.0;
      Sum_Weighted_MC_Eff_Low_Uncertainty = 0.0;
      Sum_Weighted_MC_Eff_High_Uncertainty = 0.0;

      // For final data efficiencies
      Data_effs = 0.0;
      Data_effs_Low_Uncertainty = 0.0;
      Data_effs_High_Uncertainty = 0.0;

      //////////////////////////////////////////////////////////////////////////////////////////////////////
      // From here, efficiencies will be computed taking all MC samples (starting by DY ones).
      //////////////////////////////////////////////////////////////////////////////////////////////////////

      // Sum over all scale factors, with exception of "DY_Jets-LL_M_50" sample,
      // in order to compute the denominator.
      denominator = 0.0;
      for ( Int_t i = 0; i < (number_of_MC_samples - 1); i++ ) {
	get_entries_per_bin ( variable, v, i, bin, entries_per_bin );
	denominator = denominator + ( entries_per_bin * global_weight[i] );
      }

      // If denominator ~ 0, make efficiency ~ 0.
      if ( denominator < 0.0000000001 ) {
	//	cout << "___________denominator: " << denominator << ", ___________bin: " << bin << endl;
	eff_ID_All_MC->SetPoint( bin, eff_ID_MC[counter-1]->GetX()[bin], 0. );
	eff_ID_All_MC->SetPointError( bin, eff_ID_MC[counter-1]->GetErrorXlow( bin ),
				      eff_ID_MC[counter-1]->GetErrorXhigh( bin ), 0., 0. );
      }
      else {
	// Loop over number of All MC samples, with exception of "DY_Jets-LL_M_50" sample
	for ( Int_t i = 0; i < (number_of_MC_samples - 1); i++) {
	  get_entries_per_bin ( variable, v, i, bin, entries_per_bin );
	  
	  // Efficiencies higher than "1", or approach "zero", are taken as "zero".
	  if ( ((Double_t)eff_ID_MC[(v*counter/(v+1)) + i]->GetY()[bin] > 1)  ||
	       ((Double_t)eff_ID_MC[(v*counter/(v+1)) + i]->GetY()[bin] < 0.000001) ) {
	    //	    cout << "xxxxxxxxxxxxxxxxxxxiiiiiiiiii: " << i << endl;
	    //	    cout << "bin: " << bin << ",\t All MC: " << i << ",\t den: " << denominator << ",\t w: "
	    //		 << global_weight[i] << ",\t Ni: " << entries_per_bin << ",\t Eff: "
	    //		 << eff_ID_MC[(v*counter/(v+1)) + i]->GetY()[bin];
	    eff      = 0.0;
	    effYlow  = 0.0;
	    effYhigh = 0.0;
	  }
	  else {
	    // This sentence "[(v*counter/(v+1)) + i]" makes sure to get the proper element
	    // of the TGraphAsymmErrors vector.
	    eff      = (Double_t)eff_ID_MC[(v*counter/(v+1)) + i]->GetY()[bin];
	    effYlow  = (Double_t)eff_ID_MC[(v*counter/(v+1)) + i]->GetErrorYlow(bin);
	    effYhigh = (Double_t)eff_ID_MC[(v*counter/(v+1)) + i]->GetErrorYhigh(bin);
	    
	    //	    cout << "bin: " << bin << ",\t All MC: " << i << ",\t den: " << denominator << ",\t w: "
	    //		 << global_weight[i] << ",\t Ni: " << entries_per_bin << ",\t Eff: " << eff;
	  }
	  
	  // Get the weighted efficiency for each MC sample in this bin
	  weighted_MC_effs                  = (Double_t)(global_weight[i] * entries_per_bin / denominator) * eff;
	  weighted_MC_effs_Low_Uncertainty  = (Double_t)(global_weight[i] * entries_per_bin / denominator) * effYlow;
	  weighted_MC_effs_High_Uncertainty = (Double_t)(global_weight[i] * entries_per_bin / denominator) * effYhigh;
	  
	  // Sum the weighted efficiency with previous one (in the loop of MC samples) for the same bin
	  Sum_Weighted_MC_Eff                  = Sum_Weighted_MC_Eff + weighted_MC_effs;
	  Sum_Weighted_MC_Eff_Low_Uncertainty  = Sum_Weighted_MC_Eff_Low_Uncertainty +
	                                         TMath::Power( weighted_MC_effs_Low_Uncertainty, 2 );
	  Sum_Weighted_MC_Eff_High_Uncertainty = Sum_Weighted_MC_Eff_High_Uncertainty +
                                                 TMath::Power( weighted_MC_effs_High_Uncertainty, 2 );
	  
	  //	  cout << ",\t Bin Eff: " << weighted_MC_effs << ",\t Sum_Weighted_MC_Eff: " << Sum_Weighted_MC_Eff << endl;
	}
	//	cout << "counter: " << counter << endl;
	
	// Uncertainties are summed in quadrature
	Sum_Weighted_MC_Eff_Low_Uncertainty  = TMath::Sqrt( Sum_Weighted_MC_Eff_Low_Uncertainty );
	Sum_Weighted_MC_Eff_High_Uncertainty = TMath::Sqrt( Sum_Weighted_MC_Eff_High_Uncertainty );
	
	// Get final efficiency for All MC samples together
	eff_ID_All_MC->SetPoint(      bin, eff_ID_MC[counter-1]->GetX()[bin], Sum_Weighted_MC_Eff );
	eff_ID_All_MC->SetPointError( bin, eff_ID_MC[counter-1]->GetErrorXlow( bin ),
				      eff_ID_MC[counter-1]->GetErrorXhigh( bin ),
				      Sum_Weighted_MC_Eff_Low_Uncertainty,
				      Sum_Weighted_MC_Eff_High_Uncertainty );
	
	//	cout << "All MC -> bin: " << bin << ",\t den: " << denominator << ",\t Final Bin Eff: "
	//	     << Sum_Weighted_MC_Eff << ",\t Xbin: " << eff_ID_MC[counter-1]->GetX()[bin] << endl;
      }

      //////////////////////////////////////////////////////////////////////////////////////////////////////
      // Now efficiencies for only DY samples will be computed.
      //////////////////////////////////////////////////////////////////////////////////////////////////////
      Sum_Weighted_MC_Eff                  = 0.0;
      Sum_Weighted_MC_Eff_Low_Uncertainty  = 0.0;
      Sum_Weighted_MC_Eff_High_Uncertainty = 0.0;
      denominator                          = 0.0;
      // Sum over all scale factors, for DY samples only, in order to compute the denominator.
      for ( Int_t i = 0; i < number_of_DY_samples; i++ ) {
	get_entries_per_bin ( variable, v, i, bin, entries_per_bin );
        denominator = denominator + ( entries_per_bin * global_weight[i] );
      }

      // If denominator ~ 0, make efficiency ~ 0.
      if ( denominator < 0.0000000001 ) {
	//	cout << "___________denominator DY: " << denominator << ", ___________bin: " << bin << endl;
	eff_ID_DY_MC->SetPoint( bin, eff_ID_MC[counter-1]->GetX()[bin], 0. );
	eff_ID_DY_MC->SetPointError( bin, eff_ID_MC[counter-1]->GetErrorXlow( bin ),
				     eff_ID_MC[counter-1]->GetErrorXhigh( bin ), 0., 0. );
      }
      else {
	// Loop over number of DY samples
	for ( Int_t i = 0; i < number_of_DY_samples; i++) {
	  get_entries_per_bin ( variable, v, i, bin, entries_per_bin );

	  // Efficiencies higher than "1", or approach "zero", are taken as "zero".
	  if ( ((Double_t)eff_ID_MC[(v*counter/(v+1)) + i]->GetY()[bin] > 1) ||
	       ((Double_t)eff_ID_MC[(v*counter/(v+1)) + i]->GetY()[bin] < 0.000001)) {
	    //	    cout << "xxxxxxxxxxxxxxxxxxxaaaaaaaaaaa: " << i << endl;
	    //	    cout << "bin: " << bin << ",\t DY MC: " << i << ",\t den: " << denominator << ",\t w: "
	    //		 << global_weight[i] << ",\t Ni: " << entries_per_bin << ",\t Eff: "
	    //		 << eff_ID_MC[(v*counter/(v+1)) + i]->GetY()[bin];
	    eff      = 0.0;
	    effYlow  = 0.0;
	    effYhigh = 0.0;
	  }
	  else {
	    // This sentence "[(v*counter/(v+1)) + i]" makes sure to get the proper element
	    // of the TGraphAsymmErrors vector.
	    eff      = (Double_t)eff_ID_MC[(v*counter/(v+1)) + i]->GetY()[bin];
	    effYlow  = (Double_t)eff_ID_MC[(v*counter/(v+1)) + i]->GetErrorYlow(bin);
	    effYhigh = (Double_t)eff_ID_MC[(v*counter/(v+1)) + i]->GetErrorYhigh(bin);

	    //	    cout << "bin: " << bin << ",\t DY MC: " << i << ",\t den: " << denominator << ",\t w: "
	    //		 << global_weight[i] << ",\t Ni: " << entries_per_bin << ",\t Eff: " << eff;
	  }

	  // Get the weighted efficiency for each MC sample in this bin
	  weighted_MC_effs                     = (Double_t)(global_weight[i] * entries_per_bin / denominator) * eff;
	  weighted_MC_effs_Low_Uncertainty     = (Double_t)(global_weight[i] * entries_per_bin / denominator) * effYlow;
	  weighted_MC_effs_High_Uncertainty    = (Double_t)(global_weight[i] * entries_per_bin / denominator) * effYhigh;

	  // Sum the weighted efficiency with previous one (in the loop of MC samples) for the same bin
	  Sum_Weighted_MC_Eff                  = Sum_Weighted_MC_Eff + weighted_MC_effs;
	  Sum_Weighted_MC_Eff_Low_Uncertainty  = Sum_Weighted_MC_Eff_Low_Uncertainty + TMath::Power( weighted_MC_effs_Low_Uncertainty, 2 );
	  Sum_Weighted_MC_Eff_High_Uncertainty = Sum_Weighted_MC_Eff_High_Uncertainty + TMath::Power( weighted_MC_effs_High_Uncertainty, 2 );

	  //	  cout << ",\t Bin Eff: " << weighted_MC_effs << ",\t Sum_Weighted_MC_Eff: " << Sum_Weighted_MC_Eff << endl;
	}

	// Error bars are summed in quadrature
	Sum_Weighted_MC_Eff_Low_Uncertainty = TMath::Sqrt( Sum_Weighted_MC_Eff_Low_Uncertainty );
	Sum_Weighted_MC_Eff_High_Uncertainty = TMath::Sqrt( Sum_Weighted_MC_Eff_High_Uncertainty );
	
	// Get final efficiency for All MC samples together
	eff_ID_DY_MC->SetPoint(      bin, eff_ID_MC[counter-1]->GetX()[bin], Sum_Weighted_MC_Eff );
	eff_ID_DY_MC->SetPointError( bin, eff_ID_MC[counter-1]->GetErrorXlow( bin ),
				     eff_ID_MC[counter-1]->GetErrorXhigh( bin ),
				     Sum_Weighted_MC_Eff_Low_Uncertainty,
				     Sum_Weighted_MC_Eff_High_Uncertainty );

	//	cout << "All MC -> bin: " << bin << ",\t den: " << denominator << ",\t Final Bin Eff: "
	//	     << Sum_Weighted_MC_Eff << ",\t Xbin: " << eff_ID_MC[counter-1]->GetX()[bin] << endl;
      }

      //////////////////////////////////////////////////////////////////////////////////////////////////////
      // For final data efficiencies
      //////////////////////////////////////////////////////////////////////////////////////////////////////
      Data_effs                  = (Double_t)eff_ID_DY_MC->GetY()[bin] * (Double_t)eff_ID_DATA[v]->GetY()[bin];

      Data_effs_Low_Uncertainty  = Data_effs * TMath::Sqrt( ((Double_t)eff_ID_DY_MC->GetErrorYlow(bin)*eff_ID_DY_MC->GetErrorYlow(bin) /
							     (eff_ID_DY_MC->GetY()[bin]*eff_ID_DY_MC->GetY()[bin]))
							    +
							    ((Double_t)eff_ID_DATA[v]->GetErrorYlow(bin)*eff_ID_DATA[v]->GetErrorYlow(bin) /
							     (eff_ID_DATA[v]->GetY()[bin]*eff_ID_DATA[v]->GetY()[bin])) );

      Data_effs_High_Uncertainty = Data_effs * TMath::Sqrt( ((Double_t)eff_ID_DY_MC->GetErrorYhigh(bin)*eff_ID_DY_MC->GetErrorYhigh(bin) /
							     (eff_ID_DY_MC->GetY()[bin]*eff_ID_DY_MC->GetY()[bin]))
							    +
							    ((Double_t)eff_ID_DATA[v]->GetErrorYhigh(bin)*eff_ID_DATA[v]->GetErrorYhigh(bin) /
							     (eff_ID_DATA[v]->GetY()[bin]*eff_ID_DATA[v]->GetY()[bin])) );

      // If efficiency from all MC is ~ 0, make final Data efficiency ~ 0.
      if ((Double_t)eff_ID_All_MC->GetY()[bin] < 0.0000000001 ) {
	//	cout << "___________Efficiency from all MC: " << (Double_t)eff_ID_All_MC->GetY()[bin]
	//	     << ", ___________bin: " << bin << endl;

	eff_ID_DATA_Final->SetPoint( bin, eff_ID_DATA[v]->GetX()[bin], 0. );
	eff_ID_DATA_Final->SetPointError( bin, eff_ID_DATA[v]->GetErrorXlow( bin ),
					  eff_ID_DATA[v]->GetErrorXhigh( bin ), 0., 0. );
      }
      else {
	Double_t r                 = (Double_t)Data_effs / (Double_t)eff_ID_All_MC->GetY()[bin];

	Data_effs_Low_Uncertainty  = ( (Double_t)Data_effs == 0 ? 0 :
				       r * TMath::Hypot((Double_t)Data_effs_Low_Uncertainty / Data_effs,
							(Double_t)eff_ID_All_MC->GetErrorYhigh(bin) / eff_ID_All_MC->GetY()[bin]) );
	
	Data_effs_High_Uncertainty = ( (Double_t)Data_effs == 0 ? (Double_t)Data_effs_High_Uncertainty / (eff_ID_All_MC->GetY()[bin]) :
				       r * TMath::Hypot((Double_t)Data_effs_High_Uncertainty / Data_effs,
							(Double_t)eff_ID_All_MC->GetErrorYlow(bin) / eff_ID_All_MC->GetY()[bin]) );
	
	eff_ID_DATA_Final->SetPoint(      bin, eff_ID_DATA[v]->GetX()[bin], r );
	eff_ID_DATA_Final->SetPointError( bin, eff_ID_DATA[v]->GetErrorXlow( bin ),
					  eff_ID_DATA[v]->GetErrorXhigh( bin ),
					  Data_effs_Low_Uncertainty,
					  Data_effs_High_Uncertainty );
      }
    }

    // Now we will plot the efficiencies. I think it is a good idea we compare Data, all MC and DY only.
    // We expect that, if we are considering correctly all the necessary MCs to describe the data,
    // the curves "DATA - RunC" and "All MC" would be in agreement.
    //
    // We intend to reach the DY+Jets curve always high because this sample has a good number of "true"
    // opposite sign muon pairs.
    auxiliar_string.str("");
    auxiliar_string << variable[v] << "_" << v;

    TCanvas *c1 = new TCanvas();
    c1->cd();
    // Use Log in X axis if pt or mass to have a better visualization
    if ((variable[v] == "pt") || (variable[v] == "mass"))
      gPad->SetLogx();

    // Do not display any of the standard histogram decorations.
    gStyle->SetOptTitle(0);

    set_Xaxis_label( variable, Xaxis_label, v );
    
    // Draw efficiency of DY+Jets
    Int_t last_file = counter - 1;
    eff_ID_MC[ last_file ]->Draw("ep AP");
    eff_ID_MC[ last_file ]->GetXaxis()->SetTitle( Xaxis_label.str().c_str() );
    eff_ID_MC[ last_file ]->GetYaxis()->SetRangeUser(0.0,1.0);
    eff_ID_MC[ last_file ]->GetYaxis()->SetTitle("Efficiency");
    eff_ID_MC[ last_file ]->SetMarkerStyle(23);
    eff_ID_MC[ last_file ]->SetMarkerColor(6);
    eff_ID_MC[ last_file ]->SetLineColor(6);

    // Draw apparent efficiency of Data
    eff_ID_DATA[v]->Draw("ep same");
    eff_ID_DATA[v]->SetMarkerStyle(20);
    
    gPad->SetTicks(1,1);
    gPad->SetGrid();

    // Draw efficiency of DY samples
    eff_ID_DY_MC->Draw("ep same");
    eff_ID_DY_MC->SetMarkerStyle(33);
    eff_ID_DY_MC->SetMarkerColor(4);
    eff_ID_DY_MC->SetLineColor(4);
    
    // Draw apparent efficiency of All MC
    eff_ID_All_MC->Draw("ep same");
    eff_ID_All_MC->SetMarkerStyle(33);
    eff_ID_All_MC->SetMarkerColor(2);
    eff_ID_All_MC->SetLineColor(2);

    // Draw final efficiency of Data
    eff_ID_DATA_Final->Draw("ep same");
    eff_ID_DATA_Final->SetMarkerStyle(20);
    eff_ID_DATA_Final->SetMarkerColor(3);
    eff_ID_DATA_Final->SetLineColor(3);

    // Setup legend
    if (variable[v] == "pt")     TLegend *le = new TLegend(0.75,0.8,0.99,0.99);
    if (variable[v] == "eta")    TLegend *le = new TLegend(0.75,0.2,0.99,0.39);
    if (variable[v] == "phi")    TLegend *le = new TLegend(0.75,0.2,0.99,0.39);
    if (variable[v] == "mass")   TLegend *le = new TLegend(0.75,0.5,0.99,0.69);
    if (variable[v] == "vertex") TLegend *le = new TLegend(0.75,0.2,0.99,0.39);
    if (variable[v] == "deltaR") TLegend *le = new TLegend(0.75,0.8,0.99,0.99);

    le->SetBorderSize(1);
    le->SetFillColor(0);
    le->AddEntry( eff_ID_DATA[v],         Data_title.c_str(),       "lp" );
    le->AddEntry( eff_ID_DATA_Final,      Final_Data_title.c_str(), "lp" );
    le->AddEntry( eff_ID_DY_MC,           DY_MC_title.c_str(),      "lp" );
    le->AddEntry( eff_ID_All_MC,          All_MC_title.c_str(),     "lp" );
    le->AddEntry( eff_ID_MC[ last_file ], DY_Jets_title.c_str(),    "lp" );
    le->Draw();

    // Save plots
    if ( save == 1 ) {
      for ( Int_t j = 0; j < nOutputFormats; j++ ) {
	auxiliar_string.str("");
	auxiliar_string << file_name << "_" << variable[v] << "." << output_format[j];
	cout << "\t___________Saving plot of efficiency: \"" << auxiliar_string.str().c_str() << "\"___________" << endl;
	c1->SaveAs( auxiliar_string.str().c_str() );
      }
    }
    cout << "_____________________________________________________________________________________________" << endl;
  }
}
