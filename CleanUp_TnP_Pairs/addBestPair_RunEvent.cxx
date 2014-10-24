/******************************************************************************

    Version v11: Jun, 14th, 2014
    __________________________
    - Version v11: comes up from version 10
          - Include variable "weight", which takes care of agreement of vertex
            multiplicity between Data and MC.
          - Function "compute_weights" is added to compute such weights.

    - Version v10: comes up from version 9
          - Most part of events happens with a Chi2/NDF approaching to "0",
            instead of approaching to "1". In this version, the best T&P pair,
            of a event, is that one with Chi2/NDF closest to 0.
          - It made the code simpler than previous versions.

    - Version v9: comes up from version 8
          - Clean up of T&P pairs is done only requiring pairs with the Chi2/NDF
            closest to 1. Pairs whose Chi2/NDF > 100 are marked as bad pairs.
          - In case of TT, e.g., Tag becomes Probe and vice-versa, then we have
            two pairs with the same Chi2/NDF. Will keep both of them.
          - Adding variable "transverse mass" between pt(Tag) and MET:
              tag_MT = sqrt( 2*tag_NewTuneP_pt*tag_MET*(1 - cos(tag_NewTuneP_phi - tag_METphi)) )
          - Adding variable "let_choose_pair" to provide (via command line) to User
            with a possibility of either choosing or not a best T&P pair setting up
            the variable "pair_BestPair".
          - Adding variable "let_create_selection_variable" to provide (via command
            line) a possibility of either encompassing or not the User's pre-defined
            selections into variable "pair_passBaselineSelections".
          - Now, this macro has to be run in the way:
              root -b -l -q 'addBestPair_RunEvent.cxx("dataset_definition", "dataset_path", debug, create_new_root, choose_pair, create_selection_variable, "selected_cuts")'
            where:
              - dataset_definition: is a string defining the data set input file;
              - dataset_path: is the path to the input ROOT file;
              - debug: 1 (0) in case of (not) wanting to debug the code;
              - create_new_root: 1 (0) in case of (not) wanting to create a new;
              - choose_pair: 1 (0) in case of (not) wanting to choose a best T&P pair;
              - create_selection_variable: 1 (0) in case of (not) wanting to encompass selections;
              - selected_cuts: selection cuts.

    - Version v8: comes up from version 7
          - change variable "pair_passBaselineSelections" from type Bool_t to Int_t.
          - add a variable called "cosmics" of type Float_t of value "TMath::Pi() - 0.02"
            to remove cosmic-ray muons.

    - Version v7: comes up from version 6
          - adding a new variable which takes care of the track based isolation,
            being defined as
                  probe_tkRelIso = tkIso / NewTuneP_pt
          - making variable "pair_collinearity1" available to remove cosmic-rays
                  alpha = (TMath::Pi() - pair_collinearity1) > 0.02

    - Version v6: comes up from version 4
          - adding a new boolean variable ("pair_passBaselineSelections") to care
            information if Tag&Probe pairs has by passed or not some baseline
            selections.
          - New trees will be created with this new variable. Having several
            selections when computing efficiencies (further step) may not work,
            so adding a new variable which includes all other ones is a possible
            solution.
          - Variable "pair_BestPair" (created in the same way as in version 4)
            is also included into "pair_passBaselineSelections".

    - Version v5: comes up from version 3
          - Correcting the way as the condition of best Chi2 is computed in a loop.
          - When the (first) condition of Tag becoming Probe has been
            completely convered, all other remaining pairs have to be analyzed
            w.r.t. their Chi2, getting ride of pairs with final words from the
            first condition.
          - It was not been doing properly on version 3, where previous pairs
            (without final decision) were not taken into account on the condtion
            of best Chi2.

    - Verson v4: comes up from version 3 and differs because
          - verify which Tag & Probe pair has the best Chi2 if remains more
            than one pair per event after analyzing if Tag becomes Probe.
          - In this case, there is no need to verify if there is a Tag making
            pairs with different Probes.

    __________________________

    This macro loops over pairs of Tag and Probe muons of a ROOT file and
    label such pairs as "good" or "bad". In the end, a new ROOT file is
    produced containing a new variable called "pair_BestPair" carrying
    values "1" and "0" if the pairs were labeled as "good" or "bad",
    respectively.

    Run this macro in this way:
          root -b -l -q 'addBestPair_RunEvent.cxx("dataset_type", "dataset_definition", "dataset_path", "Data_dataset_path", debug, create_new_root)'
    where:
          - dataset_type: type of data set (Data or MC)
          - dataset_definition: is a string defining the data set input file;
          - dataset_path: is the path to the input ROOT file;
          - Data_dataset_path: is the path to the Data input ROOT file;
          - debug: 1 (0) in case of (not) wanting to debug the code;
          - create_new_root: 1 (0) in case of (not) wanting to create a new
            ROOT file with newest variable.

    The basic idea of the code is:
          (1) loop over the TP pairs belonging to the same event;
          (2) look for pairs whose Chi2/NDF < 0 and mark as "bad";
          (3) first, look for pairs where Tag becomes Probe, and vice-versa:
              (3.1) if pT(Tag) < pT(Probe), mark as "good";
              (3.2) if pT(Tag) > pT(Probe), mark as "bad";
              (3.3) if pT(Tag) = pT(Probe), it is the case of Chi2/NDF < 0.
          (4) second, look for the pair with the best Chi2/NDF:
              (4.1) mark as "good" only the pair with the best Chi2/NDF,
              (4.2) that is, the Chi2/NDF closest to "1". It is achieved
                    taking into account the Chi2 probability function:
                      - http://pdg.lbl.gov/2013/reviews/rpp2012-rev-statistics.pdf
                      - http://www.fourmilab.ch/rpkp/experiments/analysis/chiCalc.html

    In the end, it is reported the number of "good" and "bad" pairs, as well as
    the number of "good" pairs passing specific conditions:
          - [Tag <--> Probe     ]: Tag becomes Probe, and vice-versa;
          - [best Chi2/NDF]: the same Tag makes a pair with different Probes.
          - None of the above.

    Taking the selection of "good" pairs, a new set of specific selections defined by
    the USER is applied, and a set of basic informations will be reported later on.
    Such informations are the number of Tag and Probe pairs where:
          - pT(Tag) < pT(Probe);
          - pT(Tag) > pT(Probe);
          - pT(Tag) = pT(Probe);
          - happens any other strange situation;
          - specific selections fail.

    It is also created a "txt" file summarizing the informations above in a
    Python format.

    It is possible to run in debug mode. In this case, only 100 pairs are
    analyzed and NO new file is produced.

 ******************************************************************************/

