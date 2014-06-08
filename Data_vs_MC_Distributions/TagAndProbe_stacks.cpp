 /*****************************************************************************
 *                                                                           *
 *                    This is a ROOT macro created by                        *
 *                    Angelo Santos ( angelo.santos@cern.ch )                *
 *                    on 15th of May, 2014.                                  *
 *                    ***********************************                    *
 *                                                                           *
 *      - Version 16:                                                        *
 *        * comes up from version V15. Include the possibility to save       *
 *          the histograms in end, so someone can analyze them later.        *
 *        * Now MC normalization no longer comes from the ratio between      *
 *          the Data and MC invariant mass around the Z mass (from 60 to     *
 *          120 GeV), but from the ratio between gaussian fits on them.      *
 *                                                                           *
 *      - Version 15:                                                        *
 *        * comes up from version V14. The weight factors from vertex        *
 *          multiplicity (applyied to MC samples) no longer come from        *
 *          input trees, but are built on the fly. It is because             *
 *          we have reaized that such weights depend on the selection        *
 *          requirements.                                                    *
 *        * Those weight factors are computed after apllying User's          *
 *          pre-defined selections on the muon pairs. For that, a T&P        *
 *          macro file called                                                *
 *             addNVtxWeight.cxx                                             *
 *          is implemented here as a function, where clones of the MC        *
 *          trees are created in to contain the new "weight" variable        *
 *          called now by "vertex_weight".
 *                                                                           * 
 *      - Version 14:                                                        *
 *        * comes up from version V13. This version takes care of            *
 *          setting up bottom plots for the ratios Data/MC and               *
 *          (Data - MC)/uncertainty_Data.                                    *
 *        * adds an option of applying a scale factor to MC samples          *
 *          chosen by the User. Actually, this option was included to        *
 *          apply a scale factor computed from Data/MC in the Z peak         *
 *          using T&P pairs when both muons are Tag muons (Tight2012=1).     *
 *        * Now User has to tell to the code if wanting to either            *
 *	    analyze all variables or only a small set of them. Being         *
 *          the latest, User needs to tell which set of variables are        *
 *          of interesting.
 *                                                                           *
 *      - Version 13, compared with verion 12 takes care of                  *
 *        output file names due inclusion of new variables.                  *
 *                                                                           *
 *                    ****** Summary of the macro ******                     *
 *                                                                           *
 *      This macro reads TTree entries from input ROOT files and             *
 *      create stacks of them, with exception of the first input             *
 *      file (which is addressed to Data events). Plots are created          *
 *      comparing Data and MC events for variables from the Tag and          *
 *      Probe method.                                                        *
 *                                                                           *
 *                    ************ Workflow ************                     *
 *                                                                           *
 *      There are 4 distinct blocks in this macro:                           *
 *        * In the first block, global functions are declared in.            *
 *                                                                           * 
 *        * In the second block, vectors of variables are declared in.       *
 *                                                                           *
 *        * The third block stars reading the file Setting_Variables.h       *
 *          (as and #include) where the USER may provide:                    *
 *           ** paths to the input ROOT files;                               *
 *           ** with corresponding weights and histogram colors;             *
 *           ** variables to be ploted;                                      *
 *           ** event selections;                                            *
 *           ** number of bins;                                              *
 *           ** X and Y axis ranges;                                         *
 *           ** X and Y axis labels;                                         *
 *           ** choice of canvas size;                                       *
 *           ** choice between log and linear scale;                         *
 *           ** choice for poping up the plots or for saving them only.      *
 *                                                                           *
 *        * In the fourth block, all USER's input parameters will            *
 *          be read into vectors and a loop starts over all variables        *
 *          required by the USER, poping up plots on the screen or saving    *
 *          them only (depending on the USER definition in the 3rd block).   *
 *                                                                           *
 *        * In the fifth block, all functions declared in the first          *
 *          block are finally defined. Some comments follow the functions    *
 *          in order to make them clearer.                                   *
 *                                                                           *
 *                                                                           * 
 *****************************************************************************/

#include <math.h>
#include <vector>
#include <sstream>
#include <iostream>

#include "TH1.h"
#include "TPad.h"
#include "TTree.h"
#include "TFile.h"
#include "TROOT.h"
#include "TChain.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"
#include "TString.h"

#include "Setting_Variables.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                   Here starts the first block                            //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
void read_trees ( vector <TFile*>, vector <TTree*> & );

void copy_trees ( vector <TTree*>, vector <TTree*> &, vector <string> );

void events_per_bin ( vector <TTree*>, vector <string>, vector <vector <double> >,
		      vector <string>, vector <string> );

void create_canvas ( vector <TCanvas*> &, vector <string>, Int_t, Int_t, Int_t, Int_t );

void create_pads (vector <TPad*> &, Int_t, vector <string>, Int_t, Float_t, Float_t,
		  Float_t, Float_t, Int_t);

void  fill_histograms ( vector <TTree*>, vector <TH1F*> &, vector <string>,
			vector <string>, vector <double>, vector <int>,
			vector <double>, vector <double>, vector <int>, Int_t, Int_t );

Double_t normalization_factor ( vector <TH1F*> );

void setup_stack ( vector <THStack*> &, vector <int>, vector <double>, vector <double>,
		   vector <double>, vector <double>, vector <string>, vector <string>,
		   vector <string>, Int_t, Int_t );

void add_histo_in_stack ( vector <TH1F*> &, Double_t, vector <THStack*> &, Int_t );

void create_legend ( vector <TLegend*> &, vector <TH1F*>, double, double,
		     double, double, vector <string>, int );

void ratios ( vector <string>, vector <TH1F*>, Int_t, Int_t, TH1F* &, vector <string> );

