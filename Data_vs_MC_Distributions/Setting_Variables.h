/////////////////////////////////////////////////////////////////////
//                                                                 //
//                  Here starts the USER's block                   //
//                                                                 //
//    USER has to go through the lines bellow and include input    //
//    root files, variables to be read and plotted, as well as     //
//    their X and Y axis ranges, labels, legends and colors.       //
//                                                                 //
//    USERS can also define the bin edges to compute the number    //
//    of entries per bin per input file per variable.              //
//                                                                 //
/////////////////////////////////////////////////////////////////////
/********************************************************************

      Version 5, 07 April 2014
      _______________________________________________________________
      - v5:
           * comes up from version v4, incorporating version v3
           * takes care of using only a small setting of variables,
             mainly because User would like get number of entries
             of only a few variables.
	   * Now User has to tell to the code if wanting to either
	     analyze all variables or only a small set of them.
      - v4:
           * comes from version v1
           * includes information about variable "tag_METphi"
	   * takes care of settings for bottom plots as Data/MC and
	     (Data - MC)/uncertainty_of_Data
********************************************************************/

///////////////////////////////////////////////////////////////////
// Set width and height of canvas.
// NOTE: It is safer to not touch in the canvas settup, unless you
//       know what you are doing.
Float_t canvas_ratio = 0.29; // right side percent of the pad not covered
Int_t wCanvas = 750;  // width
Int_t hCanvas = 600;  // height

//////////////////////////////////////////////////////////////////
// Would you like to apply a linear (0) or a log scale (1) to Y axis?
Int_t linear_log_scale = 1;

//////////////////////////////////////////////////////////////////
// Would you like to only pop up the plots, or to only save them, or to do both?
// In case of USER running in batch mode ('root -b' option), this code
// will automatically save plots in ".png" format.
// What do you like to do?
//   - Pop up only -> 0
//   - Save only -> 1
//   - Both (pop up and save) -> 2
Int_t pop_save = 1;

//////////////////////////////////////////////////////////////////
// If USER wants to save plots, tell what format:
// pdf? png? eps? root?
string output_file_format[] = {"png", "pdf", "root"};

//////////////////////////////////////////////////////////////////
// If USER wants to save plots, provide a commom name.
string output_file_name = "distribution_TP_ID_v15.1_BaselineSelections_v9";
//string output_file_name = "distribution_TP_ID_v15.1_BaselineSelections_WithBestPair_v9";

//////////////////////////////////////////////////////////////////
// Would you like to get the entries per bin per input file per variable?
// If "NO"  -> output_entries_per_bin = 0  || (macro will create plots only)
// If "YES" -> output_entries_per_bin = 1  || (macro will get entries only)
// If "YES" -> output_entries_per_bin > 1  || (macro will do both things: create plots and get entries)
Int_t output_entries_per_bin = 0;

//////////////////////////////////////////////////////////////////
// Would you like to scale MC samples?
// If "NO"  ---> normalize = 0 (all MC samples will have a normalization factor == 1)
// If "YES" ---> normalize = 1 (from normalization in Z peak based on data / MC ratio)
// If "YES" ---> normalize = 2 (from User's pre-defined value)
Int_t normalize            = 1;
// In case of -> normalize = 2, User has to write bellow what is the MC normalization factor.
Float_t User_normalization = 1.06019;

//////////////////////////////////////////////////////////////////
// Set legend position
double xMinLegend = 0.71; // minimum x position of the legend
double yMinLegend = 0.01; // minimum y position of the legend
double xMaxLegend = 0.99; // maximum x position of the legend
double yMaxLegend = 1.00; // maximum y position of the legend

//////////////////////////////////////////////////////////////////
// Set pads position
Float_t padXmin  = 0.00;  // minimum x position of the pad1
Float_t padXmax  = 1.00;  // maximum x position of the pad1
Float_t padYmin1 = 0.50;  // minimum y position of the pad1
Float_t padYmax1 = 0.99;  // minimum y position of the pad1
Float_t padYmin2 = 0.30;  // minimum y position of the pad2
Float_t padYmax2 = 0.50;  // minimum y position of the pad2
Float_t padYmin3 = 0.00;  // minimum y position of the pad3
Float_t padYmax3 = 0.30;  // minimum y position of the pad3

// Set pad margins
Float_t PadToptMargin   = 0.01; // pad top margin
Float_t PadLeftMargin   = 0.10; // pad left margin
Float_t PadRightMargin  = 0.30; // pad right margin
Float_t PadBottomMargin = 0.30; // pad bottom margin

//////////////////////////////////////////////////////////////////
// Set dot size, line color and line width of ratio plots
Int_t dot_size    = 8;  // Size of dots in the Data/MC ratio
Int_t line_color  = 4;  // Straight line indicating the ideal position
Int_t line_width  = 1;  // Straight line indicating the ideal position
Int_t ratio_color = 46; // Color of dots and histograms in the ratio plots