#include "TTree.h"
#include "TFile.h"
#include "TMath.h"
#include "TSystem.h"
#include "TStopwatch.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <typeinfo>

using namespace std;
#define NUMBER_OF_ARRAYS 100 // This number may be lower but need caution when changing it

// =================================================================================================
void addBestPair_RunEvent () {
  cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
  cout << "\t ERROR: the correct way to use this macro is" << endl;
  cout << "\t\t root -b -l -q \'addBestPair_RunEvent.cxx(\"dataset_type\", \"dataset_definition\", \
\"dataset_path\", \"Data_dataset_path\", debug, create_new_root, choose_pair, create_selection_variable, \
\"selected_cuts\")\'" << endl;
  cout << "\t where" << endl;
  cout << "\t - dataset_type: is type of data set (Data or MC)" << endl;
  cout << "\t - dataset_definition: is a string defining the data set of the input file;" << endl;
  cout << "\t - dataset_path: is a string showing the path to the input ROOT file;" << endl;
  cout << "\t - Data_dataset_path: is a string showing the path to the Data input ROOT file" << endl;
  cout << "\t - debug: 1 (0) in case of (not) wanting to debug the code;" << endl;
  cout << "\t - create_new_root: 1 (0) in case of (not) wanting to create a new ROOT file with \
the newest variables." << endl;
  cout << "\t - choose_pair: 1 (0) in case of (not) wanting to choose the best pair of T&P muons." << endl;
  cout << "\t - create_selection_variable: 1 (0) in case of (not) wanting to create a variable \
encompassing User's pre-defined selections;" << endl;
  cout << "\t - selected_cuts: selection cuts.\n\n" << endl;

  gSystem->Exit(0);
}

// =================================================================================================
vector <double> compute_weights ( TTree*, TTree*, string, string );