TTree* addNVtxWeight( vector <TTree*>, Int_t, vector <string> );


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                   Here starts the main function                          //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
void TagAndProbe_stacks () {

  //////////////////////////////////////////////////////////////////////////////
  //                                                                          //
  //                   Here starts the second block                           //
  //                                                                          //
  //////////////////////////////////////////////////////////////////////////////
  vector <int> nBins;
  vector <int> histo_color;
  vector <TFile*> file;
  vector <TTree*> old_tree;
  vector <TTree*> tree;
  vector <string> output_format;
  vector <string> varName;
  vector <string> few_variables;
  vector <string> events_selection;
  vector <string> selection_for_binning;
  vector <vector <double> > bin_edges;
  vector <double> weights;
  vector <double> xMin;
  vector <double> xMax;
  vector <double> yMin;
  vector <double> yMax;
  vector <string> X_label;
  vector <string> Y_label;
  vector <string> histo_title;
  vector <string> histo_label;
  vector <TCanvas*> canvas;
  vector <TPad*> pad;
  vector <THStack*> histoStack;
  vector <TLegend*> legend;


  //////////////////////////////////////////////////////////////////////////////
  //                                                                          //
  //                   Here starts the third block                            //
  //                                                                          //
  //////////////////////////////////////////////////////////////////////////////
  cout << "**************************************************************************" << endl;
  cout << "**************************************************************************" << endl;
  cout << "*\t Reading information from the list of variables set up" << endl;
  cout << "*\t by the USER in the file Setting_Variables.h." << endl;
  cout << "*" << endl;

  //  fill_bins (bin_edges);

  cout << "*" << endl;
  cout << "*\t Applying common selection: " << default_selection.c_str() << endl;


  /////////////////////////////////////////////////////////////////
  //                                                             //
  //              Here starts the fourth block                   //
  //                                                             //
  /////////////////////////////////////////////////////////////////
  cout << "**************************************************************************" << endl;
  // Reading each input ROOT file into a vector of TFile's.
  Int_t number_of_input_files = sizeof(input_files) / sizeof(string);
  for ( Int_t i = 0; i < number_of_input_files; i++ )
    file.push_back(TFile::Open( input_files[i].c_str() ));

  cout << "*" << endl;
  cout << "****** There are ___" << file.size() << "___ input root files:" << endl;
  cout << "*" << endl;

  for ( Int_t i = 0; i < file.size(); i++ )
    cout << "*\t file[" << i << "]:\t" << file[i]->GetName() << endl;

  /////////////////////////////////////////////////////////////
  // Reading types of output format files will be saved in.
  Int_t nOutputFormats = ( sizeof(output_file_format) / sizeof(string) );
  for (Int_t i = 0; i < nOutputFormats; i++) {
    output_format.push_back( output_file_format[i] );
  }
  cout << "*" << endl;

  cout << "**************************************************************************" << endl;
  /////////////////////////////////////////////////////////////
  // Passing weights of each input ROOT file into a vector of double's.
  cout << "****** Weight of each input file:" << endl;
  for ( Int_t i = 0; i < ( sizeof(weight) / sizeof(double) ); i++ ) {
    weights.push_back( weight[i] );
    cout << "*\t file[" << i << "]\t-> weight: " << weights[i] << endl;
  }
  cout << "*" << endl;

  /////////////////////////////////////////////////////////////
  // Combination between defaulf event selections ("default_selection") and
  // selection on the mass ("mass_selection[]") of each input ROOT file.
  stringstream selection_of_events;
  for ( Int_t i = 0; i < ( sizeof(mass_selection) / sizeof(string) ); i++ ) {
    if ( i == 0 ) {
      selection_of_events << mass_selection[i] << " && " << default_selection;
      selection_for_binning.push_back( selection_of_events.str() );
    }
    else {
      selection_of_events << mass_selection[i] << " && " << default_selection;
      selection_for_binning.push_back( selection_of_events.str() );

      selection_of_events.str("");
      //      selection_of_events << "( " << mass_selection[i] << " && " << default_selection << " )*weight";
      selection_of_events << "( " << mass_selection[i] << " && " << default_selection << " )";
    }
    //    cout << "...... selection_of_events: " << selection_of_events.str().c_str() << endl;
    events_selection.push_back( selection_of_events.str() );
    selection_of_events.str("");
  }
  cout << "*" << endl;

  /////////////////////////////////////////////////////////////
  // Compute number of variables and create a vector of variables.
  Int_t nVariables = ( sizeof(list_of_variables) / sizeof(string) );
  for (Int_t i = 0; i < nVariables; i++) {
    varName.push_back( list_of_variables[i] );
  }

  /////////////////////////////////////////////////////////////
  // In case of wanting to get results only for a small set of variables (when getting the number
  // of entries, for example), User has been told which variable names he/she is interested in.
  Int_t nFewVariables = ( sizeof(small_set_of_variables) / sizeof(string) );
  for (Int_t iVar = 0; iVar < nFewVariables; iVar++) {
    few_variables.push_back( small_set_of_variables[iVar] );
  }

  cout << "**************************************************************************" << endl;
  /////////////////////////////////////////////////////////////
  // Tell the USER what variables will be analyzed in.
  //  if ( output_entries_per_bin != 1 ) {
  // In case of analyzing a more complete list of variables...
  if ( analyze_few_variables == 0 ) {
    cout << "****** These " << nVariables << " variables will be analyzed:" << endl;
    for (Int_t i = 0; i < nVariables; i++) cout << "*\t-> " << varName[i] << endl;
  }
  // In case of analyzing a small set of variables...
  else {
    cout << "****** These " << nFewVariables << " variables will be analyzed:" << endl;
    for (Int_t i = 0; i < nFewVariables; i++) cout << "*\t-> " << few_variables[i] << endl;
  }
  cout << "*" << endl;
  //}

  /////////////////////////////////////////////////////////////
  // Loop over the number of variables to create vectors for the number of bins,
  // X and Y axis ranges, X and Y axis labels and vectors for the titles, colors
  // and legends of histograms.
  for (Int_t i = 0; i < nVariables; i++) {
    nBins.push_back( number_of_bins[i] );
    xMin.push_back( x_minimum[i] );
    xMax.push_back( x_maximum[i] );
    yMin.push_back( y_minimum[i] );
    yMax.push_back( y_maximum[i] );
    X_label.push_back( x_axis_label[i] );
    Y_label.push_back( y_axis_label[i] );
    histo_title.push_back( title_of_histogram[i] );
  }
  for (Int_t i = 0; i < file.size(); i++) {
    histo_color.push_back( color_of_histogram[i] );
    histo_label.push_back( label_of_histogram[i] );
  }

  cout << "**************************************************************************" << endl;
  /////////////////////////////////////////////////////////////
  // Read trees
  cout << "****** Tree entries of each input ROOT file:" << endl;
  read_trees(file, old_tree);
  cout << "*" << endl;

  cout << "**************************************************************************" << endl;
  /////////////////////////////////////////////////////////////
  // Copy trees based on selection of pairs
  cout << "****** Copying trees to add weights from vertex multiplicity "
       << "after applying baseline selection to MC samples:" << endl;
  copy_trees(old_tree, tree, events_selection);
  cout << "*" << endl;

  /////////////////////////////////////////////////////////////
  // Output on screen of entries/bin/file/ variable.
  if ( output_entries_per_bin > 0 )
    events_per_bin ( tree, varName, bin_edges, selection_for_binning, few_variables );

  /////////////////////////////////////////////////////////////
  // Stop to run the macro if would like only to get number of entries
  if ( output_entries_per_bin == 1 ) {
    cout << "*" << endl;
    cout << "*\t<<<<<< Done! >>>>>>" << endl;
    cout << "*" << endl;
    cout << "**************************************************************************" << endl;
    cout << "**************************************************************************" << endl;
    gSystem->Exit(0);
  }

  cout << "**************************************************************************" << endl;
  /////////////////////////////////////////////////////////////
  // Check if plots should be saved and/or poped up, as well as
  // if the macro should run in batch mode concerning User's
  // input parameters in the Setting_Variables.h file
  stringstream file_name;
  if ( gROOT->IsBatch() ) {
    pop_save = 1;
    cout << "*\tIt is running in batch mode. Plots will be saved in" << endl;
    for ( Int_t j = 0; j < nOutputFormats; j++ ) {
      cout << "*\t                                                  ."
	   << output_format[j].c_str() << " format" << endl;
    }
    cout << "*" << endl;
  }
  else if ( pop_save == 0 )
    cout << "*\tPlots will be poped up on the screen, but not saved." << endl;
  else if ( pop_save == 1 ) {
    gROOT->SetBatch( kTRUE );
    cout << "*\tPlots will be saved in" << endl;
    for ( Int_t j = 0; j < nOutputFormats; j++ )
      cout << "*\t                     ." << output_format[j].c_str() << " format" << endl;
    cout << "*" << endl;
  }
  else if ( pop_save == 2 ) {
    cout << "*\tPlots will be poped up on the screen and also saved in" << endl;
    for ( Int_t j = 0; j < nOutputFormats; j++ ) {
      cout << "*\t                                                     ."
	   << output_format[j].c_str() << " format" << endl;
    }
    cout << "*" << endl;
  }
  else
    cout << "WARNING: Value" << pop_save 
	 << "given to \"pop_save\" is wrong. Only 0, 1 or 2 are allowed." << endl;

  cout << "*" << endl;

  cout << "**************************************************************************" << endl;
  cout << "*\t Computing MC normalization factor" << endl;
  /////////////////////////////////////////////////////////////
  // Filling histograms only for the variable mass and compute the normalization factor
  // in the range mass[60, 120] GeV to be applied to the DY sample with the same range.
  vector <TH1F*> auxiliar_histo; // Histograms used only to compute the Data/MC factor
  Double_t normalization;
  if ( normalize == 0 ) normalization = 1.0; // If User does NOT want to normalize MC
  if ( normalize == 1 ) {                    // If User wants to normalize MC samples
    for ( Int_t i = 0; i < nVariables; i++ ) {
      // What is the vector related to the mass variable?
      if ( varName[i] == mass_variable ) {
	fill_histograms ( tree, auxiliar_histo, varName, events_selection,
			  weights, nBins, xMin, xMax, histo_color, i, 999 );
	// Get the normalization factor
	normalization = normalization_factor ( auxiliar_histo );
	break; // Skip from the loop! Normalization already computed!
      }
    }
  }
  // If User wants to normalize MC samples using his pre-defined value
  else if ( normalize == 2 ) normalization = User_normalization;
  else {
    cout << "*\t ERROR: There is no option from normalize > 2" << endl;
  }

  // Delete components of "auxiliar_histo" vector
  //  auxiliar_histo.erase ( auxiliar_histo.begin(), auxiliar_histo.end() );
  cout << "*" << endl;


  cout << "**************************************************************************" << endl;
  // ______________________________________________________________________________________
  // Fill histograms and create stacks
  //
  // First of all, vector "histo" will be filled. Then the vector "histoStack"
  // is set up with labels and Y axis range. Finally, vector "histo" is added
  // in the stacks by vector "histoStack".
  Int_t count_pads = 0;

  cout << "****** Creating plots for each variable:" << endl;
  for (Int_t i = 0; i < nVariables; i++) {
    /////////////////////////////////////////////////////////////
    // In case of USER wanting to get results only for a small set of variables (when getting the number
    // of entries, for example), User has been told which variable names he/she is interested in.
    //    * If "analyze_few_variables == 0", the codes runs over all listed variables.
    //    * If "analyze_few_variables == 1", the codes runs over only a small set pre-defined by User.

    Int_t analyze = 1; // In principle, all variables will be analyzed.
    Int_t index   = 0; // Index for the case of list of few variables

    if (analyze_few_variables == 1) {
      for(Int_t iFewVar = 0; iFewVar < nFewVariables; iFewVar++) {
	if (varName[i] == few_variables[iFewVar]) {
	  analyze = 1;     // Go ahead analyzing this variable
	  index = iFewVar; // Index for the current variable
	  break;
	}
	else
	  analyze = 0; // Do not analyze this variable
      }
    }
    else index = i;    // In case of going to analyze a complete list of variables

    /////////////////////////////////////////////////////////////
    // Skip of analyzing this variables. This variable is not of User's interesting
    if (analyze == 0) continue;

    cout << "______________________________________________" << endl;
    cout << "*\t Variable " << (i+1) << "\t:\t" << varName[i] << endl;

    /////////////////////////////////////////////////////////////
    // Create a vector of canvases. Each canvas corresponds to each i-th variable.
    create_canvas (canvas, varName, wCanvas, hCanvas, i, index);
    canvas[index]->cd();

    /////////////////////////////////////////////////////////////
    // Create 3 different pads: distribution, Data/MC and pull.
    //
    // This pad contains Data and MC distributions
    create_pads (pad, i, varName, 1, padXmin, padYmin1, padXmax, padYmax1, count_pads);
    // This pad contains Data/MC ratio
    create_pads (pad, i, varName, 2, padXmin, padYmin2, padXmax, padYmax2, count_pads+1);
    // This pad contains (Data - MC)/Data_uncertainty
    create_pads (pad, i, varName, 3, padXmin, padYmin3, padXmax, padYmax3, count_pads+2);

    /////////////////////////////////////////////////////////////
    // Filling histograms for the correspondent i-th variable
    pad[count_pads]->cd();

    vector <TH1F*> histo;
    fill_histograms (tree, histo, varName, events_selection, weights, nBins, xMin, xMax, histo_color, i, 0);

    /////////////////////////////////////////////////////////////
    // Setting up the stack of histograms for the correspondent i-th variable
    setup_stack (histoStack, nBins, xMin, xMax, yMin, yMax, histo_title, Y_label, X_label, i, index);

    /////////////////////////////////////////////////////////////
    // Add histograms in stacks.
    add_histo_in_stack (histo, normalization, histoStack, index);

    /////////////////////////////////////////////////////////////
    // Linear (0) or log (1) scale
    if ( linear_log_scale == 1 ) {
      pad[count_pads]->SetLogy();
    }

    /////////////////////////////////////////////////////////////
    // Data events will be draw above stacks.
    histoStack[index]->Draw();
    histo[0]->Draw("ep same");

    /////////////////////////////////////////////////////////////
    // Fill the legend
    //
    // A component of the "histoStack" vector will be draw into each canvas
    // with respective legends.
    create_legend (legend, histo, xMinLegend, yMinLegend, xMaxLegend, yMaxLegend, histo_label, index);
    legend[index]->Draw();
    pad[count_pads]->SetTicks(1, 1);
    pad[count_pads]->RedrawAxis();

    /////////////////////////////////////////////////////////////
    // This pad contains Data/MC ratio
    pad[count_pads+1]->cd();

    TH1F *DataMC_ratio;
    ratios (varName, histo, i, 1, DataMC_ratio, X_label);
    DataMC_ratio->Draw("P");

    TLine *line_DataMC_ratio = new TLine(xMin[i], 1.0, xMax[i], 1.0);
    line_DataMC_ratio->SetLineColor(line_color);
    line_DataMC_ratio->SetLineWidth(line_width);
    line_DataMC_ratio->Draw("same");

    pad[count_pads+1]->SetGrid();
    pad[count_pads+1]->SetTicks(1, 1);
    pad[count_pads+1]->RedrawAxis();

    /////////////////////////////////////////////////////////////
    // This pad contains (Data - MC)/Data_uncertainty
    pad[count_pads+2]->cd();

    TH1F *pull;
    ratios (varName, histo, i, 2, pull, X_label);
    pull->Draw("B");

    TLine *line_pull = new TLine(xMin[i], 0.0, xMax[i], 0.0);
    line_pull->SetLineColor(line_color);
    line_pull->SetLineWidth(line_width);
    line_pull->Draw("same");

    pad[count_pads+2]->SetGrid();
    pad[count_pads+2]->SetTicks(1, 1);
    pad[count_pads+2]->RedrawAxis();
    count_pads = count_pads + 3;

    /////////////////////////////////////////////////////////////
    // Save and/or pop plots
    if ( pop_save > 0 ) {
      for ( Int_t j = 0; j < nOutputFormats; j++ ) {
	if ( linear_log_scale == 1 ) {
	  if ( varName[i] == "(TMath::Pi() - pair_collinearity1)" )
	    file_name << output_file_name << "_" << "pair_collinearity1" << "_log." << output_format[j];
	  else if ( varName[i] == "(tag_NewTuneP_pt - NewTuneP_pt) / (tag_NewTuneP_pt + NewTuneP_pt)" )
	    file_name << output_file_name << "_" << "pt_balance" << "_log." << output_format[j];
	  else
	    file_name << output_file_name << "_" << varName[i] << "_log." << output_format[j];
	}
	if ( linear_log_scale == 0 ) {
	  if ( varName[i] == "(TMath::Pi() - pair_collinearity1)" )
	    file_name << output_file_name << "_" << "pair_collinearity1" << "_log." << output_format[j];
	  else if ( varName[i] == "(tag_NewTuneP_pt - NewTuneP_pt) / (tag_NewTuneP_pt + NewTuneP_pt)" )
	    file_name << output_file_name << "_" << "pt_balance" << "_log." << output_format[j];
	  else
	    file_name << output_file_name << "_" << varName[i] << "_linear." << output_format[j];
	}

	canvas[index]->SaveAs( file_name.str().c_str() );

	// Save histograms in a ROOT format, so one can analyzed them later
	// (optimization cuts, for example).
	if ( j+1 == nOutputFormats ) {
	  file_name.str("");
	  if ( varName[i] == "(TMath::Pi() - pair_collinearity1)" )
	    file_name << "Histograms_" << output_file_name << "_" << "pair_collinearity1.root";
	  else if ( varName[i] == "(tag_NewTuneP_pt - NewTuneP_pt) / (tag_NewTuneP_pt + NewTuneP_pt)" )
	    file_name << "Histograms_" << output_file_name << "_" << "pt_balance.root";
	  else
	    file_name << "Histograms_" << output_file_name << "_" << varName[i] << ".root";

	  TFile *fileOutPut = new TFile( file_name.str().c_str(), "RECREATE" );
	  fileOutPut->cd();
	  // Looper over the number of histograms
	  gStyle->SetOptStat(kTRUE);
	  for ( Int_t iHisto = 0; iHisto < histo.size(); iHisto++ )  histo[iHisto]->Write();
	  delete fileOutPut;
	  gStyle->SetOptStat(kFALSE);
	}

	file_name.str("");
      }
    }

    // Delete all components of the vector "histo".
    histo.erase ( histo.begin(), histo.end() );
  } // End of for ( Int_t i = 0; i < nVariables; i++)

  /////////////////////////////////////////////////////////////
  cout << "*" << endl;
  cout << "*\t<<<<<< Done! >>>>>>" << endl;
  cout << "*" << endl;
  cout << "**************************************************************************" << endl;
  cout << "**************************************************************************" << endl;

  // Deleting auxiliary file
  gROOT->ProcessLine(".!rm -f TagProbeZ_withNVtxWeights.root");

  Avengers ();

} // End of the main function "void TagAndProbe_stacks()".


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                   Here starts the fifth block                            //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