//////////////////////////////////////////////////////////////////
// Setting histograms on the pads
//
// NOTE: It is safer to not touch in the histograms settup, unless you
//       know what you are doing.
//
// Setting histograms on pad1
Float_t y_title_offset1 = 0.63; // distance between label and Y axis.
Float_t y_titleSize1    = 0.06; // title size of Y axis
Float_t y_labelSize1    = 0.05; // size of number along Y axis
//
// Setting histograms on pad2
Float_t y_title_offset2 = 0.30; // distance between label and Y axis.
Float_t y_titleSize2    = 0.12; // title size of Y axis
Float_t y_labelSize2    = 0.12; // size number along Y axis
Float_t y_Min2          =-0.25; // minimum y position of ratio in pad2
Float_t y_Max2          = 2.75; // maximum y position of ratio in pad2
string  y_title2        = "Data / MC"; // title of Y axis
//
// Setting histograms on pad3
Float_t x_title_offset3 = 0.90; // distance between label and X axis.
Float_t x_titleSize3    = 0.15; // title size of X axis
Float_t x_labelSize3    = 0.10; // size of number along X axis
Float_t y_title_offset3 = 0.39; // distance between label and Y axis.
Float_t y_titleSize3    = 0.09; // title size of Y axis
Float_t y_labelSize3    = 0.08; // size of number along Y axis
Float_t y_Min3          = -5.0; // minimum y position of ratio in pad3
Float_t y_Max3          = 4.50; // maximum y position of ratio in pad3
string  y_title3        = "(Data-MC)/#sigma_{Data}"; // title of Y axis


//////////////////////////////////////////////////////////////////
// Include bellow as many lines as you need to add ROOT files.
// First input ROOT file must have DATA events (root file 0).
// The code will create histogrms in stacks following the order
// of the input root files as they appear here: the histogram
// on the top corresponds to the input root file 1, while the
// histogram on the bottom corresponds to the last input root
// file entry bellow.
//============================[[ ROOT files with addBestPair_RunEvent_v9.cxx ]]===========================
//============================[[ Information about selected events only      ]]===========================

string input_files[] = {
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_Data_RunC__Mass_mu1_mu2__gt_0_GeV.root",       // root file 0 (Data)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_20_GeV.root",        // root file 1 (DY->MuMu M 20)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_120_GeV.root",       // root file 2 (DY->MuMu M 120)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_200_GeV.root",       // root file 3 (DY->MuMu M 200)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_500_GeV.root",       // root file 4 (DY->MuMu M 500)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_800_GeV.root",       // root file 5 (DY->MuMu M 800)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_1000_GeV.root",      // root file 6 (DY->MuMu M 1000)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_1500_GeV.root",      // root file 7 (DY->MuMu M 1500)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_2000_GeV.root",      // root file 8 (DY->MuMu M 2000)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_DY_tautau__Mass_tau__tau___gt_0_GeV.root",     // root file 9 (DY->TauTau M 20)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_ttbar__Mass_mu1_mu2__gt_0_GeV.root",           // root file 10 (ttbar)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_tW__Mass_mu1_mu2__gt_0_GeV.root",              // root file 11 (tW)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_tbarW__Mass_mu1_mu2__gt_0_GeV.root",           // root file 12 (tbarW)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_WW__Mass_mu1_mu2__gt_0_GeV.root",              // root file 13 (WW)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_WZ__Mass_mu1_mu2__gt_0_GeV.root",              // root file 14 (WZ)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_ZZ__Mass_mu1_mu2__gt_0_GeV.root",              // root file 15 (ZZ)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_W_Jets__Mass_mu1_mu2__gt_0_GeV.root",          // root file 16 (W+Jets->lnu)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_Inclusive_QCD__Mass_mu1_mu2__gt_0_GeV.root"    // root file 17 (Inclusive QCD)
};

//============================[[ ROOT files with addBestPair_RunEvent_v9.cxx            ]]================================
//============================[[ Information about selected events + filtered T&P pairs ]]===========================
/*
string input_files[] = {
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_Data_RunC__Mass_mu1_mu2__gt_0_GeV.root",       // root file 0 (Data)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_20_GeV.root",        // root file 1 (DY->MuMu M 20)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_120_GeV.root",       // root file 2 (DY->MuMu M 120)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_200_GeV.root",       // root file 3 (DY->MuMu M 200)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_500_GeV.root",       // root file 4 (DY->MuMu M 500)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_800_GeV.root",       // root file 5 (DY->MuMu M 800)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_1000_GeV.root",      // root file 6 (DY->MuMu M 1000)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_1500_GeV.root",      // root file 7 (DY->MuMu M 1500)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_2000_GeV.root",      // root file 8 (DY->MuMu M 2000)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_DY_tautau__Mass_tau__tau___gt_0_GeV.root",     // root file 9 (DY->TauTau M 20)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_ttbar__Mass_mu1_mu2__gt_0_GeV.root",           // root file 10 (ttbar)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_tW__Mass_mu1_mu2__gt_0_GeV.root",              // root file 11 (tW)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_tbarW__Mass_mu1_mu2__gt_0_GeV.root",           // root file 12 (tbarW)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_WW__Mass_mu1_mu2__gt_0_GeV.root",              // root file 13 (WW)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_WZ__Mass_mu1_mu2__gt_0_GeV.root",              // root file 14 (WZ)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_ZZ__Mass_mu1_mu2__gt_0_GeV.root",              // root file 15 (ZZ)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_W_Jets__Mass_mu1_mu2__gt_0_GeV.root",          // root file 16 (W+Jets->lnu)
  "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_Inclusive_QCD__Mass_mu1_mu2__gt_0_GeV.root"    // root file 17 (Inclusive QCD)
};
*/