// =================================================================================================
//
//                                       Main function starts here.
//
// =================================================================================================
void addBestPair_RunEvent ( string dataset_type == "", string dataset_definition == "",
			    const char* file_path == "", const char* data_file_path == "",
			    Int_t let_debug == 0, Int_t let_create_new_root_file == 0,
			    Int_t let_choose_pair == 1, Int_t let_create_selection_variable == 1,
			    string selected_cuts == "" ) {

  // This macro can be used in CONDOR nodes, and it is important to not modify this line
  string output_file_name = "output_nEvents_after_selection.txt";  // MUST NOT CHANGE THIS LINE
  // std::ios::out --> File open for writing, supporting output operations.
  // std::ios::app --> All output operations happen at the end of the file, appending to its existing contents.
  ofstream output_file (output_file_name.c_str(), std::ios::out);

  // In case of wanting to run a debug mode, and/or
  // to create a new ROOT file with the newest variable:
  //    * 0 --> No
  //    * 1 --> Yes
  Int_t debug = let_debug;
  Int_t create_new_root_file = let_create_new_root_file;
  Int_t choose_pair = let_choose_pair;
  Int_t create_selection_variable = let_create_selection_variable;

  if ( (typeid(debug)                     != typeid(Int_t)) ||
       (typeid(create_new_root_file)      != typeid(Int_t)) ||
       (typeid(choose_pair)               != typeid(Int_t)) ||
       (typeid(create_selection_variable) != typeid(Int_t)) ||
       ((debug != 0)                      && (debug != 1))                ||
       ((create_new_root_file != 0)       && (create_new_root_file != 1)) ||
       ((choose_pair != 0)                && (choose_pair != 1))          ||
       ((create_selection_variable != 0)  && (create_selection_variable != 1))
       ) {
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "\t ERROR: wrong value passed to input variables.\n";
    addBestPair_RunEvent ();
  }
  if ( (typeid(dataset_type) != typeid(string)) || (dataset_type == "") ) {
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "\t ERROR: in macro addBestPair_RunEvent ():" << endl;
    cout << "\t Maybe has passed wrong input type (or even no input) into variable \"dataset_type\"." << endl;
    addBestPair_RunEvent ();
    gSystem->Exit(0);
  }
  if ( (typeid(file_path) != typeid(char*)) || (file_path == "") ) {
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "\t ERROR: in macro addBestPair_RunEvent ():" << endl;
    cout << "\t Maybe has passed wrong input type (or even no input) into variable \"file_path\"." << endl;
    addBestPair_RunEvent ();
    gSystem->Exit(0);
  }
  if ( (typeid(data_file_path) != typeid(char*)) || (data_file_path == "") ) {
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "\t ERROR: in macro addBestPair_RunEvent ():" << endl;
    cout << "\t Maybe has passed wrong input type (or even no input) into variable \"data_file_path\"." << endl;
    addBestPair_RunEvent ();
    gSystem->Exit(0);
  }
  if ( (typeid(dataset_definition) != typeid(string)) || (dataset_definition == "") ) {
  cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "\t ERROR: in macro addBestPair_RunEvent ():" << endl;
    cout << "\t Maybe has passed wrong input type (or even no input) into variable \"dataset_definition\"." << endl;
    addBestPair_RunEvent ();
    gSystem->Exit(0);
  }
  if ( (typeid(selected_cuts) != typeid(string)) || (selected_cuts == "") ) {
  cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "\t ERROR: in macro addBestPair_RunEvent ():" << endl;
    cout << "\t Maybe has passed wrong input type (or even no input) into variable \"dataset_definition\"." << endl;
    addBestPair_RunEvent ();
    gSystem->Exit(0);
  }

  // ================================================
  // ===== Open input file
  TFile *fIn = TFile::Open( file_path );
  if( !fIn ){
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "\t ERROR: in macro addBestPair_RunEvent ():" << endl;
    cout << "\t Input ROOT file \"" << file_path << "\" not found. Check the dataset path." << endl;
    addBestPair_RunEvent ();
    gSystem->Exit(0);
  }
  TTree *tIn  = (TTree *)fIn->Get("tpTree/fitter_tree");

  // ================================================
  // ===== Write here the variables you want to use
  UInt_t run, event, lumi;
  tIn->SetBranchAddress("run",   &run);
  tIn->SetBranchAddress("lumi",  &lumi);
  tIn->SetBranchAddress("event", &event);


  Int_t   tag_IsoMu24;
  Float_t NewTuneP_pt,  tag_NewTuneP_pt,             pair_DimuonVtxFitNormQui2;  
  Float_t NewTuneP_eta, tag_NewTuneP_eta,            pair_newTuneP_mass;
  Float_t NewTuneP_phi, tag_NewTuneP_phi;
  Float_t charge,       tag_charge,                  pair_dz;
  Float_t tkIso,        tag_combRelIsoPF04dBeta,     tag_innertrackPtRelError;
  Float_t tag_MET10,    tag_METphi10,                pair_collinearity1;
  Float_t tag_nVertices;
  tIn->SetBranchAddress("NewTuneP_pt",               &NewTuneP_pt);
  tIn->SetBranchAddress("tag_NewTuneP_pt",           &tag_NewTuneP_pt);
  tIn->SetBranchAddress("NewTuneP_eta",              &NewTuneP_eta);
  tIn->SetBranchAddress("tag_NewTuneP_eta",          &tag_NewTuneP_eta);
  tIn->SetBranchAddress("NewTuneP_phi",              &NewTuneP_phi);
  tIn->SetBranchAddress("tag_NewTuneP_phi",          &tag_NewTuneP_phi);
  tIn->SetBranchAddress("charge",                    &charge);
  tIn->SetBranchAddress("tag_charge",                &tag_charge);
  tIn->SetBranchAddress("pair_dz",                   &pair_dz);
  tIn->SetBranchAddress("pair_newTuneP_mass",        &pair_newTuneP_mass);
  tIn->SetBranchAddress("pair_DimuonVtxFitNormQui2", &pair_DimuonVtxFitNormQui2);
  tIn->SetBranchAddress("tag_IsoMu24",               &tag_IsoMu24);
  tIn->SetBranchAddress("tag_combRelIsoPF04dBeta",   &tag_combRelIsoPF04dBeta);
  tIn->SetBranchAddress("tag_innertrackPtRelError",  &tag_innertrackPtRelError);
  tIn->SetBranchAddress("tkIso",                     &tkIso);
  tIn->SetBranchAddress("pair_collinearity1",        &pair_collinearity1);
  tIn->SetBranchAddress("tag_MET10",                 &tag_MET10);
  tIn->SetBranchAddress("tag_METphi10",              &tag_METphi10);
  tIn->SetBranchAddress("tag_nVertices",             &tag_nVertices);


  // ===============================================
  // ===== Write here the variables you want to save
  //  if (create_new_root_file) {
  string new_file_name = "tnpZ_addBestPair.root";
  TFile *fOut  = new TFile( "auxiliar_tnpZ_addBestPair.root", "RECREATE" );
  fOut->mkdir("tpTree")->cd();
  TTree *tOut  = tIn->CloneTree(0);  
  Int_t   bestPair;
  Int_t   pair_passBaselineSelections;
  Float_t probe_tkRelIso;
  Float_t tag_MT;

  if (create_new_root_file) {
    tOut->Branch("probe_tkRelIso",                &probe_tkRelIso,              "probe_tkRelIso/F");
    tOut->Branch("tag_MT",                        &tag_MT,                      "tag_MT/F");
    if ( choose_pair )
      tOut->Branch("pair_BestPair",               &bestPair,                    "pair_BestPair/I");
    if ( create_selection_variable )
      tOut->Branch("pair_passBaselineSelections", &pair_passBaselineSelections, "pair_passBaselineSelections/I");
  }


  if (!create_new_root_file)
    tOut->SetAutoFlush();

  // =====================================
  // ===== Compute your new variables here
  int step = tIn->GetEntries()/1000;
  double evDenom = 100.0/double(tIn->GetEntries());
  TStopwatch timer; timer.Start();
  UInt_t n = 0;
  if(debug)  n = 100;
  if(!debug) n = tIn->GetEntries();

  // ===============================================
  // ===== Variables used to provid a report
  // ===== in the end of processing
  UInt_t count_TagPt_greater_than_ProbePt = 0, count_ProbePt_greater_than_TagPt = 0;
  UInt_t count_TagPt_equal_to_ProbePt     = 0, count_anything_strange           = 0, failing_selections = 0;

  // ===============================================
  // ===== Variables used to:
  Int_t   counter                = 0;       // account for the number of pairs in the same event
  UInt_t  event_i                = 0;       // copy value of event of a given pair
  UInt_t  current_pair           = 0;       // keep value of the current pair in the loop
  UInt_t  pairs_passed           = 0;       // account number of pairs marked as "good"
  UInt_t  pairs_failed           = 0;       // account number of pairs marked as "bad"
  Float_t min_difference         = 0.00001; // minimum difference between pT of muons (it's up to the USER)

  Int_t   bestCandidate          = 0;  // inform the best pair candidate concerning the Chi2 condition
  Int_t   double_pair            = 0;  // other best pair candidate (if is a replic of the first one when "T <-> P"

  Float_t cosmics                = TMath::Pi() - 0.02; // remove cosmic-ray muons with 3D-angle > cosmics

  // ===============================================
  // ===== Arrays to keep information about
  // - done[]:            pairs which have been already analyzed and should not be touched
  // - tag_pt[]:          pT of Tag
  // - probe_pt[]:        pT of Probe
  // - NormChi2[]:        Chi2/NDF
  // - pair_pass[]:       pairs passing or not
  Int_t   pair_pass[NUMBER_OF_ARRAYS], done[NUMBER_OF_ARRAYS];
  Float_t tag_pt[NUMBER_OF_ARRAYS],    probe_pt[NUMBER_OF_ARRAYS];
  Float_t NormChi2[NUMBER_OF_ARRAYS];

  cout << "======================================================================" << endl;
  cout << "\n\t Debug mode?      : " << ((debug) ? "Yes" : "No")
       << "\n\t Create new file? : " << ((create_new_root_file) ? "Yes" : "No")
       << "\n\t Choose pair?     : " << ((let_choose_pair) ? "Yes" : "No")
       << "\n\t Select events?   : " << ((let_create_selection_variable) ? "Yes" : "No")
       << "\n\t Number of events : " << n
       << "\n\t Dataset type     : " << dataset_definition
       << "\n\t Dataset path     : " << file_path << "\n" << endl;
  cout << "======================================================================" << endl;


  // =================================================================================================
  // ===== If wanting only to create new ROOT file with new variables, without choosing pairs
  if ( (create_new_root_file) && (!choose_pair) && (!create_selection_variable) ) {
    for ( UInt_t i = 0; i < n; ++i ) {
      tIn->GetEntry(i);

      // Fill variable MT = sqrt{ 2*pT(tag)*ETmiss*(1 - cos(Dphi)) }
      // where   cos(Dphi) = cos(DeltaPhi( tag_NewTuneP_phi, NewTuneP_phi ))
      tag_MT = sqrt( 2*tag_NewTuneP_pt*tag_MET10*(1 - cos(tag_NewTuneP_phi - tag_METphi10)) );
      
      // Evaluate variable of track based isolation for the current Tag & Probe pair
      probe_tkRelIso = (Float_t)tkIso / (Float_t)NewTuneP_pt;

      // Fill new ROOT file.
      // Even in case of not wanting to create a new ROOT file, "Fill()" triggers
      // the "SetAutoFlush()" function, speeding up the processing.
      tOut->Fill();
    }
  }

  // =================================================================================================
  // In case of only wanting to create a new variable encompassing selected events, without
  // the workflow to clean up events.
  if ((!choose_pair) && (create_selection_variable)) {
    // In case of wanting to verify the number of pairs also passing specific selections, modify
    // the condition bellow based on the available list of variables previously set in the input tree.
    //
    //      WARNING: this macro may be used in CONDOR nodes, being important to keep the commented
    //               line bellow as it is. Only change the "if ()" condition.
    //
    for ( UInt_t i = 0; i < n; ++i ) {
      // Before filling the new ROOT file, it is mandatory to get entry from the pair being
      // analyzed. Start "j" from the first pair ("current_pair") of the current event.
      tIn->GetEntry(i);
      
      if (tag_charge*charge < 0) { //===>>> Apply general selections here
	if      (    (NewTuneP_pt > tag_NewTuneP_pt)                  ) ++count_ProbePt_greater_than_TagPt;
	else if (    (NewTuneP_pt < tag_NewTuneP_pt)                  ) ++count_TagPt_greater_than_ProbePt;
	else if (fabs(NewTuneP_pt - tag_NewTuneP_pt) < min_difference ) ++count_TagPt_equal_to_ProbePt;
	else {/*******************************************************/ ++count_anything_strange;}
	pair_passBaselineSelections = 1;
      }
      else {
	++failing_selections;
	pair_passBaselineSelections = 0;
      }

      // Fill variable MT = sqrt{ 2*pT(tag)*ETmiss*(1 - cos(Dphi)) }
      // where   cos(Dphi) = cos(DeltaPhi( tag_NewTuneP_phi, NewTuneP_phi ))
      tag_MT = sqrt( 2*tag_NewTuneP_pt*tag_MET10*(1 - cos(tag_NewTuneP_phi - tag_METphi10)) );
      
      // Evaluate variable of track based isolation for the current Tag & Probe pair
      probe_tkRelIso = (Float_t)tkIso / (Float_t)NewTuneP_pt;

      // Fill new ROOT file.
      // Even in case of not wanting to create a new ROOT file, "Fill()" triggers
      // the "SetAutoFlush()" function, speeding up the processing.
      tOut->Fill();

    } // End of for ( Int_t i = 0; i < n; ++i )
  } // End of if ((choose_pair == 0) && (create_selection_variable == 1))

  // =================================================================================================
  // In case of wanting to run over the workflow and mark pairs as "good" or "bad" depending on Chi2/NDF
  if ( choose_pair ) {
    // Start looping over pairs of Tag and Probe muons
    for ( UInt_t i = 0; i < n; ++i ) {
      if (debug)
	cout << "======================================================================" << endl;

      tIn->GetEntry(i);     // Take an entry

      current_pair  = i;     // keep value of the current pair in the loop
      counter       = 0;     // re-inialize the counter of pairs of the same event
      event_i       = event; // copy value of event of a given pair "i"
      bestCandidate = 0;     // keep correspondent index of the best pair concerning its Chi2/NDF
      double_pair   = 0;     // label second pair, in case of a tag becoming probe and a probe becoming tag

      // Inialize arrays
      Int_t number_of_arrays = NUMBER_OF_ARRAYS;
      for ( Int_t j  = 0; j < number_of_arrays; ++j )   pair_pass[j] = 0;

      // =================================================================================================
      // Loop over pairs of Tag and Probe muons, starting the current pair,
      // and find pairs belonging to the same event.
      for ( Int_t j = i; j < n; ++j ) {
	tIn->GetEntry(j);
	// Have found a different event number? Stop the loop!
	if ( event_i != event ) {
	  i = j-1; // Making sure next event loop will start from the right point
	  j = n;   // Same as using "break;"
	}
	else {
	  // Make a copy of variables of the current pair
	  probe_pt[counter]        = NewTuneP_pt;
	  tag_pt[counter]          = tag_NewTuneP_pt;
	  NormChi2[counter]        = pair_DimuonVtxFitNormQui2;
	  // Account for the number of pairs in the current event
	  ++counter;
	}
      } // End of for ( UInt_t j = i; j < n; ++j )


      // ==============================================================================================
      if ( counter == 1 ) {
	// If Chi2/NDF < 0, it is a "bad" pair.
	if ( (NormChi2[0] < 0) || (fabs(tag_pt[0] - probe_pt[0]) < min_difference) || (NormChi2[0] > 100) )
	  pair_pass[0] = 0;
	else
	  pair_pass[0] = 1;
      }
      else { // (1)
	// =================================================================================================
	// Loop over T&P pairs of the same event and check which pair has the
	// lowest Chi2/NDF value.
	for ( Int_t j = 0; j < counter; j++ ) {
	  if ( j == 0 ) {
	    // Take it as the best pair, for while
	    pair_pass[j]  = 1;
	    bestCandidate = j;
	  }
	  else { // (2)
	    // In case of current pair is the same one of a previous pair already
	    // analyzed (and marked as the best one) both pairs should be marked as the bests.
	    if ( (fabs(tag_pt[j] - probe_pt[bestCandidate]) <  min_difference) &&
		 (fabs(tag_pt[bestCandidate] - probe_pt[j]) <  min_difference) ) {
	      pair_pass[j] = 1;
	      double_pair  = j; // double_pair > 0 means we found a duplicated pair with Tag becoming Probe
	    }
	    else { // (3)
	      // Compare with previous best pair
	      if ( NormChi2[j] < NormChi2[bestCandidate] ) {
		// In case when the best previous candidate was a double pair
		if ( double_pair > 0 ) {
		  pair_pass[j]             = 1; // First, inform that now this is the best pair so far.
		  pair_pass[double_pair]   = 0; // Second, duplicated previous best candidate becomes a bad pair.
		  pair_pass[bestCandidate] = 0; // Third, previous best candidate becomes a bad pair.
		  bestCandidate            = j; // Finally, update current pair as the best candidate.
		}
		// In case when the best previous candidate was not a double pair
		else {
		  pair_pass[j]             = 1; // First, inform that now this is the best pair so far.
		  pair_pass[bestCandidate] = 0; // Second, make the older one becomes a bad pair.
		  bestCandidate            = j; // Finally, update current pair as the best candidate.
		}
	      } // End of if ( NormChi2[j] < NormChi2[bestCandidate] )
	      else {
		// In case current pair is not the best one
		pair_pass[j]  = 0;
	      }
	    } // End of (3)
	  } // End of (2)
	} // End of ( Int_t j = 0; j < counter; j++ )
      } // End of (1)
      // =================================================================================================
      // Loop over the pair of Tag & Probe muons belonging to the same event and fill the tree
      // with the new variable ("bestPair") concerning information from the "pair_pass[]" array
      // provided above.
      for ( Int_t j = 0; j < counter; ++j ) {
	// Before filling the new ROOT file, it is mandatory to get entry from the pair being
	// analyzed. Start "j" from the first pair ("current_pair") of the current event.
	tIn->GetEntry(current_pair + j);
	
	if ( pair_pass[j] == 0 ) { // If pair has been marked as "bad"
	  bestPair = 0;
	  ++pairs_failed;
	  pair_passBaselineSelections = 0;
	}
	else { // (9)
	  // If pair has been marked as "good"
	  bestPair = 1;
	  ++pairs_passed;
	  
	  if ( create_selection_variable ) {
	    // In case of wanting to verify the number of pairs also passing specific selections, modify
	    // the condition bellow based on the available list of variables previously set in the input tree.
	    //
	    //      WARNING: this macro may be used in CONDOR nodes, being important to keep the commented
	    //               line bellow as it is. Only change the "if ()" condition.
	    //
	    if (tag_charge*charge < 0) { //===>>> Apply general selections here
	      if      (    (NewTuneP_pt > tag_NewTuneP_pt)                  ) ++count_ProbePt_greater_than_TagPt;
	      else if (    (NewTuneP_pt < tag_NewTuneP_pt)                  ) ++count_TagPt_greater_than_ProbePt;
	      else if (fabs(NewTuneP_pt - tag_NewTuneP_pt) < min_difference ) ++count_TagPt_equal_to_ProbePt;
	      else {/*******************************************************/ ++count_anything_strange;}
	      pair_passBaselineSelections = 1;
	    }
	    else {
	      ++failing_selections;
	      pair_passBaselineSelections = 0;
	    }
	  } // End of if ( create_selection_variable )
	} // End of (9)
	  
	if (debug) {
	  cout << "i: "                << current_pair + j   << ",\t event: "      << event
	       << ",\t pair_pass["     << j                  << "]: "              << pair_pass[j]
	       << ",\t bestPair: "     << bestPair           << ",\t tag_pt: "     << tag_NewTuneP_pt
	       << ",\t probe_pt: "     << NewTuneP_pt	     << ",\t tag_charge: " << tag_charge
	       << ",\t probe_charge: " << charge             << ",\t dz: "         << pair_dz
	       << ",\t mass: "         << pair_newTuneP_mass << ",\t Chi2: "       << pair_DimuonVtxFitNormQui2
	       << endl;
	}
	
	// Fill variable MT = sqrt{ 2*pT(tag)*ETmiss*(1 - cos(Dphi)) }
	// where   cos(Dphi) = cos(DeltaPhi( tag_NewTuneP_phi, NewTuneP_phi ))
	tag_MT = sqrt( 2*tag_NewTuneP_pt*tag_MET10*(1 - cos(tag_NewTuneP_phi - tag_METphi10)) );
	
	// Evaluate variable of track based isolation for the current Tag & Probe pair
	probe_tkRelIso = (Float_t)tkIso / (Float_t)NewTuneP_pt;

	// Fill new ROOT file.
	// Even in case of not wanting to create a new ROOT file, "Fill()" triggers
	// the "SetAutoFlush()" function, speeding up the processing.
	tOut->Fill();
	
	if ( (current_pair + j + 1) == n ) i = n;        // Avoid repeating the last pair
      } // End of for ( Int_t j = 0; j < counter; ++j )
      
      if( !debug ) {
	if ((i+1) % step == 0) { 
	  double totalTime = timer.RealTime()/60.;    timer.Continue();
	  double fraction  = double(i+1)/double(n+1), remaining = totalTime*(1-fraction)/fraction;
	  printf ("Done %9d/%9d   %5.1f%%   (elapsed %5.1f min, remaining %5.1f min)\n",
		  i, n, i*evDenom, totalTime, remaining); 
	  fflush (stdout);
	}
      }
    } // End of for (UInt_t i = 0; i < n; ++i)
  } // if ( choose_pair )

  tOut->AutoSave(); // According to root tutorial this is the right thing to do
  //  fOut->Close();

  printf ("==============================================================================================================\n");
  printf ("\t Computing vertex multiplicity weight...\n\n");



  // =================================================================================================
  // If MC, compute vertex multiplicity weight
  if ( create_new_root_file && (dataset_type == "MC") ) {
    // ================================================
    // ===== Open Data input file
    TFile *fDataIn = TFile::Open( data_file_path );
    if( !fDataIn ){
      cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
      cout << "\t ERROR: in macro addBestPair_RunEvent ():" << endl;
      cout << "\t Input ROOT file \"" << data_file_path << "\" not found. Check the dataset path." << endl;
      addBestPair_RunEvent ();
      gSystem->Exit(0);
    }
    TTree *tDataIn  = (TTree *)fDataIn->Get("tpTree/fitter_tree");

    TFile *fOutFinal  = new TFile( new_file_name.c_str(), "RECREATE" );
    fOutFinal->mkdir("tpTree")->cd();
    TTree *tOutFinal  = tOut->CloneTree(0);

    Float_t weight;
    tOutFinal->Branch("weight", &weight, "weight/F"); 

    // =================================================================================================
    // ==== Compute vertex multiplicity weight for MC samples
    vector <double> weights(101, 1.0);
    string MCcuts;
    stringstream joinCuts;
    if ( choose_pair ) {
      joinCuts << selected_cuts << " && (pair_BestPair == 1)";
      MCcuts = joinCuts.str();
      weights = compute_weights(tOut, tDataIn, MCcuts, selected_cuts);
    }
    else
      weights = compute_weights(tOut, tDataIn, selected_cuts, selected_cuts);

    Float_t tag_Data_nVertices;
    tOut->SetBranchAddress("tag_nVertices", &tag_Data_nVertices);

    for ( UInt_t i = 0; i < n; ++i ) {
      tOut->GetEntry(i);
      // Compute vertex multiplicity weight for MC samples
      weight = (float)weights[int(tag_Data_nVertices)];
      tOutFinal->Fill();
    }
    tOutFinal->AutoSave(); // According to root tutorial this is the right thing to do
    fOutFinal->Close();
  }
  else if ( create_new_root_file && (dataset_type != "MC") ) {
    fOut->Close();
    stringstream rename_file;
    rename_file << ".!mv auxiliar_" << new_file_name << " " << new_file_name; // Using bash commands via ROOT
    gROOT->ProcessLine( rename_file.str().c_str() );
  }
  else {
    fOut->Close();
    stringstream deleting_file;
    deleting_file << ".!rm -f auxiliar_" << new_file_name; // Using bash commands via ROOT
    gROOT->ProcessLine( deleting_file.str().c_str() );
  }

  printf ("==============================================================================================================\n");
  printf ("\t [[ Data Set: %s ]]\n\n",                       dataset_definition.c_str());
  printf ("\t -------------------------------------------------\n");
  printf ("\t Total number of pairs analyzed        = %9d\n", n);
  printf ("\t -------------------------------------------------\n");
  printf ("\t Number of pairs without specific selections:\n");
  printf ("\t   - good pairs                        = %9d\n", pairs_passed);
  printf ("\t   - bad pairs                         = %9d\n", pairs_failed);
  printf ("\t -------------------------------------------------\n");
  printf ("\t Number of \"good\" pairs after specific selections:\n");
  printf ("\t   - pT(Tag) < pT(Probe)               = %9d\n", count_ProbePt_greater_than_TagPt);
  printf ("\t   - pT(Tag) > pT(Probe)               = %9d\n", count_TagPt_greater_than_ProbePt);
  printf ("\t   - pT(Tag) = pT(Probe)               = %9d\n", count_TagPt_equal_to_ProbePt);
  printf ("\t   - anything strange                  = %9d\n", count_anything_strange);
  printf ("\t   - failing selections                = %9d\n", failing_selections);
  printf ("==============================================================================================================\n");

  // A "txt" file is created summarizing the informations above in a Python format.
  if ( output_file.is_open() ) {
    output_file << "\t(" << pairs_passed            << ", ";
    output_file << pairs_failed                     << ", ";
    output_file << count_ProbePt_greater_than_TagPt << ", ";
    output_file << count_TagPt_greater_than_ProbePt << ", ";
    output_file << count_TagPt_equal_to_ProbePt     << ", ";
    output_file << count_anything_strange           << ", ";
    output_file << failing_selections               << ", \"";
    output_file << dataset_definition               << "\"),\n";
    output_file.close();
  }
  else{
    cout << "\t ERROR: Unable to open output file.";
    gSystem->Exit(0);
  }

} // End of main function