//=================================================================================================
//=================================================================================================
void read_trees (vector <TFile*> f, vector <TTree*> &t ) {
  // This function reads trees from input ROOT
  // files into vectors of TTree's.


  for (Int_t i = 0; i < f.size(); i++) {
    t.push_back( (TTree*)f[i]->Get("tpTree/fitter_tree") );
    cout << "*\t file[" << i << "]\t-> Entries: " << t[i]->GetEntries() << endl;
  }
}

//=================================================================================================
//=================================================================================================
void copy_trees ( vector <TTree*> old_tree, vector <TTree*> &new_tree, vector <string> events_selection ) {
  // This function creates new trees from old ones with and additional variable ("vertex_weight")
  // based on seleted pair of Tag & Probe muons.

  for (Int_t i = 0; i < old_tree.size(); i++) {
    // Should not apply any weight concerning vertex multiplicity in Data
    if ( i == 0 ) {
      cout << "*\t tree[" << i << "]\t"
	   << ".................................................."
	   << "..................................................";
      new_tree.push_back( (TTree*)old_tree[i] );
      cout << " Entries: " << new_tree[i]->GetEntries();
    }
    else {
      cout << endl << "*\t tree[" << i << "]\t";

      new_tree.push_back( (TTree*)addNVtxWeight( old_tree, i, events_selection ) );
      cout << " Entries: " << new_tree[i]->GetEntries( events_selection[i].c_str() );
    }
  }
  cout << endl;
}