//////////////////////////////////////////////////////////////////
// Write the weight of each input ROOT file.
// Of course, weight of DATA is "1.0".
double weight[] = {
  1.0,      // input file 0  (DATA)
  4.0989119,//    2.5756,   // input file 1  (DY->MuMu M 20)
  0.8787172,//    0.5522,   // input file 2  (DY->MuMu M 120)
  0.1097428,//    0.0690,   // input file 3  (DY->MuMu M 200)
  0.0032626,//    0.0021,   // input file 4  (DY->MuMu M 500)
  0.0004058,//    0.0003,   // input file 5  (DY->MuMu M 800)
  0.0001327,//    0.00008,  // input file 6  (DY->MuMu M 1000)
  0.0000126,//    0.000008, // input file 7  (DY->MuMu M 1500)
  0.0000016,//    0.000001, // input file 8  (DY->MuMu M 2000)
  4.0970485,//    2.5745,   // input file 9  (DY->TauTau M 20)
  0.0761073,//    0.0478,   // input file 10 (ttbar) // Verify with Cesar's results.
  0.1572465,//    0.0988,   // input file 11 (tW)
  0.1585843,//    0.0996,   // input file 12 (tbarW)
  0.0386323,//    0.0243,   // input file 13 (WW)
  0.0234053,//    0.0147,   // input file 14 (WZ)
  0.0126613,//    0.0080,   // input file 15 (ZZ)
  13.897167,//    8.7325,   // input file 16 (W+Jets->lnu)
  44.194163 //    27.7702   // input file 17 (Inclusive QCD)
  //    0.4290    // input file 18 (DY+Jets->LL M 50)
};

//////////////////////////////////////////////////////////////////
// Write the list of variables inside an array.
string list_of_variables[] = {
  "pair_newTuneP_probe_pt", //--------------> 1
  "eta", //---------------------------------> 2
  "phi", //---------------------------------> 3
  "pair_newTuneP_mass", //------------------> 4
  "tag_nVertices", //-----------------------> 5
  "pair_deltaR", //-------------------------> 6
  "probe_tkRelIso", //----------------------> 7 //
  "combRelIsoPF04dBeta", //-----------------> 8
  "tag_combRelIsoPF04dBeta", //-------------> 9
  "innertkSigmaPtOverPt", //----------------> 10
  "tag_innertrackPtRelError", //------------> 11
  "NewTuneP_eta", //------------------------> 12
  "tag_NewTuneP_eta", //--------------------> 13
  "NewTuneP_phi", //------------------------> 14
  "tag_NewTuneP_phi", //--------------------> 15
  "NewTuneP_pt", //-------------------------> 16
  "tag_NewTuneP_pt", //---------------------> 17
  "tag_MET", //-----------------------------> 18
  "(TMath::Pi() - pair_collinearity1)", //--> 19
  "pair_DimuonVtxProbePtAtTheVtx", //-------> 20
  "pair_DimuonVtxTagPtAtTheVtx", //---------> 21
  "pair_DimuonVtxProbePtBefore", //---------> 22
  "pair_DimuonVtxTagPtBefore", //-----------> 23
  "pair_DimuonVtxZcoordinate", //-----------> 24
  "pair_DimuonVtxRdistance", //-------------> 25
  "pair_DimuonVtxRdistanceFromBS", //-------> 26
  "pair_DimuonVtxFitNormQui2", //-----------> 27
  "pair_dz", //-----------------------------> 28
  "(tag_NewTuneP_pt - NewTuneP_pt) / (tag_NewTuneP_pt + NewTuneP_pt)", //--> 29
  "tag_MT", //------------------------------> 30
  "tag_METphi", //--------------------------> 31
  "pair_DimuonVtxZcoordinateFromBS" //------> 32
};

