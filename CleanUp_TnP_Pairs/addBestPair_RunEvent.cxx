/******************************************************************************

    Version v9: Mar, 1st, 2014
    __________________________

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
              root -b -l -q 'addBestPair_RunEvent.cxx("dataset_definition", "dataset_path", debug, create_new_root, choose_pair, create_selection_variable)'
            where:
              - dataset_definition: is a string defining the data set input file;
              - dataset_path: is the path to the input ROOT file;
              - debug: 1 (0) in case of (not) wanting to debug the code;
              - create_new_root: 1 (0) in case of (not) wanting to create a new;
              - choose_pair: 1 (0) in case of (not) wanting to choose a best T&P pair;
              - create_selection_variable: 1 (0) in case of (not) wanting to encompass selections.

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
          root -b -l -q 'addBestPair_RunEvent.cxx("dataset_definition", "dataset_path", debug, create_new_root)'
    where:
          - dataset_definition: is a string defining the data set input file;
          - dataset_path: is the path to the input ROOT file;
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
#define NUMBER_OF_ARRAYS 100 // This number may be lower but need caution when decreasing it

// =================================================================================================
void addBestPair_RunEvent () {
  cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
  cout << "\t ERROR: the correct way to use this macro is" << endl;
  cout << "\t\t root -b -l -q \'addBestPair_RunEvent.cxx(\"dataset_definition\", \"dataset_path\", \
debug, create_new_root, choose_pair, create_selection_variable)\'" << endl;
  cout << "\t where" << endl;
  cout << "\t - dataset_definition: is a string defining the data set of the input file;" << endl;
  cout << "\t - dataset_path: is a string showing the path to the input ROOT file;" << endl;
  cout << "\t - debug: 1 (0) in case of (not) wanting to debug the code;" << endl;
  cout << "\t - create_new_root: 1 (0) in case of (not) wanting to create a new ROOT file with \
the newest variables." << endl;
  cout << "\t - choose_pair: 1 (0) in case of (not) wanting to choose the best pair of T&P muons." << endl;
  cout << "\t - create_selection_variable: 1 (0) in case of (not) wanting to create a variable \
encompassing User's pre-defined selections.\n\n" << endl;
  gSystem->Exit(0);
}

// =================================================================================================
//
//                                       Main function starts here.
//
// =================================================================================================
void addBestPair_RunEvent ( string dataset_definition == "", const char* file_path == "",
			       Int_t let_debug == 0, Int_t let_create_new_root_file == 0,
			       Int_t let_choose_pair == 1, Int_t let_create_selection_variable == 1 ) {

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
  if ( (typeid(file_path) != typeid(char*)) || (file_path == "") ) {
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "\t ERROR: in macro addBestPair_RunEvent ():" << endl;
    cout << "\t Maybe has passed wrong input type (or even no input) into variable \"file_path\"." << endl;
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
  tIn->SetBranchAddress("run",        &run);
  tIn->SetBranchAddress("event",      &event);
  tIn->SetBranchAddress("lumi",       &lumi);

  Int_t   tag_IsoMu24;
  Float_t NewTuneP_pt,  tag_NewTuneP_pt,         pair_DimuonVtxFitNormQui2;  
  Float_t NewTuneP_eta, tag_NewTuneP_eta,        pair_newTuneP_mass;
  Float_t NewTuneP_phi, tag_NewTuneP_phi;
  Float_t charge,       tag_charge,              pair_dz;
  Float_t tkIso,        tag_combRelIsoPF04dBeta, tag_innertrackPtRelError;
  Float_t tag_MET,      tag_METphi,              pair_collinearity1;
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
  tIn->SetBranchAddress("tag_MET",                   &tag_MET);
  tIn->SetBranchAddress("tag_METphi",                &tag_METphi);

  // ===============================================
  // ===== Write here the variables you want to save
  //  if (create_new_root_file) {
  string new_file_name = "tnpZ_addBestPair.root";
  TFile *fOut  = new TFile( new_file_name.c_str(), "RECREATE" );
  fOut->mkdir("tpTree")->cd();
  TTree *tOut  = tIn->CloneTree(0);  
  Int_t bestPair;
  Int_t pair_passBaselineSelections;
  Float_t probe_tkRelIso;
  Float_t tag_MT;

  if (create_new_root_file) {
    tOut->Branch("pair_BestPair",               &bestPair,                    "pair_BestPair/I"); 
    tOut->Branch("probe_tkRelIso",              &probe_tkRelIso,              "probe_tkRelIso/F");
    tOut->Branch("tag_MT",                          &tag_MT,                          "tag_MT/F"); 
    if ( create_selection_variable )
      tOut->Branch("pair_passBaselineSelections", &pair_passBaselineSelections, "pair_passBaselineSelections/I");
  }
  else {
    // Nothing to do!
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
  Float_t Chi2Probability        = 0.; // decide the best candidade concernig chi2 probability
  Float_t min_Chi2Probability    = 0.; // receive value of best chi2 probability
  Float_t bestChi2Probability    = TMath::Prob(1, 1); // carry value of a chi2 probability with NDF = 1

  UInt_t pairs_NoCondition       = 0;  // account for the number of pairs which do not pass by any condition
  UInt_t pairs_Chi2Condition     = 0;  // account for the number of pairs passing by "T <-> P" condition
  UInt_t pairs_ExchangeCondition = 0;  // account for the number of pairs passing by Chi2 condition

  Float_t cosmics                = TMath::Pi() - 0.02; // remove cosmic-ray muons with 3D-angle > cosmics

  // ===============================================
  // ===== Arrays to keep information about
  // - done[]:            pairs which have been already analyzed and should not be touched
  // - tag_pt[]:          pT of Tag
  // - probe_pt[]:        pT of Probe
  // - NormChi2[]:        Chi2/NDF
  // - pair_pass[]:       pairs passing or not
  // - chi2Probability[]: chi2 probability
  Int_t   pair_pass[NUMBER_OF_ARRAYS], done[NUMBER_OF_ARRAYS];
  Float_t tag_pt[NUMBER_OF_ARRAYS],    probe_pt[NUMBER_OF_ARRAYS];
  Float_t NormChi2[NUMBER_OF_ARRAYS],  chi2Probability[NUMBER_OF_ARRAYS];

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
  // In case of only wanting to create a new variable encompassing selected events, without
  // the workflow to clean up events.
  if ((!choose_pair) && (create_selection_variable)) {
    // In case of wanting to verify the number of pairs also passing specific selections, modify
    // the condition bellow based on the available list of variables previously set in the input tree.
    //
    //      WARNING: this macro may be used in CONDOR nodes, being important to keep the commented
    //               line bellow as it is. Only change the "if ()" condition.
    //
    for ( Int_t i = 0; i < n; ++i ) {
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
      tag_MT = sqrt( 2*tag_NewTuneP_pt*tag_MET*(1 - cos(tag_NewTuneP_phi - tag_METphi)) );
      
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

      // Inialize arrays
      Int_t number_of_arrays = NUMBER_OF_ARRAYS;
      for ( Int_t j = 0; j < number_of_arrays; ++j ) {
	done[j]      = 0;
	pair_pass[j] = 0;
      }

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
	  chi2Probability[counter] = TMath::Prob(pair_DimuonVtxFitNormQui2, 1);
	  // Account for the number of pairs in the current event
	  ++counter;
	}
      } // End of for ( UInt_t j = i; j < n; ++j )


      // =================================================================================================
      if ( counter == 1 ) {
	// If Chi2/NDF < 0, it is a "bad" pair.
	if ( (NormChi2[0] < 0) || (fabs(tag_pt[0] - probe_pt[0]) < min_difference) || (NormChi2[0] > 100) )
	  pair_pass[0] = 0;
	
	else
	  pair_pass[0] = 1;
      }
      else { // (1)
	// =================================================================================================
	// Start looping again over the pairs, but only look at those ones found to
	// belong to the same event.
	for ( Int_t j = 0; j < (counter-1); ++j ) {
	  if ( (NormChi2[j] < 0) || (fabs(tag_pt[j] - probe_pt[j]) < min_difference) || (NormChi2[j] > 100) ) {
	    pair_pass[j] = 0; done[j] = 999; // This is a "bad" pair
	    continue;
	  }
	  else if ( (done[j] == 999) && (pair_pass[j] == 0) ) {
	    // This pair has already been analyzed and a final decision has alrealdy been given
	    // about this pair using "done[] = 999" and "pair_pass[j] = 0". It is a "bad" pair.
	    continue; 
	  }
	  else if ( (done[j] == 999) && (pair_pass[j] == 1) &&  (bestCandidate == 0) ) {
	    // Do nothing!!!
	    // This pair has been marked as "good" ("pair_pass[j] == 1"). Let's keep as it is.
	  }
	  else if ( (done[j] == 999) && (pair_pass[j] == 1) &&  (bestCandidate != 0) ) {
	    // This pair has been marked as "good" ("pair_pass[j] == 1"), but may be compared
	    // with a previous pair candidate already analyzed concening the best Chi2. Variable
	    // "done[j]" has to be updated accordingly.
	    done[j] = bestCandidate;
	  }
	  else if ( (done[j] != 999) && (done[j] > 0) ) {
	    // This pair has alrealdy been analyzed concening the best Chi2 condition. Variable
	    // "done[j]" has to be updated accordingly.
	    done[j] = bestCandidate;
	  }
	  else if ( (done[j] == 0) && (bestCandidate != 0) ) {
	    // This pair has alrealdy been analyzed concening the best Chi2 condition. Variable
	    // "done[j]" has to be updated accordingly, but since "done[j] = 0", it has been
	    // marked as a "bad" pair.
	    done[j] = bestCandidate;
	  }
	  else {
	    // This pair has not been analyzed and, in principle, it is a "good" pair. This
	    // situation may change later on, so...
	    pair_pass[j] = 1; done[j] = 0;
	  }


	  // =================================================================================================
	  // Loop over pairs of same event, starting from the first pair ahead.
	  // First of all, look for cases where a Tag becomes a Probe and vice-versa.
	  /*
	  for ( Int_t m = j+1; m < counter; ++m ) {
	    if ( done[m] == 999 ) {
	      // Do nothing!!! This pair has already been analyzed and a final decision
	      // has alrealdy been given about it using "done[] = 999".
	      continue;
	    }
	    else { // (3)
	      if ( (NormChi2[m] < 0) || (fabs(tag_pt[m] - probe_pt[m]) < min_difference) ) {
		pair_pass[m] = 0; done[m] = 999; // This is a "bad" pair
	      }
	      else { // (4)
		// Check if the Tag of the pair "j" has become a Probe of the pair "m" taking
		// into account the difference between their transverse momentum w.r.t. the
		// minimum difference defined above.
		if ( (fabs(tag_pt[j] - probe_pt[m]) <  min_difference) &&
		     (fabs(tag_pt[m] - probe_pt[j]) <  min_difference) ) {
		  if ( tag_pt[m] < probe_pt[m] ) {   // In the current pair, pT(Tag) < pT(Probe) ?
		    pair_pass[j] = 0; done[j] = 999; // Pair "j" is "bad"
		    pair_pass[m] = 1; done[m] = 999; // Pair "m" is "good"
		  }
		  else {                             // In the current pair, pT(Tag) > pT(Probe) ?
		    pair_pass[j] = 1; done[j] = 999; // Pair "j" is "good"
		    pair_pass[m] = 0; done[m] = 999; // Pair "m" is "bad"
		  }
		} // End of if ( (fabs(tag_pt[j] - probe_pt[m]) <  min_difference) &&
	        //             (fabs(probe_pt[j] - tag_pt[m]) <  min_difference) )
		else {
		  // Tag has NOT became a Probe, so mark current pair as "good" for while.
		  pair_pass[m] = 1; done[m] = 0;
		}
	      } // End of (4)
	    } // End of (3)
	  } // End of for ( Int_t m = j+1; m < counter; ++m )
	  */

	  // =================================================================================================
	  for ( Int_t m = j+1; m < counter; ++m ) {
	    if ( done[m] == 999 ) {
	      // Do nothing!!! This pair has already been analyzed and a final decision
	      // has alrealdy been given about it using "done[] = 999".
	      continue;
	    }
	    // Initializing pairs "m > j"
	    else pair_pass[m] = 1; done[m] = 0;
	  }

	  // The first condition [Tag <--> Probe] has already been looked for and, if found,
	  // pair "j" has already been marked as "bad" (as final answer). Then there
	  // is no meaning to keep looking at this pair. Continue if it is the case!
	  if ( (done[j] == 999) && (pair_pass[j] == 0) )
	    continue;

	  // =================================================================================================
	  // If case of [Tag <--> Probe] has not been found, loop again over pairs of the same
	  // event, starting from the first pair ahead, and look for a pair with the best Chi2/NDF.
	  // This pair will be marked as "good", and all other ones as "bad", even if it has already
	  // been marked as "good" when analyzing the case of Tag becoming a Probe [Tag <--> Probe].
	  for ( Int_t m = j+1; m < counter; ++m ) {
	    if ( (pair_pass[m] == 0) && (done[m] == 999) ) {
	      // Do nothing!!! This pair has already been analyzed and a final decision
	      // has alrealdy been given about it using "done[] = 999" and "pair_pass[j] = 0".
	      continue;
	    }

	    if ( (NormChi2[m] < 0) || (fabs(tag_pt[m] - probe_pt[m]) < min_difference) || (NormChi2[m] > 100) ) {
	      pair_pass[m] = 0; done[m] = 999;
	    }
	    else { // (5)
	      // Get the Chi2 probability of the pair whose Chi2/NDF is the closest to "1".
	      // A negative signal is given if pair "j" wins, and positive if pair "m".
	      min_Chi2Probability = (fabs(chi2Probability[j] - bestChi2Probability) <
				     fabs(chi2Probability[m] - bestChi2Probability)) ?
		                    chi2Probability[j]*(-1) : chi2Probability[m];

	      // If the pair has not been analyzed yet ("bestCandidate == 0") ...
	      if ( bestCandidate == 0 ) { // (6)
		// Check which pair has the best Chi2 probability. Pair "j" or pair "m" ?
		if (      min_Chi2Probability < 0) {
		  // Candidate with best Chi2 probability is identified via array "done[j]".
		  // "done[2] = 100+2" means that pair "j = 2" is the best one.
		  pair_pass[j] = 1; done[j] = 100+j;
		  pair_pass[m] = 0; done[m] = 0;
		}
		else if ( (min_Chi2Probability > 0) &&
			  (fabs(chi2Probability[j] - chi2Probability[m]) > min_difference) ) {
		  // "done[2] = 100+2" means that pair "m = 2" is the best one, but this
		  // information is kept using index "j" ("done[j] = 100+m") because the
		  // best candidate is chosen taking pair "j" as reference for the following pairs.
		  pair_pass[j] = 0; done[j] = 100+m;
		  pair_pass[m] = 1; done[m] = 1;
		}
		else if ( (min_Chi2Probability > 0) &&
			  (fabs(chi2Probability[j] - chi2Probability[m]) < min_difference) ) {
		  // This is the case where pairs "j" and "m" are composed by the same muons.
		  // Here, a Tag became a Probe and vice-versa.
		  pair_pass[j] = 1; done[j] = 100+m;
		  pair_pass[m] = 1; done[m] = 1;
		}
		else {
		  // This is the case of "min_Chi2Probability ~ 0".
		  // That is, both pairs have chi2 probability ~ 0.
		  // To be simple, let's make both pairs passing as good candidates.
		  pair_pass[j] = 1; done[j] = 100+m;
		  pair_pass[m] = 1; done[m] = 1;
		}

		// Get the index correspondent to the candidate marked as the best one
		bestCandidate = done[j];

	      } // End of (6)
	      else { // (7)
		// If the pair has already been analyzed ("done[j] == 100+j or 100+m") ...
		// Get the index correspondent to the candidate marked as the best one.
		bestCandidate = done[j] - 100;
		
		if ( (bestCandidate == j) || (bestCandidate == m) ) { // (7.1)
		  // Check which pair has the best Chi2 probability. Pair "j" or pair "m" ?
		  if (      min_Chi2Probability < 0) {
		    // Candidate with best Chi2 probability is identified via array "done[j]".
		    // "done[2] = 100+2" means that pair "j = 2" is the best one.
		    pair_pass[j] = 1; done[j] = 100+j;
		    pair_pass[m] = 0; done[m] = 0;
		  }
		  else if ( (min_Chi2Probability > 0) &&
			    (fabs(chi2Probability[j] - chi2Probability[m]) > min_difference) ) {
		    // "done[2] = 100+2" means that pair "m = 2" is the best one, but this
		    // information is kept using index "j" ("done[j] = 100+m") because the
		    // best candidate is chosen taking pair "j" as reference for the following pairs.
		    pair_pass[j] = 0; done[j] = 100+m;
		    pair_pass[m] = 1; done[m] = 1;
		  }
		  else if ( (min_Chi2Probability > 0) &&
			    (fabs(chi2Probability[j] - chi2Probability[m]) < min_difference) ) {
		    // This is the case where pairs "j" and "m" are composed by the same muons.
		    // Here, a Tag became a Probe and vice-versa.
		    pair_pass[j] = 1; done[j] = 100+m;
		    pair_pass[m] = 1; done[m] = 1;
		  }
		  else {
		    // This is the case of "min_Chi2Probability ~ 0".
		    // That is, both pairs have chi2 probability ~ 0.
		    // To be simple, let's make both pairs passing as good candidates.
		    pair_pass[j] = 1; done[j] = 100+m;
		    pair_pass[m] = 1; done[m] = 1;
		  }
		
		  // Get the index correspondent to the candidate marked as the best one
		  bestCandidate = done[j];
		  continue; // Comparision between pairs "j" and "m" are done at this point.
		} // End of (7.1)

		// Compute how far the Chi2 probability of pairs "j" or "m" is from the best
		// scenario (Chi2/NDF ~ 1). A positive sign indicates that previous best candidate
		// keeps being the best, while a negative sign means that previous best candidate
		// is no longer the best (may be pair "j" or "m").
		Chi2Probability = fabs(fabs(min_Chi2Probability)      - bestChi2Probability) -
		                  fabs(chi2Probability[bestCandidate] - bestChi2Probability);

		if (      (Chi2Probability > 0) ) {
		  // Previous best candidate wins
		  done[j]             = 100 + bestCandidate;   pair_pass[j]             = 0;
		  done[m]             = 0;                     pair_pass[m]             = 0;
		}
		else if ( (Chi2Probability < 0) && (min_Chi2Probability < 0) ) {
		  // Previous best candidate looses, and pair "j" wins
		  done[j]             = 100+j;                 pair_pass[j]             = 1;
		  done[m]             = 0;                     pair_pass[m]             = 0;
		  done[bestCandidate] = 0;                     pair_pass[bestCandidate] = 0;
		}
		else if ( (Chi2Probability < 0) && (min_Chi2Probability > 0) ) {
		  // Previous best candidate looses, and pair "m" wins
		  done[j]             = 100+m;                 pair_pass[j]             = 0;
		  done[m]             = 1;                     pair_pass[m]             = 1;
		  done[bestCandidate] = 0;                     pair_pass[bestCandidate] = 0;
		}
		else {
		  // This is not an expected case, where "Chi2Probability ~ 0".
		  // That is, either both pairs have chi2 probability very close to each other, or one of
		  // them has chi2 probability very close to a good pair candidate already analyzed.
		  // To be simple, let's make both pairs passing as good candidates.
		  done[j]             = 100 + bestCandidate;   pair_pass[j]             = 1;
		  done[m]             = 1;                     pair_pass[m]             = 1;
		}
		
		// Get the index correspondent to the candidate marked as the best pair.
		bestCandidate = done[j];

	      } // End of (7)
	    } // End of (5)
	  } // End of for ( Int_t m = j+1; m < counter; ++m )
	} // End of for ( Int_t j = 0; j < (counter-1); ++j )
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

	  if ( done[j] == 999 )        // Considering only "good" pairs...
	    ++pairs_ExchangeCondition; // how many passed the condition [Tag <--> Probe] ?
	  else {                       // ------------------------------------------------------
	    if ( done[j] > 0  )        // Considering only "good" pairs...
	      ++pairs_Chi2Condition;   // how many passed the condition [best Chi2/NDF] ?
	                               // ------------------------------------------------------
	    if ( done[j] == 0 )        // Considering only "good" pairs...
	      ++pairs_NoCondition;     // how many passed without reaching any condition above ?
	  }

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
	       << ",\t probe_pt: "     << NewTuneP_pt	   << ",\t tag_charge: " << tag_charge
	       << ",\t probe_charge: " << charge             << ",\t dz: "         << pair_dz
	       << ",\t mass: "         << pair_newTuneP_mass << ",\t Chi2: "       << pair_DimuonVtxFitNormQui2 << endl;
	}

	// Fill variable MT = sqrt{ 2*pT(tag)*ETmiss*(1 - cos(Dphi)) }
	// where   cos(Dphi) = cos(DeltaPhi( tag_NewTuneP_phi, NewTuneP_phi ))
	tag_MT = sqrt( 2*tag_NewTuneP_pt*tag_MET*(1 - cos(tag_NewTuneP_phi - tag_METphi)) );
	
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
	  printf ("Done %9d/%9d   %5.1f%%   (elapsed %5.1f min, remaining %5.1f min)\n", i, n, i*evDenom, totalTime, remaining); 
	  fflush (stdout);
	}
      }
    } // End of for (UInt_t i = 0; i < n; ++i)
  } // End of if ( choose_pair )

  printf ("==============================================================================================================\n");
  printf ("\t [[ Data Set: %s ]]\n\n",                       dataset_definition.c_str());
  printf ("\t -------------------------------------------------\n");
  printf ("\t Total number of pairs analyzed        = %9d\n", n);
  printf ("\t -------------------------------------------------\n");
  printf ("\t Number of pairs without specific selections:\n");
  printf ("\t   - good pairs                        = %9d\n", pairs_passed);
  printf ("\t   - bad pairs                         = %9d\n", pairs_failed);
  printf ("\t -------------------------------------------------\n");
  printf ("\t From pairs marked as \"good\":\n");
  printf ("\t   - condition [Tag <===> Probe]       = %9d\n", pairs_ExchangeCondition);
  printf ("\t   - condition [Best Chi2/NDF]         = %9d\n", pairs_Chi2Condition);
  printf ("\t   - condition [None of above]         = %9d\n", pairs_NoCondition);
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
    output_file << pairs_ExchangeCondition          << ", ";
    output_file << pairs_Chi2Condition              << ", ";
    output_file << pairs_NoCondition                << ", ";
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

  tOut->AutoSave(); // According to root tutorial this is the right thing to do
  fOut->Close();
  // If not wanting to create a new ROOT file, it is deleted in The End.
  if ( !create_new_root_file ) {
    stringstream deleting_file;
    deleting_file << ".!rm -f " << new_file_name; // Using bash commands via ROOT
    gROOT->ProcessLine( deleting_file.str().c_str() );
  }
}