//=================================================================================================
//=================================================================================================
void events_per_bin ( vector <TTree*> t, vector <string> variable, vector <vector <double> > bin_edges,
		      vector <string> selection_for_binning, vector <string> few_variables ) {
  // This function compute the number of entries
  // per bin per input file per variable.

  stringstream selection;

  cout << "****** Entries per bin per file per variable:" << endl;
  for ( Int_t i = 0; i < variable.size(); i++ ) {
    // In case of USER wanting to get results only for a small set of variables (when getting the number
    // of entries, for example), User has been told which variable names he/she is interested in.
    //    * If "analyze_few_variables == 0", the codes runs over all listed variables.
    //    * If "analyze_few_variables == 1", the codes runs over only a small set pre-defined by User.
    Int_t analyze = 1; // In principle, all variables will be analyzed.

    if (analyze_few_variables == 1) {
      for(Int_t iFewVar = 0; iFewVar < few_variables.size(); iFewVar++) {
	if (variable[i] == few_variables[iFewVar]) {
	  analyze = 1; // Analyze this variable
	  break;
	}
	else
	  analyze = 0; // Do not analyze this variable
      }
    }

    if (analyze == 0)  // Skip of analyzing this variables. It is not of User's interesting.
      continue;

    cout << "*\t* Variable -> <<<<<   " << variable[i] << "   >>>>>" << endl;
    cout << "*\t\t** Bin edges:\t\t{";
    for ( Int_t j = 0; j < bin_edges[i].size(); j++ ) {
      cout << bin_edges[i].at(j);
      if ( j == (bin_edges[i].size() - 1) ) cout << "};";
      else cout << ",\t";
    }
    cout << endl;
    for ( Int_t j = 1; j < t.size(); j++ ) {
      cout << "\t\t if (sample == " << (j-1) << ")\t{long File" << "[] =\t{";

      for ( Int_t k = 0; k < ( bin_edges[i].size() - 1 ); k++ ) {
	selection << selection_for_binning[j]
		  << " && (" << variable[i] << " >= " << bin_edges[i].at( k )
		  << ") && (" << variable[i] << " < " << bin_edges[i].at( k+1 ) << ")";

	if ( k < (bin_edges[i].size() - 2) )
	  cout << t[j]->GetEntries( selection.str().c_str() ) << ",\t";
	else
          cout << t[j]->GetEntries( selection.str().c_str() ) << "};}";

	selection.str("");
      }
      cout << endl;
    }
    cout << "*" << endl;
  }
}