//////////////////////////////////////////////////////////////////
// In case of USER wanting to get results only for a small set of variables (when getting the
// number of entries, for example), User has to tell which are the variable names (as above)
// he/she is interested in. First of all, User has to tell if wanting to either analyze all
// variables or only a small set of them.
// WARNING: it is mandatory to have the variable of dimuon-mass ("pair_newTuneP_mass") included.
Int_t analyze_few_variables = 0; // 0 -> Analyze all variables
/////////////////////////////////// 1 -> Analyze only a small set of variables (as listed bellow)
string small_set_of_variables[] = {
  "pair_newTuneP_mass", //------------------> 4
  "tag_nVertices", //-----------------------> 5
  "pair_deltaR", //-------------------------> 6
  "NewTuneP_eta", //------------------------> 12
  "NewTuneP_phi", //------------------------> 14
  "NewTuneP_pt" //--------------------------> 16
};

//////////////////////////////////////////////////////////////////
// USER has to tell what is the variable name related to the mass.
string mass_variable = "pair_newTuneP_mass";

//////////////////////////////////////////////////////////////////
// Write the selections on the mass with respect to each input ROOT file.
string mass_selection[] = {
  "(pair_newTuneP_mass >=   0.0) && (pair_newTuneP_mass < 3000.0)", // root file 0  (Data)
  "(pair_newTuneP_mass >=   20.0) && (pair_newTuneP_mass <  120.0)", // root file 1  (DY->MuMu M 20)
  "(pair_newTuneP_mass >=  120.0) && (pair_newTuneP_mass <  200.0)", // root file 2  (DY->MuMu M 120)
  "(pair_newTuneP_mass >=  200.0) && (pair_newTuneP_mass <  500.0)", // root file 3  (DY->MuMu M 200)
  "(pair_newTuneP_mass >=  500.0) && (pair_newTuneP_mass <  800.0)", // root file 4  (DY->MuMu M 500)
  "(pair_newTuneP_mass >=  800.0) && (pair_newTuneP_mass < 1000.0)", // root file 5  (DY->MuMu M 800)
  "(pair_newTuneP_mass >= 1000.0) && (pair_newTuneP_mass < 1500.0)", // root file 6  (DY->MuMu M 1000)
  "(pair_newTuneP_mass >= 1500.0) && (pair_newTuneP_mass < 2000.0)", // root file 7  (DY->MuMu M 1500)
  "(pair_newTuneP_mass >= 2000.0)",                                  // root file 8  (DY->MuMu M 2000)
  "(pair_newTuneP_mass >=  20.0) && (pair_newTuneP_mass < 3000.0)", // root file 9  (DY->TauTau M 20)
  "(pair_newTuneP_mass >=   0.0) && (pair_newTuneP_mass < 3000.0)", // root file 10 (ttbar)
  "(pair_newTuneP_mass >=   0.0) && (pair_newTuneP_mass < 3000.0)", // root file 11 (tW)
  "(pair_newTuneP_mass >=   0.0) && (pair_newTuneP_mass < 3000.0)", // root file 12 (tbarW)
  "(pair_newTuneP_mass >=   0.0) && (pair_newTuneP_mass < 3000.0)", // root file 13 (WW)
  "(pair_newTuneP_mass >=   0.0) && (pair_newTuneP_mass < 3000.0)", // root file 14 (WZ)
  "(pair_newTuneP_mass >=   0.0) && (pair_newTuneP_mass < 3000.0)", // root file 15 (ZZ)
  "(pair_newTuneP_mass >=   0.0) && (pair_newTuneP_mass < 3000.0)", // root file 16 (W+Jets->lnu)
  "(pair_newTuneP_mass >=   0.0) && (pair_newTuneP_mass < 3000.0)"  // root file 17 (Inclusive QCD)
};

//////////////////////////////////////////////////////////////////
// Write the default event selections (with exception of mass).
string default_selection = "(pair_passBaselineSelections == 1) && \
(pair_DimuonVtxFitNormQui2 < 2.0) && \
((tag_NewTuneP_pt - NewTuneP_pt) / (tag_NewTuneP_pt + NewTuneP_pt) > -0.4) && \
((tag_NewTuneP_pt - NewTuneP_pt) / (tag_NewTuneP_pt + NewTuneP_pt) <  0.4)";
// && (tag_MET < 50.0) && \
//(((tag_NewTuneP_pt - NewTuneP_pt) / (tag_NewTuneP_pt + NewTuneP_pt)) > -0.5) && \
//(((tag_NewTuneP_pt - NewTuneP_pt) / (tag_NewTuneP_pt + NewTuneP_pt)) < 0.4)";
//(pair_passBaselineSelections == 1) && (pair_DimuonVtxFitNormQui2 > 0.5) && \
//(pair_DimuonVtxFitNormQui2 < 1.5) && (tag_MET < 50.0) && \
//(pair_DimuonVtxRdistanceFromBS < 0.25) && (probe_tkRelIso > 0.0) && \
//(((tag_NewTuneP_pt - NewTuneP_pt) / (tag_NewTuneP_pt + NewTuneP_pt)) > -0.5) && \
//(((tag_NewTuneP_pt - NewTuneP_pt) / (tag_NewTuneP_pt + NewTuneP_pt)) < 0.4)";