//=========================================================================================
//=========================================================================================
vector <double> compute_weights ( TTree* tIn, TTree* tData, string cutsMC, string cutsData ) {

  /////////////////////////////////////////////////////////////
  // Check if Data and MC trees are not empty
  if ( tData->GetEntries() == 0 ) {
    std::cerr << "Data tree is empty" << std::endl;
    return;
  }
  else if ( tIn->GetEntries() == 0 ) {
    std::cerr << "MC tree is empty" << std::endl;
    return;
  }
  else {
    // Trees are OK!
  }

  stringstream histogram_name;
  histogram_name << "hData";

  TH1F *hData = new TH1F(histogram_name.str().c_str(), histogram_name.str().c_str(), 100, -0.5, 99.5);
  histogram_name.str("");
  histogram_name << "hMC";
  TH1F *hMC   = new TH1F(histogram_name.str().c_str(), histogram_name.str().c_str(), 100, -0.5, 99.5);

  /////////////////////////////////////////////////////////////
  // Filling Data distributions of vertices
  stringstream canvas_weight;
  canvas_weight << "canvas_weight";
  TCanvas *c1 = new TCanvas(canvas_weight.str().c_str(), canvas_weight.str().c_str());
  c1->Divide(2,1);
  c1->cd(1);
  histogram_name.str("");
  histogram_name << "tag_nVertices >> hData";
  tData->Draw(histogram_name.str().c_str(), cutsData.c_str());

  c1->cd(2);
  // Filling MC distributions of vertices
  histogram_name.str("");
  histogram_name << "tag_nVertices >> hMC";
  tIn->Draw(histogram_name.str().c_str(), cutsMC.c_str());

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
    }
    else {
      cout << endl << "*\t\t MC histogram is empty after baseline selection!";
      cout << endl << "*\t\t Some weights will be set to \"1\" for this MC sample!";
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

  return weights;
}