//=================================================================================================
//=================================================================================================
void create_canvas (vector <TCanvas*> &c, vector <string> variable,
		    Int_t width, Int_t height, Int_t j, Int_t index) {
  // This function sets the canvas concerning
  // input parameters defined by the USER.

  c.push_back( new TCanvas() );
  c[index]->Divide(1,1);
  c[index]->SetRightMargin( canvas_ratio );
  c[index]->SetTitle( variable[j].c_str() );
  c[index]->SetWindowSize(width, height);
  gROOT->SetStyle("Plain");

  gStyle->SetPadTopMargin(PadToptMargin);
  gStyle->SetPadLeftMargin(PadLeftMargin);
  gStyle->SetPadRightMargin(PadRightMargin);
  gStyle->SetPadBottomMargin(PadBottomMargin);

  gStyle->SetOptStat(kFALSE);
}

//=================================================================================================
//=================================================================================================
void create_pads (vector <TPad*> &pad, Int_t variable_order, vector <string> variable,
		  Int_t pad_type, Float_t padXMin, Float_t padYMin, Float_t padXMax,
		  Float_t padYMax, Int_t j) {
  // This function sets the pads concerning input parameters defined by the USER.
  // There are three kinds of pads (one bellow the other):
  //   - in the top, Data and MC distributions;
  //   - in the middle, Data/MC ratio;
  //   - in the bottom, (Data - MC)/Data_uncertainty.

  stringstream pad_name;
  pad_name << variable[variable_order] << pad_type;
  pad.push_back( new TPad(pad_name.str().c_str(), pad_name.str().c_str(), padXMin, padYMin, padXMax, padYMax) );

  pad[j]->Draw();

  if (pad_type == 1) {
    pad[j]->SetTopMargin(0.);
    pad[j]->SetBottomMargin(0.);
  }
  else if (pad_type == 2) {
    pad[j]->SetTopMargin(0.);
    pad[j]->SetBottomMargin(0.);
  }
  else if (pad_type == 3) {
    pad[j]->SetTopMargin(0.);
  }
  else {
    cout << "ERROR: wrong \"pad_type\" value. Please, check this variable."
    gSystem->Exit(0);
  }

  pad_name.str("");
  pad_name.clear();
}

//=================================================================================================
//=================================================================================================
void fill_histograms ( vector <TTree*> t, vector <TH1F*> &h,
		       vector <string> variable, vector <string> events_selection,
		       vector <double> weights, vector <int> nBins, vector <double> xMin,
		       vector <double> xMax, vector <int> color, Int_t j, Int_t k ) {
  // A branch will be copied to "histo_temp", which is previously definded
  // by the USER. It is important because the USER can have total control
  // over bin numbers and x axis range.
  //
  // Histogram "h" will be a clone of "histo_temp", being used in the stacks later on.

  // These string variables are used to handle the way as the histograms
  // are filled via "Draw()".
  const char* variable_name;
  string var_temp;
  stringstream h_Name = "";

  // Loop over all trees. Each tree corresponds to one histogram.
  for (Int_t i = 0; i < t.size(); i++) {
    // Give a different name to the histogram "histo_temp" for each j-th
    // variable and i-th input root file. Then for each "j" and "i" values,
    // a name "histo_tempji" is given to the histogram. For example:
    // "histo_temp00", "histo_temp01", "histo_temp10", "histo_temp11", etc.
    //
    // If the histogram name was kept unchanged, the TH1F would be replaced
    // at each loop step, probably causing a fail due a memory leakage.
    if ( k == 999) {
      // "k = 999" means that this functions has been called to compute Data/MC
      // scale factors to normalize the MC samples later on.
      h_Name << "histo_temp" << j << i << k;
      // When "k = 999", variable analyzed is the invariant mass, so 1000 bins
      // are used to improve computation of MC normalization factor.
      TH1F *histo_temp = new TH1F( h_Name.str() .c_str(), "", 1000, xMin[j], xMax[j] );
    }
    else {
      h_Name << "histo_temp" << j << i;
      TH1F *histo_temp = new TH1F( h_Name.str() .c_str(), "", nBins[j], xMin[j], xMax[j] );
    }

    // "Draw()" function gets a "const char" type as input. Then it is
    // necessary to handle with "string"s and "const char"s in order to
    // provide the correct variable name to be passed to the correct histogram.
    // "k = 999" is used to create histograms to compute MC normalization factor.
    if ( k == 999) variable_name = mass_variable.c_str();
    // Tell which variable would be passed in via "j".
    else           variable_name = variable[j].c_str();

    //-------------------------------------------------------------------------
    var_temp = variable_name;                    // Copy a "const char" to a "string".
    var_temp = var_temp + " >> " + h_Name.str(); // Include the histogram name.
    variable_name = "";                          // Clear the "const char" object.
    variable_name = var_temp.c_str();            // Copy a "string" to a "const char".

    // Draw variable to a histogram applying event selections defined by USER.
    if (i == 0) { // In case of Data sample, use only baseline selections
      t[i]->Draw( variable_name, events_selection[i].c_str() );
    }
    else { // In case of MC samples, add the "vertex_weight" to the baseline selections
      stringstream weighted_selection;
      weighted_selection << events_selection[i] << "*vertex_weight";

      t[i]->Draw( variable_name, weighted_selection.str().c_str() );
    }

    // Each element of the vector "h" is a clone of each "histo_temp" histogram,
    // taking care to get different names.
    h_Name << "clone";
    h.push_back( (TH1F*)histo_temp->Clone( h_Name.str().c_str() ) );

    if ( i == 0 ) { // Data events
      h[i]->Scale( weights[i] );
      h[i]->SetLineColor(1);
      h[i]->SetMarkerColor(1);
      h[i]->SetMarkerSize(0.7);
      h[i]->SetMarkerStyle(20);
    }
    else {          // MC samples
      h[i]->Scale( weights[i] );
      h[i]->SetFillColor( color[i] );
    }

    // Clear objects for the next loop step
    variable_name = "";
    h_Name.str("");
    h_Name.clear();
    // Deleting histograms
    delete histo_temp;
  }

  // Deleting histograms
  //  delete histo_temp;
  //  h_temp.erase ( h_temp.begin(), h_temp.end() );
} // End of "void fill_histograms()".