//(pair_BestPair == 1) && (charge != tag_charge) && (NewTuneP_eta > -2.4) && \
//(NewTuneP_eta < 2.4) && (tag_NewTuneP_eta > -2.1) && (tag_NewTuneP_eta < 2.1) && (tag_Mu40 == 1) && \
//(pair_newTuneP_mass > 20.) && (pair_DimuonVtxFitNormQui2 < 10.) && (tag_NewTuneP_pt > 45.) && \
//(NewTuneP_pt > 45.) && (pair_dz > -0.2) && (pair_dz < 0.2) &&		\
//(tag_combRelIsoPF04dBeta < 0.12) && (tag_innertrackPtRelError < 0.3)";
//(pair_collinearity1 < (TMath::Pi() - 0.02))
//(pair_newTuneP_mass > 20.) && (tag_NewTuneP_pt > 45.) && \
//(pair_DimuonVtxFitNormQui2 < 10.)";
//(tag_NewTuneP_pt > 45.) && (pair_collinearity1 > 0.02) && (pair_dz >= -0.2) && (pair_dz <= 0.2) && \
//(pair_DimuonVtxFitNormQui2 < 10.)";

//////////////////////////////////////////////////////////////////
// Number of bins, Y and X axis ranges and stylistic definitions from USER.
//
// If you want, you can also provide the histogram title and the Y and X
// axis labels for each variable. Labels are giving to each histogram
// corresponding to each input root file.
// Give also a different color to each histogram corresponding to each input
// root file.
// Order of numbers bellow has to follow order of the list of variables provided above.
int number_of_bins[] = {100, 50, 34, 100, 60, 100, 100,
			100, 100, 100, 100, 50, 50, 34,
			34, 100, 100, 100, 100, 100, 100,
			100, 100, 100, 100, 100, 100, 100,
			100, 100, 34,100};
//---------------------------------------------------------------
double x_minimum[] = {0., -2.5, -3.4, 0., 0., 0., 0.,
		      0., 0., 0., 0., -2.5, -2.5, -3.4,
		      -3.4, 0., 0., 0., 0., 0., 0.,
		      0., 0., -50., 0., 0., 0., -0.25,
		      -1., 0., -3.4, -50.};
//---------------------------------------------------------------
double x_maximum[] = {1000., 2.5, 3.4, 3000., 60., 6., 0.12,
		      0.5, 0.15, 1.0, 0.12, 2.5, 2.5, 3.4,
		      3.4, 1000., 1000., 500., 3.5, 500., 500.,
		      500., 500., 50., 1., 1., 11., 0.25,
		      1., 800., 3.4, 50.};
//---------------------------------------------------------------
double y_minimum[] = {0.01, 10., 10., 0.001, 0.01, 0.5, 10.,
		      0.1, 0.1, 1., 1., 1., 10., 1.,
		      10., 0.01, 0.01, 0.5, 1., 0.01, 0.01,
		      0.01, 0.01, 0.5, 100., 0.001, 10., 0.1,
		      0.1, 0.5, 10., 0.5};
//---------------------------------------------------------------
double y_maximum[] = {1000000., 50000., 50000., 1000000., 100000., 500000., 1000000.,
		      1000000., 1000000., 1000000., 1000000., 50000., 50000., 50000.,
		      50000., 1000000., 2000000., 500000., 50000., 1000000., 1000000.,
		      1000000., 1000000., 500000., 500000., 2000000., 1000000., 1000000.,
		      1000000., 500000., 50000., 500000.};
//---------------------------------------------------------------

//////////////////////////////////////////////////////////////////
// Provide label of X axis.
string x_axis_label[] = {
  "p_{T} #left[GeV/c#right]",
  "#eta",
  "#phi [rad]",
  "m_{#mu#mu} #left[GeV/c^{2}#right]",
  "Number of Vertices",
  "#Delta R (#mu,#mu)",
  "IsolationR03 SumPt/pt", // ===================================
  "combRelIsoPF04dBeta", //
  "tag_combRelIsoPF04dBeta", //
  "innertkSigmaPtOverPt",
  "tag_innertrackPtRelError",
  "NewTuneP_eta",
  "tag_NewTuneP_eta",
  "NewTuneP_phi",
  "tag_NewTuneP_phi",
  "NewTuneP_pt",
  "tag_NewTuneP_pt",
  "tag_MET",
  "pair_collinearity1",
  "pair_DimuonVtxProbePtAtTheVtx",
  "pair_DimuonVtxTagPtAtTheVtx",
  "pair_DimuonVtxProbePtBefore",
  "pair_DimuonVtxTagPtBefore",
  "pair_DimuonVtxZcoordinate",
  "pair_DimuonVtxRdistance",
  "pair_DimuonVtxRdistanceFromBS",
  "pair_DimuonVtxFitNormQui2",
  "pair_dz",
  "p_{T} balance",
  "Transverse Mass [GeV]",
  "#phi (MET) [rad]",
  "pair_DimuonVtxZcoordinateFromBS"
};