//=================================================================================================
//=================================================================================================
Double_t normalization_factor ( vector <TH1F*> h ) {
  // This function compute integral of invariant mass distributions,
  // in the range between 60 and 120 GeV, for Data and MC events.
  // If requested by the USER, MC events are normalized by Data/MC ratio.

  //  Double_t integral = 0.;
  Double_t ratio = 0.;
  Double_t bin_min = 0;
  Double_t bin_max = 0;

  TCanvas *canvas_normalization = new TCanvas("canvas_normalization", "canvas_normalization");
  canvas_normalization->cd();
  gStyle->SetOptStat(kFALSE);

  TH1F *histo_normalization_Data   = new TH1F("histo_normalization_Data", "histo_normalization_Data",
					      1000, 0., 3000.);
  TH1F *histo_normalization_MC     = new TH1F("histo_normalization_MC"  , "histo_normalization_MC"  ,
					      1000, 0., 3000.);
  TH1F *histo_MC_normalized        = new TH1F("histo_MC_normalized"     , "histo_MC_normalized"     ,
					      1000, 0., 3000.);

  //  TF1 *fit_Data = new TF1("fit_Data", "expo(0)*gaus(0)*TMath::BreitWigner(x, [0], [1])", 60., 120.);
  //  TF1 *fit_MC   = new TF1("fit_MC",   "expo(0)*gaus(0)*TMath::BreitWigner(x, [0], [1])", 60., 120.);

  histo_normalization_Data->Add( h[0] );

  // Loop over all MC samples and make a stack for the invariant mass
  for ( Int_t j = 1; j < h.size(); j++ ) histo_normalization_MC->Add( h[j] );

  histo_normalization_Data->GetYaxis()->SetTitle("Events");
  histo_normalization_Data->GetXaxis()->SetTitle("M_{\mu\mu} [GeV/c^{2}]");
  histo_normalization_Data->SetTitle("");
  histo_normalization_Data->SetLineColor(1);
  histo_normalization_Data->SetMarkerColor(1);
  histo_normalization_Data->SetMarkerSize(0.7);
  histo_normalization_Data->SetMarkerStyle(20);
  histo_normalization_Data->Sumw2();

  histo_normalization_MC->SetLineColor(2);
  histo_normalization_MC->SetLineWidth(2);
  histo_normalization_MC->SetLineStyle(2);


  //  TFitResultPtr result_fit_Data = histo_normalization_Data->Fit("fit_Data", "S0");
  //  TFitResultPtr result_fit_MC   = histo_normalization_MC->Fit(  "fit_MC"  , "S0");

  cout << "*" << endl;
  //  cout << "*\t* Integral from Data Fit(60, 120): " << fit_Data->Integral(60., 120.) << endl;
  //  cout << "*\t* Integral from MC   Fit(60, 120): " << fit_MC->Integral(  60., 120.) << endl;

  // MC scale factor is the ratio between integral in Data fit and integral in MC fit.
  //  ratio = (double)fit_Data->Integral(60., 120.) / (double)fit_MC->Integral(60., 120.);
  //  cout << "*\t* MC normalization factor: " << ratio << endl;

  //  fit_Data->SetLineColor(4);
  //  fit_Data->SetLineWidth(2);
  //  fit_MC->SetLineColor(2);
  //  fit_MC->SetLineWidth(2);

  histo_normalization_Data->Draw("ep");
  //  fit_Data->Draw("same");
  //  fit_MC->Draw("same");

  for ( Int_t j = 1; j < h.size(); j++ ) {
    bin_min = h[j]->FindBin( 60 );
    bin_max = h[j]->FindBin( 120 );
    ratio = ratio + ( h[j]->Integral( bin_min, bin_max ) );
    //    cout << ".... integral[" << j << "](" << bin_min << ", " << bin_max << "): " << integral << endl;
  }

  if ( !(ratio > 0) ) {
    cout << "*\t\t ERROR: MC Integral = 0 when computing MC normalization!!!!!!!!!\n" << endl;
    gSystem->Exit(0);
  }

  bin_min = h[0]->FindBin( 60 );
  bin_max = h[0]->FindBin( 120 );
  //  cout << ".... integral[0](" << bin_min << ", " << bin_max << "): " << h[0]->Integral( bin_min, bin_max ) << endl;
  ratio = (double)h[0]->Integral( bin_min, bin_max ) / ratio;
  cout << "*\t* MC Normalized by factor: " << ratio << endl;


  histo_MC_normalized->Add( histo_normalization_MC, (double)ratio );
  histo_MC_normalized->SetLineColor(9);
  histo_MC_normalized->SetLineWidth(2);
  histo_MC_normalized->Draw("histo same");
  histo_normalization_MC->Draw("histo same");

  stringstream ScaleFactor;
  ScaleFactor << "MC Normalized by factor: " << ratio;

  TLegend *legend_normalization = new TLegend(0.5, 0.6, 0.88, 0.88);
  legend_normalization->SetBorderSize(0);
  legend_normalization->SetFillColor(0);
  legend_normalization->AddEntry(histo_normalization_Data, "Data-RunC, 7.05 fb^{-1}", "lp");
  legend_normalization->AddEntry(histo_normalization_MC,   "All MC Samples",          "l");
  legend_normalization->AddEntry(histo_MC_normalized,      ScaleFactor.str().c_str(), "l");
  //  legend_normalization->AddEntry(fit_Data,                 "Data Fit",                "l");
  //  legend_normalization->AddEntry(fit_MC,                   "MC Fit",                  "l");
  legend_normalization->Draw();

  canvas_normalization->SaveAs("Data_MC_SF.png");
  canvas_normalization->SaveAs("Data_MC_SF.pdf");
  canvas_normalization->SaveAs("Data_MC_SF.root");

  return ratio;
}

//=================================================================================================
//=================================================================================================
void setup_stack ( vector <THStack*> &h, vector <int> nBins,
		   vector <double> xMin, vector <double> xMax,
		   vector <double> yMin, vector <double> yMax,
		   vector <string> title, vector <string> y_label,
		   vector <string> x_label, Int_t j, Int_t index ) {
  // This fuction sets up the THStack vector "h" with labels, title
  // and Y axis range. Each component of "h" corresponds to one
  // variable (j-th variable).

  h.push_back( new THStack() );
  stringstream h_Name = "";
  h_Name << "h_stack" << j;

  // A way to provide labels to THStack is through a TH1F
  TH1F *h_stack = 0;
  h_stack = new TH1F( h_Name.str() .c_str(),"", nBins[j], xMin[j], xMax[j] );
  h_stack->GetXaxis()->SetTitle( x_label[j].c_str() );

  h_stack->GetYaxis()->SetTitle( y_label[j].c_str() );
  h_stack->GetYaxis()->SetTitleOffset( y_title_offset1 );
  h_stack->GetYaxis()->CenterTitle();
  h_stack->GetYaxis()->SetTitleSize(y_titleSize1);
  h_stack->GetYaxis()->SetLabelSize(y_labelSize1);

  // THStack receives labels (already set up by a TH1F above) via SetHistogram().
  // Title and Y axis range are provided directly to the THStack.
  h[index]->SetHistogram( h_stack );
  h[index]->SetMinimum( yMin[j] );
  //  h[index]->SetMaximum( yMax[j] );
  h[index]->SetTitle( title[j].c_str() );

  // Clear the "stringstream" object to free the memory.
  h_Name.str("");
  h_Name.clear();
} // End of "void setup_stack()".