//////////////////////////////////////////////////////////////////
// Provide label of Y axis.
string y_axis_label[] ={
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events",
  "Events"
};

//////////////////////////////////////////////////////////////////
// Provide titles (Not implemented yet)
string title_of_histogram[] = {"", "", "", "", "", "", "",
			       "", "", "", "", "", "", "",
			       "", "", "", "", "", "", "",
			       "", "", "", "", "", "", "",
                               "", "", "", ""};

//////////////////////////////////////////////////////////////////
// Provide label of histograms concerning each input ROOT file
string label_of_histogram[] = {
  "Data-RunC, 7.05 fb^{-1}",   // root file 0  (Data)
  "DY #rightarrow #mu#mu, 20 < M(#mu#mu) <  120",   // root file 1  (DY->MuMu M 20)
  "DY #rightarrow #mu#mu, 120 < M(#mu#mu) <  200",  // root file 2  (DY->MuMu M 120)
  "DY #rightarrow #mu#mu, 200 < M(#mu#mu) <  500",  // root file 3  (DY->MuMu M 200)
  "DY #rightarrow #mu#mu, 500 < M(#mu#mu) <  800",  // root file 4  (DY->MuMu M 500)
  "DY #rightarrow #mu#mu, 800 < M(#mu#mu) < 1000",  // root file 5  (DY->MuMu M 800)
  "DY #rightarrow #mu#mu, 1000 < M(#mu#mu) < 1500", // root file 6  (DY->MuMu M 1000)
  "DY #rightarrow #mu#mu, 1500 < M(#mu#mu) < 2000", // root file 7  (DY->MuMu M 1500)
  "DY #rightarrow #mu#mu, M(#mu#mu) > 2000",        // root file 8  (DY->MuMu M 2000)
  "DY #rightarrow #tau#tau",                        // root file 9  (DY->TauTau M 20)
  "t#bar{t}",                                       // root file 10 (ttbar)
  "tW",                                             // root file 11 (tW)
  "#bar{t}W",                                       // root file 12 (tbarW)
  "WW",                                             // root file 13 (WW)
  "WZ",                                             // root file 14 (WZ)
  "ZZ",                                             // root file 15 (ZZ)
  "W+Jets #rightarrow l#nu",                        // root file 16 (W+Jets->lnu)
  "Inclusive QCD"                                   // root file 17 (Inclusive QCD)
};

//////////////////////////////////////////////////////////////////
// The first color number will be the color of the histograms correspondent to the first
// input ROOT file, while the last color number will be assigned to the last input file.
int color_of_histogram[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};