//=================================================================================================
//=================================================================================================
void add_histo_in_stack ( vector <TH1F*> &h, Double_t normalization,
			  vector <THStack*> &h_Stack, Int_t index ) {
  // This add each component of the TH1F vector "h" in stack into
  // the THStack "h_Stack". It is added in descending order, so the the
  // histogram from the first input root file will be in the top of the
  // stack, while the last one will be in the bottom.

  Double_t stack_MC_integral = 0.;
  Double_t Data_integral = 0.;

  for (Int_t i = h.size() - 1; i > 0; i--) {
    h[i]->Scale( normalization );
    h_Stack[index]->Add( h[i] );

    stack_MC_integral = stack_MC_integral + h[i]->Integral();
  }

  Data_integral = h[0]->Integral();
  cout << "*\t\t * Histo[Data] integral = " << Data_integral << endl;
  cout << "*\t\t * Histo[MC] integral = " << stack_MC_integral << endl;
  cout << "*\t\t * Ratio Data/MC = " << (1.0*Data_integral)/stack_MC_integral << endl;
}

//=================================================================================================
//=================================================================================================
void create_legend ( vector <TLegend*> &l, vector <TH1F*> h, double xMin,
		     double yMin, double xMax, double yMax,
		     vector <string> label, int index ) {
  // This function creates a legend for each variable.

  l.push_back( new TLegend(xMin, yMin, xMax, yMax) );
  l[index]->SetFillColor(0);
  l[index]->SetBorderSize(1);

  // Provide legends looping over all histograms
  for (Int_t i = 0; i < h.size(); i++)
    if ( i == 0 )
      l[index]->AddEntry(h[i], label[i].c_str(),"pl");
    else
      l[index]->AddEntry(h[i], label[i].c_str(),"f");
}

//=================================================================================================
//=================================================================================================
void ratios ( vector <string> variable, vector <TH1F*> histo, Int_t j,
	      Int_t ratio_type, TH1F* &h_ratio, vector <string> x_label ) {
  // This function takes care of two different Data vs. MC comparisions:
  //   - ratio_type = 1  --> Data/MC ratio
  //   - ratio_type = 2  --> (Data - MC)/Data_uncertainty

  stringstream ratios_name;
  ratios_name << variable[j] << ratio_type;

  // Initialize histograms with a copy of Data histogram, only to get the same
  // range and binning.
  h_ratio                    = (TH1F*)histo[0]->Clone( ratios_name.str().c_str() );
  TH1F *first_aux_histogram  = (TH1F*)histo[0]->Clone( ratios_name.str().c_str() );
  
  // Scale histograms to zero before starting to fill them
  h_ratio             ->Scale(0.); // This histogram will contain the final ratio
  first_aux_histogram ->Scale(0.); // This will be an auxiliar histogram
  
  // In case of Data/MC ratio
  if (ratio_type == 1) {
    // Auxiliar histograms are filled in order to have a stack of all MC samples
    for ( Int_t ih = 1; ih < histo.size(); ih++ )  first_aux_histogram ->Add( histo[ih] );

    // Finally, ratio is Data divided by stack of all MC samples
    h_ratio->Divide(histo[0], first_aux_histogram);
    
    h_ratio->GetYaxis()->SetTitle( y_title2.c_str() );    // title in y axis
    h_ratio->GetYaxis()->SetTitleOffset(y_title_offset2); // distance of title from y axis
    h_ratio->GetYaxis()->SetTitleSize(y_titleSize2);      // size of title in y axis
    h_ratio->GetYaxis()->SetLabelSize(y_labelSize2);      // size of numbers in y axis
    h_ratio->GetYaxis()->SetRangeUser(y_Min2, y_Max2);    // range of y axis
  }
  // In case of (Data - MC)/Data_uncertainty
  else if (ratio_type == 2) {
    // Auxiliar histograms are filled in order to have a stack of all MC samples
    first_aux_histogram->Scale(0.);
    for ( Int_t ih = 1; ih < histo.size(); ih++ )  first_aux_histogram ->Add( histo[ih] );

    // Initialize Data_uncertainty ratio
    TH1F *h_uncertainty  = (TH1F*)histo[0]->Clone( ratios_name.str().c_str() );
    h_uncertainty->Scale(0.);

    // Get values of Data_uncertainty bin-by-bin
    for ( Int_t ibin = 1; ibin < (histo[0]->GetNbinsX() + 1); ibin++ )
      h_uncertainty->SetBinContent(ibin, histo[0]->GetBinError(ibin));

    // Initiaclize another auxiliar histogram which would be the difference between
    // Data and MC histograsm: (Data - MC)
    TH1F *third_aux_ratio = (TH1F*)histo[0]->Clone( ratios_name.str().c_str() );
    third_aux_ratio->Scale(0.);
    third_aux_ratio->Add(histo[0], first_aux_histogram, 1., -1.);

    // Finally, ratio is (Data - MC)/Data_uncertainty
    h_ratio->Divide(third_aux_ratio, h_uncertainty);

    h_ratio->GetXaxis()->SetTitle( x_label[j].c_str() );  // title in x axis
    h_ratio->GetYaxis()->SetTitle( y_title3.c_str() );    // title in y axis
    h_ratio->GetYaxis()->SetTitleOffset(y_title_offset3); // distance of title from y axis
    h_ratio->GetYaxis()->SetTitleSize(y_titleSize3);      // size of title in y axis
    h_ratio->GetYaxis()->SetLabelSize(y_labelSize3);      // size of numbers in y axis
    h_ratio->GetYaxis()->SetRangeUser(y_Min3, y_Max3);    // range of y axis
  }
  else {
    cout << "ERROR: wrong \"ratio_type\" value. Please, check this variable."
    gSystem->Exit(0);
  }

  h_ratio->GetYaxis()->CenterTitle();                     // centralize title of y axis

  h_ratio->GetXaxis()->SetTitleOffset(x_title_offset3);   // distance of title from x axis
  h_ratio->GetXaxis()->SetTitleSize(x_titleSize3);        // size of title in x axis
  h_ratio->GetXaxis()->SetLabelSize(x_labelSize3);        // size of numbers in y axis

  // Fill ratio plots with colors defined by User
  if (ratio_type == 1) {
    h_ratio->SetMarkerStyle(dot_size);
    h_ratio->SetMarkerColor(ratio_color);
  }
  else {
    h_ratio->SetLineColor(ratio_color);
    h_ratio->SetFillColor(ratio_color);
  }

  ratios_name.str("");
  ratios_name.clear();
}