//////////////////////////////////////////////////////////////////
// Write the binning for each variable:
//
double pt_bins[] = {45., 50., 55., 60., 65., 70., 75., 80., 85., 90., 140., 300., 500., 1000.};
double eta_bins[] = {-2.4, -2.1, -1.6, -1.2, -0.9, -0.6, -0.3, -0.2, 0.2, 0.3, 0.6, 0.9, 1.2, 1.6, 2.1, 2.4};
double phi_bins[] = {-3.0, -2.5, -2.0, -1.5, -1.0, -0.5, 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0};
double mass_bins[] = {20, 120, 200, 500, 800, 1000, 1500, 2000, 3000};
double vertex_bins[] = {0.5, 2.5, 4.5, 6.5, 8.5, 10.5, 12.5, 14.5, 16.5, 18.5, 20.5, 22.5, 24.5, 26.5, 28.5, 30.5, 32.5, 34.5, 36.5, 38.5, 40.5, 42.5, 44.5, 46.5, 48.5, 49.5};
double deltaR_bins[] = {0.0, 0.5, 0.75, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 6.0};
double probeIso_bins[] = {0.0, 0.005, 0.01, 0.015, 0.02, 0.025, 0.03, 0.035, 0.04, 0.045, 0.05, 0.055, 0.06, 0.065, 0.07, 0.075, 0.08, 0.085, 0.09, 0.095, 0.1, 0.105, 0.11};// ===================================================================
double combRelIsoPF04dBeta_bins[] = {0.0, 0.005, 0.01};
double tag_combRelIsoPF04dBeta_bins[] = {0.0, 0.005, 0.01};
double innertkSigmaPtOverPt_bins[] = {0.0, 0.005, 0.0};
double tag_innertrackPtRelError_bins[] = {0.0, 0.005, 0.01};
double NewTuneP_eta_bins[] = {-2.4, -2.1, -1.6, -1.2, -0.9, -0.6, -0.3, -0.2, 0.2, 0.3, 0.6, 0.9, 1.2, 1.6, 2.1, 2.4};
double tag_NewTuneP_eta_bins[] = {-2.4, -2.1, -1.6, -1.2, -0.9, -0.6, -0.3, -0.2, 0.2, 0.3, 0.6, 0.9, 1.2, 1.6, 2.1, 2.4};
double NewTuneP_phi_bins[] = {-3.0, -2.5, -2.0, -1.5, -1.0, -0.5, 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0};
double tag_NewTuneP_phi_bins[] = {-3.0, -2.5, -2.0, -1.5, -1.0, -0.5, 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0};
double NewTuneP_pt_bins[] = {45., 50., 55., 60., 65., 70., 75., 80., 85., 90., 140., 300., 500., 1000.};
double tag_NewTuneP_pt_bins[] = {45., 50., 55., 60., 65., 70., 75., 80., 85., 90., 140., 300., 500., 1000.};
double tag_MET_bins[] = {0.0, 0.005, 0.01};
double pair_collinearity1_bins[] = {0.0, 0.005, 0.01};
double pair_DimuonVtxProbePtAtTheVtx_bins[] = {45., 50., 55., 60., 65., 70., 75., 80., 85., 90., 140., 300., 500., 1000.};
double pair_DimuonVtxTagPtAtTheVtx_bins[] = {45., 50., 55., 60., 65., 70., 75., 80., 85., 90., 140., 300., 500., 1000.};
double pair_DimuonVtxProbePtBefore_bins[] = {45., 50., 55., 60., 65., 70., 75., 80., 85., 90., 140., 300., 500., 1000.};
double pair_DimuonVtxTagPtBefore_bins[] = {45., 50., 55., 60., 65., 70., 75., 80., 85., 90., 140., 300., 500., 1000.};
double pair_DimuonVtxZcoordinate_bins[] = {0.0, 0.005, 0.01};
double pair_DimuonVtxRdistance_bins[] = {0.0, 0.005, 0.01};
double pair_DimuonVtxRdistanceFromBS_bins[] = {0.0, 0.005, 0.01};
double pair_DimuonVtxFitNormQui2_bins[] = {0.0, 0.005, 0.01};
double pair_dz_bins[] = {0.0, 0.005, 0.01};
double pt_balance_bins[] = {-1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
double tag_MT_bins[] = {0.0, 0.005, 0.01};
double tag_METphi_bins[] = {-3.0, -2.5, -2.0, -1.5, -1.0, -0.5, 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0};
double pair_DimuonVtxZcoordinateFromBS_bins[] = {0.0, 0.005, 0.01};


////////////////////////////////////////////////////////////////////
// Make sure the loops bellow correspond to the variables above.  //
////////////////////////////////////////////////////////////////////
void fill_bins (vector <vector <double> > &bin_edges) {
  Int_t total_number_of_variables = (sizeof(list_of_variables) / sizeof(string));
  bin_edges.resize(total_number_of_variables + 1);

  for ( Int_t i = 0; i < (sizeof(pt_bins) / sizeof(double)); i++ )
    bin_edges[0].push_back( pt_bins[i] );
  for ( Int_t i = 0; i < (sizeof(eta_bins) / sizeof(double)); i++ )
    bin_edges[1].push_back( eta_bins[i] );
  for ( Int_t i = 0; i < (sizeof(phi_bins) / sizeof(double)); i++ )
    bin_edges[2].push_back( phi_bins[i] );
  for ( Int_t i = 0; i < (sizeof(mass_bins) / sizeof(double)); i++ )
    bin_edges[3].push_back( mass_bins[i] );
  for ( Int_t i = 0; i < (sizeof(vertex_bins) / sizeof(double)); i++ )
    bin_edges[4].push_back( vertex_bins[i] );
  for ( Int_t i = 0; i < (sizeof(deltaR_bins) / sizeof(double)); i++ )
    bin_edges[5].push_back( deltaR_bins[i] );
  for ( Int_t i = 0; i < (sizeof(probeIso_bins) / sizeof(double)); i++ )
    bin_edges[6].push_back( probeIso_bins[i] );
  for ( Int_t i = 0; i < (sizeof(combRelIsoPF04dBeta_bins) / sizeof(double)); i++ )
    bin_edges[7].push_back( combRelIsoPF04dBeta_bins[i] );
  for ( Int_t i = 0; i < (sizeof(tag_combRelIsoPF04dBeta_bins) / sizeof(double)); i++ )
    bin_edges[8].push_back( tag_combRelIsoPF04dBeta_bins[i] );
  for ( Int_t i = 0; i < (sizeof(innertkSigmaPtOverPt_bins) / sizeof(double)); i++ )
    bin_edges[9].push_back( innertkSigmaPtOverPt_bins[i] );
  for ( Int_t i = 0; i < (sizeof(tag_innertrackPtRelError_bins) / sizeof(double)); i++ )
    bin_edges[10].push_back( tag_innertrackPtRelError_bins[i] );
  for ( Int_t i = 0; i < (sizeof(NewTuneP_eta_bins) / sizeof(double)); i++ )
    bin_edges[11].push_back( NewTuneP_eta_bins[i] );
  for ( Int_t i = 0; i < (sizeof(tag_NewTuneP_eta_bins) / sizeof(double)); i++ )
    bin_edges[12].push_back( tag_NewTuneP_eta_bins[i] );
  for ( Int_t i = 0; i < (sizeof(NewTuneP_phi_bins) / sizeof(double)); i++ )
    bin_edges[13].push_back( NewTuneP_phi_bins[i] );
  for ( Int_t i = 0; i < (sizeof(tag_NewTuneP_phi_bins) / sizeof(double)); i++ )
    bin_edges[14].push_back( tag_NewTuneP_phi_bins[i] );
  for ( Int_t i = 0; i < (sizeof(NewTuneP_pt_bins) / sizeof(double)); i++ )
    bin_edges[15].push_back( NewTuneP_pt_bins[i] );
  for ( Int_t i = 0; i < (sizeof(tag_NewTuneP_pt_bins) / sizeof(double)); i++ )
    bin_edges[16].push_back( tag_NewTuneP_pt_bins[i] );
  for ( Int_t i = 0; i < (sizeof(tag_MET_bins) / sizeof(double)); i++ )
    bin_edges[17].push_back( tag_MET_bins[i] );
  for ( Int_t i = 0; i < (sizeof(pair_collinearity1_bins) / sizeof(double)); i++ )
    bin_edges[18].push_back( pair_collinearity1_bins[i] );
  for ( Int_t i = 0; i < (sizeof(pair_DimuonVtxProbePtAtTheVtx_bins) / sizeof(double)); i++ )
    bin_edges[19].push_back( pair_DimuonVtxProbePtAtTheVtx_bins[i] );
  for ( Int_t i = 0; i < (sizeof(pair_DimuonVtxTagPtAtTheVtx_bins) / sizeof(double)); i++ )
    bin_edges[20].push_back( pair_DimuonVtxTagPtAtTheVtx_bins[i] );
  for ( Int_t i = 0; i < (sizeof(pair_DimuonVtxProbePtBefore_bins) / sizeof(double)); i++ )
    bin_edges[21].push_back( pair_DimuonVtxProbePtBefore_bins[i] );
  for ( Int_t i = 0; i < (sizeof(pair_DimuonVtxTagPtBefore_bins) / sizeof(double)); i++ )
    bin_edges[22].push_back( pair_DimuonVtxTagPtBefore_bins[i] );
  for ( Int_t i = 0; i < (sizeof(pair_DimuonVtxZcoordinate_bins) / sizeof(double)); i++ )
    bin_edges[23].push_back( pair_DimuonVtxZcoordinate_bins[i] );
  for ( Int_t i = 0; i < (sizeof(pair_DimuonVtxRdistance_bins) / sizeof(double)); i++ )
    bin_edges[24].push_back( pair_DimuonVtxRdistance_bins[i] );
  for ( Int_t i = 0; i < (sizeof(pair_DimuonVtxRdistanceFromBS_bins) / sizeof(double)); i++ )
    bin_edges[25].push_back( pair_DimuonVtxRdistanceFromBS_bins[i] );
  for ( Int_t i = 0; i < (sizeof(pair_DimuonVtxFitNormQui2_bins) / sizeof(double)); i++ )
    bin_edges[26].push_back( pair_DimuonVtxFitNormQui2_bins[i] );
  for ( Int_t i = 0; i < (sizeof(pair_dz_bins) / sizeof(double)); i++ )
    bin_edges[27].push_back( pair_dz_bins[i] );
  for ( Int_t i = 0; i < (sizeof(pt_balance_bins) / sizeof(double)); i++ )
    bin_edges[28].push_back( pt_balance_bins[i] );
  for ( Int_t i = 0; i < (sizeof(tag_MT_bins) / sizeof(double)); i++ )
    bin_edges[29].push_back( tag_MT_bins[i] );
  for ( Int_t i = 0; i < (sizeof(tag_METphi_bins) / sizeof(double)); i++ )
    bin_edges[30].push_back( tag_METphi_bins[i] );
  for ( Int_t i = 0; i < (sizeof(pair_DimuonVtxZcoordinateFromBS_bins) / sizeof(double)); i++ )
    bin_edges[31].push_back( pair_DimuonVtxZcoordinateFromBS_bins[i] );
}
/////////////////////////////////////////////////
/////////                            ////////////
/////////          End of File       ////////////
/////////                            ////////////
/////////////////////////////////////////////////