// Function responsible for creating a weight variable based on the differences
// between vertex multiplicity distributions between Data and MC samples.
TTree* addNVtxWeight( vector <TTree*> trees, Int_t j, vector <string> cut ) {
  //, cut="(tag_IsoMu24_eta2p1 || tag_IsoMu24) && tag_combRelIso < 0.15") {

  /////////////////////////////////////////////////////////////
  // Check if Data and MC trees are not empty
  if ( trees[0]->GetEntries() == 0 ) {
    std::cerr << "Data tree is empty" << std::endl;
    return;
  }
  else if ( trees[j]->GetEntries() == 0 ) {
    std::cerr << "MC tree is empty" << std::endl;
    return;
  }
  else {
    // Trees are OK!
  }

  stringstream histogram_name;
  histogram_name << "hData_" << j;

  TH1F *hData = new TH1F(histogram_name.str().c_str(), histogram_name.str().c_str(), 100, -0.5, 99.5);
  histogram_name.str("");
  histogram_name << "hMC_" << j;
  TH1F *hMC   = new TH1F(histogram_name.str().c_str(), histogram_name.str().c_str(), 100, -0.5, 99.5);

  /////////////////////////////////////////////////////////////
  // Filling Data distributions of vertices
  stringstream canvas_weight;
  canvas_weight << "canvas_weight_" << j;
  TCanvas *c1 = new TCanvas(canvas_weight.str().c_str(), canvas_weight.str().c_str());
  c1->Divide(2,1);
  c1->cd(1);
  histogram_name.str("");
  histogram_name << "tag_nVertices >> hData_" << j;
  trees[0]->Draw(histogram_name.str().c_str(), cut[j].c_str());

  c1->cd(2);
  // Filling MC distributions of vertices
  histogram_name.str("");
  histogram_name << "tag_nVertices >> hMC_" << j;
  trees[j]->Draw(histogram_name.str().c_str(), cut[j].c_str());

  /////////////////////////////////////////////////////////////
  // Normalizing Data and MC distributions to have integral "1"
  double h_integral;
  h_integral = (double)hData->Integral();
  // Check if Data histogram is empty
  if ( h_integral > 0 )
    hData->Scale(1.0/(double)h_integral);
  else {
    hData->Scale(0.0);
    cout << endl << "*\t\t Data histogram is empty after baseline selection!";
    cout << endl << "*\t\t Weigths will be set to zero for this MC sample!";
    cout << endl << "*\t tree[" << j << "]\t";
  }

  h_integral = (double)hMC->Integral();
  // Check if MC histogram is empty
  if ( h_integral > 0 )
    hMC->Scale(1.0/(double)h_integral);
  else {
    hMC->Scale(0.0);
    if ( !((double)hData->Integral() > 0) ) {
      cout << endl << "*\t\t MC histogram is empty after baseline selection!";
      cout << endl << "*\t\t Weights will be set to zero for this MC sample!";
      cout << endl << "*\t tree[" << j << "]\t";

    }
    else {
      cout << endl << "*\t\t MC histogram is empty after baseline selection!";
      cout << endl << "*\t\t Some weights will be set to \"1\" for this MC sample!";
      cout << endl << "*\t tree[" << j << "]\t";
    }
  }
  
  /////////////////////////////////////////////////////////////
  // Computing weights..............................................
  // Compute N different weight values based on the N number of bins
  // from Data/MC in the vertex multiplicity distribution.
  std::vector<double> weights(hData->GetNbinsX()+1, 1.0);
  for ( int m = 1, n = weights.size(); m < n; ++m ) {
    double nData = (double)hData->GetBinContent(m);
    double nMC   = (double)  hMC->GetBinContent(m);

    // If nData > 0 && nMC > 0  -->  weight = nData/nMC
    // If nData > 0 && nMC = 0  -->  weight = 1
    // If nData = 0             -->  weight = 0
    if ( nData > 0 ) weights[m-1] = (nMC > 0 ? (double)nData/(double)nMC : 1.0);
    else             weights[m-1] = 0.0;
  }

  /////////////////////////////////////////////////////////////
  // Adding vertex weight variable
  Float_t nVtx, weight;
  trees[j]->SetBranchAddress("tag_nVertices", &nVtx);

  // TTrees will be created inside a TFile.
  // It is not a good idea to to create too
  // many big trees being in memory-resident.
  TFile *fOut = new TFile("TagProbeZ_withNVtxWeights.root", "RECREATE");
  TTree *tOut = (TTree*)trees[j]->CloneTree(0);
  tOut->Branch("vertex_weight", &weight, "vertex_weight/F");

  /////////////////////////////////////////////////////////////
  // Loop over number of entries and fill the new tree with
  // the "vertex_weight" variable.
  int step = trees[j]->GetEntries()/100;
  for (int m = 0, n = trees[j]->GetEntries(); m < n; ++m) {
    trees[j]->GetEntry(m);

    weight = (float)weights[int(nVtx)];
    tOut->Fill();

    if ((m+1) % step == 0) {
      printf(".");
      fflush(stdout);
    }
  }

  /////////////////////////////////////////////////////////////
  // Deleting histograms and canvas
  delete hData;
  delete hMC;
  delete c1;

  stringstream tree_name;
  tree_name << "clone_tree_" << j;
  return (TTree*)tOut->Clone( tree_name.str().c_str() );
}

void Avengers () {
  printf ("\n\n\n");
  printf ("                              .======== \n");
  printf ("                          ..  //     || \n");
  printf ("                    .. ... ..//      ||. ..\n");
  printf ("                 .. .. ..   //  //|  ||. .. ..\n");
  printf ("               .. ..       //  //||  ||    .. ..\n");
  printf ("            .. ..         //  // ||  ||       .. ..\n");
  printf ("          .. ..          //  //  ||  ||         .. ..\n");
  printf ("        .. ..           //  //   ||  ||           .. ..\n");
  printf ("       .. ..           //  //    ||  ||            .. ..\n");
  printf ("      .. ..           //  //     ||  ||             .. ..\n");
  printf ("     .. ..           //  //      ||  ||              .. ..\n");
  printf ("    .. ..           //  //       \\|  ||               .. ..\n");
  printf ("   .. ..           //  //        \\\\  ||                .. ..\n");
  printf ("   .. ..          //  //         |\\\\ ||                .. ..\n");
  printf ("   .. ..         //  //          ||\\\\||                .. ..\n");
  printf ("   .. ..        //  //===========|| \\\\|                .. ..\n");
  printf ("   .. ..       //                    ))                .. ..\n");
  printf ("   .. ..      //  //=============|| //|                .. ..\n");
  printf ("    .. ..    //  //              ||//||               .. ..\n");
  printf ("     .. ..  //  //               |// ||              .. ..\n");
  printf ("      .. ..//  //                //  ||             .. ..\n");
  printf ("       .. //  //                 /=====            .. ..\n");
  printf ("        .//  //                                   .. .\n");
  printf ("        //  //.                                 .. ..\n");
  printf ("       //  //. ..                             .. ..\n");
  printf ("      //  //   .. ..                       .. ..\n");
  printf ("     //  //      .. .. ..             .. .. ..\n");
  printf ("    //  //          .. ... .... .... ... ..\n");
  printf ("   //  //                 ..  ...  ..\n");
  printf ("  ======\n");
  printf ("\n\n\t\t\t\t \"If I cannot save my holiday,\n");
  printf ("\t\t\t\t     will at least revenge it!\"\n\n");
  printf ("\t\t\t\t\t\t      Avengers\n\n");
}

/////////////////////////////////////////////////
/////////                            ////////////
/////////      End of Macro file     ////////////
/////////                            ////////////
/////////////////////////////////////////////////

