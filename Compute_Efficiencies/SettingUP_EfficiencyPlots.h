/*************************************************************

  MACRO FILE: SettingUP_EfficiencyPlots.h

  This is a config file used as an include into macro file
    plotMuonID_MC_DATA_efficiencies_v10.cpp
  and should contain all User's setup and functions to
  properly create plots of efficiency from counting method
  of Tag & Probe.

  * Version:
  ________________________________________________________
    * v1 (08, April, 2014)
      - User can choose if wanting to save or not the plots
      - choose plots' name
      - say which variable will be analyzed and how many
        bins are going to be created for each variable
      - include input ROOT files containing separated
        efficiencies from Data and MC samples
      -

**************************************************************/

// Do you want to save the plots?
// Yes -> 1
// No -> 0
Int_t save = 1;
// What format?
string format[] = {"png", "pdf", "root"};
// Give a standard name for the files?
string file_name = "Efficiency_BestPairBaseline_v9_07Apr2014";

// Write all variables you would like to have in a plot of efficiency.
string variable_name[] = {"eta", "mass", "phi", "pt", "vertex", "deltaR"};//, "TkIsoRel"};

// Provide the number of bins for each variable.
Int_t number_of_bins[] = {15, 8, 12, 13, 25, 12};//, 22};

// Set title for each one of the efficiency cases
string All_MC_title     = "All MC";
string Data_title       = "DATA-RunC";
string Final_Data_title = "DATA-RunC Final";
string DY_Jets_title    = "DY #rightarrow LL+Jets";
string DY_MC_title      = "Only DY #rightarrow #mu#mu";

// Path to the ROOT files
// BestPairBaseline_v9
string files_path[] = {
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_v9_07Apr2014/TnP_Z_CountHighMassDiMuID_Data_RunC.root",                                // input file 0 (Data)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_v9_07Apr2014/TnP_Z_CountHighMassDiMuID_MC_DY_mumu__Mass_mu1_mu2__gt_20_GeV.root",      // input file 1 (DY->MuMu M 20)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_v9_07Apr2014/TnP_Z_CountHighMassDiMuID_MC_DY_mumu__Mass_mu1_mu2__gt_120_GeV.root",     // input file 2 (DY->MuMu M 120)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_v9_07Apr2014/TnP_Z_CountHighMassDiMuID_MC_DY_mumu__Mass_mu1_mu2__gt_200_GeV.root",     // input file 3 (DY->MuMu M 200)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_v9_07Apr2014/TnP_Z_CountHighMassDiMuID_MC_DY_mumu__Mass_mu1_mu2__gt_500_GeV.root",     // input file 4 (DY->MuMu M 500)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_v9_07Apr2014/TnP_Z_CountHighMassDiMuID_MC_DY_mumu__Mass_mu1_mu2__gt_800_GeV.root",     // input file 5 (DY->MuMu M 800)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_v9_07Apr2014/TnP_Z_CountHighMassDiMuID_MC_DY_mumu__Mass_mu1_mu2__gt_1000_GeV.root",    // input file 6 (DY->MuMu M 1000)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_v9_07Apr2014/TnP_Z_CountHighMassDiMuID_MC_DY_mumu__Mass_mu1_mu2__gt_1500_GeV.root",    // input file 7 (DY->MuMu M 1500)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_v9_07Apr2014/TnP_Z_CountHighMassDiMuID_MC_DY_mumu__Mass_mu1_mu2__gt_2000_GeV.root",    // input file 8 (DY->MuMu M 2000)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_v9_07Apr2014/TnP_Z_CountHighMassDiMuID_MC_DY_tautau__Mass_tau__tau___gt_0_GeV.root",   // input file 9 (DY->TauTau M 20)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_v9_07Apr2014/TnP_Z_CountHighMassDiMuID_MC_ttbar.root",                                 // input file 10 (ttbar)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_v9_07Apr2014/TnP_Z_CountHighMassDiMuID_MC_tW.root",                                    // input file 11 (tW)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_v9_07Apr2014/TnP_Z_CountHighMassDiMuID_MC_tbarW.root",                                 // input file 12 (tbarW)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_v9_07Apr2014/TnP_Z_CountHighMassDiMuID_MC_WW.root",                                    // input file 13 (WW)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_v9_07Apr2014/TnP_Z_CountHighMassDiMuID_MC_WZ.root",                                    // input file 14 (WZ)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_v9_07Apr2014/TnP_Z_CountHighMassDiMuID_MC_ZZ.root",                                    // input file 15 (ZZ)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_v9_07Apr2014/TnP_Z_CountHighMassDiMuID_MC_W_Jets.root",                                // input file 16 (W+Jets->lnu)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_v9_07Apr2014/TnP_Z_CountHighMassDiMuID_MC_Inclusive_QCD.root",                         // input file 17 (Inclusive QCD)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_v9_07Apr2014/TnP_Z_CountHighMassDiMuID_MC_DY_Jets_ll.root"                             // input file 18 (DY+Jets->LL M 50)
};
/*
// BestPairBaseline_WithBestPair_v9
string files_path[] = {
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_WithBestPair_v9_08Apr2014/TnP_Z_CountHighMassDiMuID_Data_RunC.root",                                // input file 0 (Data)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_WithBestPair_v9_08Apr2014/TnP_Z_CountHighMassDiMuID_MC_DY_mumu__Mass_mu1_mu2__gt_20_GeV.root",      // input file 1 (DY->MuMu M 20)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_WithBestPair_v9_08Apr2014/TnP_Z_CountHighMassDiMuID_MC_DY_mumu__Mass_mu1_mu2__gt_120_GeV.root",     // input file 2 (DY->MuMu M 120)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_WithBestPair_v9_08Apr2014/TnP_Z_CountHighMassDiMuID_MC_DY_mumu__Mass_mu1_mu2__gt_200_GeV.root",     // input file 3 (DY->MuMu M 200)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_WithBestPair_v9_08Apr2014/TnP_Z_CountHighMassDiMuID_MC_DY_mumu__Mass_mu1_mu2__gt_500_GeV.root",     // input file 4 (DY->MuMu M 500)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_WithBestPair_v9_08Apr2014/TnP_Z_CountHighMassDiMuID_MC_DY_mumu__Mass_mu1_mu2__gt_800_GeV.root",     // input file 5 (DY->MuMu M 800)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_WithBestPair_v9_08Apr2014/TnP_Z_CountHighMassDiMuID_MC_DY_mumu__Mass_mu1_mu2__gt_1000_GeV.root",    // input file 6 (DY->MuMu M 1000)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_WithBestPair_v9_08Apr2014/TnP_Z_CountHighMassDiMuID_MC_DY_mumu__Mass_mu1_mu2__gt_1500_GeV.root",    // input file 7 (DY->MuMu M 1500)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_WithBestPair_v9_08Apr2014/TnP_Z_CountHighMassDiMuID_MC_DY_mumu__Mass_mu1_mu2__gt_2000_GeV.root",    // input file 8 (DY->MuMu M 2000)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_WithBestPair_v9_08Apr2014/TnP_Z_CountHighMassDiMuID_MC_DY_tautau__Mass_tau__tau___gt_0_GeV.root",   // input file 9 (DY->TauTau M 20)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_WithBestPair_v9_08Apr2014/TnP_Z_CountHighMassDiMuID_MC_ttbar.root",                                 // input file 10 (ttbar)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_WithBestPair_v9_08Apr2014/TnP_Z_CountHighMassDiMuID_MC_tW.root",                                    // input file 11 (tW)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_WithBestPair_v9_08Apr2014/TnP_Z_CountHighMassDiMuID_MC_tbarW.root",                                 // input file 12 (tbarW)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_WithBestPair_v9_08Apr2014/TnP_Z_CountHighMassDiMuID_MC_WW.root",                                    // input file 13 (WW)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_WithBestPair_v9_08Apr2014/TnP_Z_CountHighMassDiMuID_MC_WZ.root",                                    // input file 14 (WZ)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_WithBestPair_v9_08Apr2014/TnP_Z_CountHighMassDiMuID_MC_ZZ.root",                                    // input file 15 (ZZ)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_WithBestPair_v9_08Apr2014/TnP_Z_CountHighMassDiMuID_MC_W_Jets.root",                                // input file 16 (W+Jets->lnu)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_WithBestPair_v9_08Apr2014/TnP_Z_CountHighMassDiMuID_MC_Inclusive_QCD.root",                         // input file 17 (Inclusive QCD)
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v5_ComputeEfficiencies_Aug_14_2013/ROOTfiles_CountEffHighMassDimuon_BestPairBaseline_WithBestPair_v9_08Apr2014/TnP_Z_CountHighMassDiMuID_MC_DY_Jets_ll.root"                             // input file 18 (DY+Jets->LL M 50)
};
*/
// Scale factors correspondent to each MC sample above
Double_t scale_factors[] = {
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
  44.194163,//    27.7702   // input file 17 (Inclusive QCD)
  8.3918589 //    0.4290    // input file 18 (DY+Jets->LL M 50)
};

// Check number of all MC samples and only DY samples
Int_t number_of_MC_samples = (sizeof (scale_factors) / sizeof (Double_t));
Int_t number_of_DY_samples = (sizeof (scale_factors) / sizeof (Double_t)) - 10;

// This function is used to tell the path of the separated efficiency plots inside the input ROOT files
void set_auxiliar_string ( vector <string> variable, stringstream &auxiliar_string, Int_t v ) {
  auxiliar_string.str("");
  if ( variable[v] == "eta" )    auxiliar_string << "tpTree/Tight2012_ETA_BINS/cnt_eff_plots/NewTuneP_eta_PLOT_pair_passBaselineSelections_pass";
  if ( variable[v] == "mass" )   auxiliar_string << "tpTree/Tight2012_MASS_BINS/cnt_eff_plots/pair_newTuneP_mass_PLOT_pair_passBaselineSelections_pass";
  if ( variable[v] == "phi" )    auxiliar_string << "tpTree/Tight2012_PHI_BINS/cnt_eff_plots/NewTuneP_phi_PLOT_pair_passBaselineSelections_pass";
  if ( variable[v] == "pt" )     auxiliar_string << "tpTree/Tight2012_PT_BINS/cnt_eff_plots/NewTuneP_pt_PLOT_pair_passBaselineSelections_pass";
  if ( variable[v] == "vertex" ) auxiliar_string << "tpTree/Tight2012_VTX_BINS/cnt_eff_plots/tag_nVertices_PLOT_pair_passBaselineSelections_pass";
  if ( variable[v] == "deltaR" ) auxiliar_string << "tpTree/Tight2012_deltaR_BINS/cnt_eff_plots/pair_deltaR_PLOT_pair_passBaselineSelections_pass";
}

// This function sets the title in the X axis
void set_Xaxis_label ( vector <string> variable, stringstream &Xaxis_label, Int_t v ) {
  Xaxis_label.str("");
  if ( variable[v] == "eta" )    Xaxis_label << "#eta";
  if ( variable[v] == "mass" )   Xaxis_label << "m_{#mu#mu} #left[GeV/c^{2}#right]";
  if ( variable[v] == "phi" )    Xaxis_label << "#phi #left[rad#right]";
  if ( variable[v] == "pt" )     Xaxis_label << "p_{T} #left[GeV/c#right]";
  if ( variable[v] == "vertex" ) Xaxis_label << "Number of Vertices";
  if ( variable[v] == "deltaR" ) Xaxis_label << "#Delta R (#mu,#mu)";
}

// This function is used to get the number of entries per variable per sample per bin.
// These numbers comes when running macro file "TagAndProbe_stacks_V14.cpp" with option
//    Int_t output_entries_per_bin = 1;
// User has to take care when pasting the number bellow.
void get_entries_per_bin ( vector <string> variable, Int_t v, Int_t sample, Int_t bin, long &entries_per_bin ) {
  //  ** Variable -> <<<<<   pair_newTuneP_probe_pt   >>>>>
  //    ** Bin edges:		{45,	50,	55,	60,	65,	70,	75,	80,	85,	90,	140,	300,	500,	1000};
  //  long File[15];
  if ( variable[v] == "pt" ) {
    //BaselineSelection_v9

    if (sample == 0)	{long File[] =	{47914,	13037,	5686,	2759,	1702,	1112,	728,	584,	463,	1805,	562,	30,	3};}
    if (sample == 1)	{long File[] =	{3825,	5651,	7536,	8220,	6756,	5057,	3790,	2654,	1891,	3525,	200,	2,	0};}
    if (sample == 2)	{long File[] =	{893,	1229,	1544,	1941,	2280,	2778,	3196,	3764,	4360,	45975,	16746,	141,	9};}
    if (sample == 3)	{long File[] =	{23,	41,	51,	61,	72,	118,	135,	192,	210,	5183,	66283,	18073,	184};}
    if (sample == 4)	{long File[] =	{0,	2,	4,	0,	4,	6,	11,	16,	19,	821,	15526,	45137,	2095};}
    if (sample == 5)	{long File[] =	{0,	0,	0,	1,	0,	1,	3,	3,	7,	260,	9456,	36122,	30366};}
    if (sample == 6)	{long File[] =	{0,	0,	0,	0,	0,	0,	0,	0,	0,	7,	1783,	9376,	31511};}
    if (sample == 7)	{long File[] =	{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	255,	3248,	11665};}
    if (sample == 8)	{long File[] =	{37,	32,	19,	21,	13,	5,	11,	7,	10,	27,	9,	0,	0};}
    if (sample == 9)	{long File[] =	{44094,	34205,	27573,	21856,	17649,	14449,	12322,	10206,	8444,	37571,	12860,	1157,	745};}
    if (sample == 10)	{long File[] =	{972,	747,	595,	467,	404,	311,	287,	234,	188,	942,	391,	24,	10};}
    if (sample == 11)	{long File[] =	{947,	744,	594,	504,	405,	317,	267,	259,	194,	987,	391,	27,	18};}
    if (sample == 12)	{long File[] =	{10060,	8418,	6790,	5482,	4591,	3642,	3023,	2421,	2084,	8999,	2991,	260,	132};}
    if (sample == 13)	{long File[] =	{15490,	10705,	7837,	5978,	4774,	3787,	3100,	2550,	2021,	9090,	3424,	285,	89};}
    if (sample == 14)	{long File[] =	{26704,	17962,	13227,	10204,	8014,	6382,	5118,	4173,	3564,	16397,	6829,	512,	131};}
    if (sample == 15)	{long File[] =	{1144,	848,	663,	475,	352,	276,	216,	165,	143,	532,	215,	48,	31};}
    if (sample == 16)	{long File[] =	{6,	4,	2,	1,	0,	0,	0,	0,	2,	0,	0,	0,	0};}

    //BaselineSelection_WithBestPair_v9
    /*
    if (sample == 0)	{long File[] =	{46202,	12023,	5024,	2270,	1265,	758,	485,	387,	271,	955,	207,	7,	0};}
    if (sample == 1)	{long File[] =	{3617,	5413,	7324,	7998,	6573,	4899,	3654,	2547,	1789,	3108,	108,	0,	0};}
    if (sample == 2)	{long File[] =	{757,	1088,	1388,	1775,	2124,	2571,	2971,	3521,	4119,	43833,	14969,	40,	1};}
    if (sample == 3)	{long File[] =	{13,	24,	33,	43,	54,	95,	110,	159,	185,	4627,	61116,	15747,	50};}
    if (sample == 4)	{long File[] =	{0,	0,	1,	0,	3,	2,	7,	9,	14,	669,	13803,	40899,	1368};}
    if (sample == 5)	{long File[] =	{0,	0,	0,	1,	0,	1,	0,	0,	5,	194,	8281,	32061,	26434};}
    if (sample == 6)	{long File[] =	{0,	0,	0,	0,	0,	0,	0,	0,	0,	2,	1511,	8155,	27343};}
    if (sample == 7)	{long File[] =	{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	217,	2791,	10030};}
    if (sample == 8)	{long File[] =	{29,	20,	11,	12,	9,	3,	8,	6,	8,	16,	8,	0,	0};}
    if (sample == 9)	{long File[] =	{26881,	21150,	17369,	13886,	11371,	9389,	7861,	6569,	5537,	24289,	7660,	602,	440};}
    if (sample == 10)	{long File[] =	{644,	532,	411,	330,	287,	216,	214,	172,	145,	673,	239,	10,	5};}
    if (sample == 11)	{long File[] =	{623,	512,	436,	363,	288,	231,	193,	196,	138,	732,	269,	13,	12};}
    if (sample == 12)	{long File[] =	{8449,	7109,	5856,	4798,	4007,	3168,	2643,	2103,	1810,	7839,	2481,	200,	102};}
    if (sample == 13)	{long File[] =	{11212,	7309,	5208,	3952,	3064,	2354,	1918,	1557,	1190,	4905,	1527,	107,	51};}
    if (sample == 14)	{long File[] =	{20093,	13034,	9216,	7002,	5451,	4162,	3319,	2600,	2198,	9609,	3378,	211,	55};}
    if (sample == 15)	{long File[] =	{808,	552,	447,	321,	243,	185,	131,	111,	99,	331,	138,	40,	22};}
    if (sample == 16)	{long File[] =	{3,	3,	1,	1,	0,	0,	0,	0,	2,	0,	0,	0,	0};}
    */
  }
  //  ** Variable -> <<<<<   eta   >>>>>
  //  *** Bin edges:{-2.4,-2.1,-1.6,-1.2,-0.9,-0.6,-0.3,-0.2,0.2,0.3,0.6,0.9,1.2,1.6,2.1,2.4};}
  else if ( variable[v] == "eta" ) {
    //BaselineSelection_v9

    if (sample == 0)	{long File[] =	{1721,	5663,	6579,	5638,	6159,	6243,	2044,	8473,	2037,	6285,	5933,	5557,	6489,	5751,	1813};}
    if (sample == 1)	{long File[] =	{1611,	3807,	3787,	3400,	3747,	4048,	1407,	5598,	1411,	4034,	3746,	3443,	3749,	3742,	1577};}
    if (sample == 2)	{long File[] =	{3068,	7139,	6876,	5837,	6295,	6630,	2241,	9052,	2259,	6538,	6288,	5800,	6739,	6970,	3124};}
    if (sample == 3)	{long File[] =	{2628,	6870,	7172,	6279,	7098,	7440,	2517,	10264,	2530,	7536,	7221,	6310,	7346,	6729,	2686};}
    if (sample == 4)	{long File[] =	{1471,	4335,	4927,	4502,	5258,	5545,	1957,	7633,	1950,	5656,	5192,	4566,	4980,	4355,	1318};}
    if (sample == 5)	{long File[] =	{1696,	5061,	5973,	5693,	6260,	6435,	2294,	9054,	2213,	6662,	6188,	5723,	6130,	5215,	1656};}
    if (sample == 6)	{long File[] =	{910,	3297,	4086,	3521,	3607,	3188,	1001,	3851,	1020,	3295,	3579,	3587,	4185,	3268,	892};}
    if (sample == 7)	{long File[] =	{668,	2309,	2274,	1503,	1086,	675,	156,	578,	161,	645,	1060,	1585,	2355,	2273,	680};}
    if (sample == 8)	{long File[] =	{3,	15,	13,	13,	15,	10,	5,	20,	9,	19,	14,	15,	12,	23,	5};}
    if (sample == 9)	{long File[] =	{3772,	12103,	16819,	17859,	21608,	23962,	8430,	34682,	8552,	24040,	21387,	17594,	16931,	12180,	3858};}
    if (sample == 10)	{long File[] =	{76,	269,	349,	407,	478,	584,	197,	855,	197,	565,	520,	365,	385,	255,	82};}
    if (sample == 11)	{long File[] =	{79,	250,	408,	399,	454,	573,	189,	917,	204,	582,	574,	358,	370,	244,	66};}
    if (sample == 12)	{long File[] =	{1707,	4158,	4595,	4175,	4563,	5022,	1690,	6964,	1806,	4980,	4711,	4233,	4586,	4071,	1723};}
    if (sample == 13)	{long File[] =	{1748,	4605,	5355,	4992,	5607,	6114,	2116,	8303,	2086,	6010,	5623,	4915,	5411,	4590,	1747};}
    if (sample == 14)	{long File[] =	{2506,	7568,	9256,	8763,	9857,	10717,	3681,	14399,	3647,	10580,	9904,	8785,	9377,	7629,	2609};}
    if (sample == 15)	{long File[] =	{133,	329,	407,	344,	433,	422,	173,	654,	153,	447,	425,	347,	405,	329,	130};}
    if (sample == 16)	{long File[] =	{2,	0,	1,	0,	1,	0,	0,	1,	0,	2,	1,	1,	3,	3,	0};}

    //BaselineSelection_WithBestPair_v9
    /*
    if (sample == 0)	{long File[] =	{1574,	5243,	6049,	5223,	5598,	5653,	1859,	7655,	1841,	5683,	5398,	5083,	5976,	5354,	1665};}
    if (sample == 1)	{long File[] =	{1549,	3639,	3618,	3253,	3581,	3894,	1338,	5368,	1347,	3878,	3568,	3297,	3590,	3598,	1512};}
    if (sample == 2)	{long File[] =	{2875,	6677,	6401,	5472,	5872,	6154,	2091,	8402,	2107,	6066,	5871,	5409,	6299,	6533,	2928};}
    if (sample == 3)	{long File[] =	{2379,	6256,	6502,	5709,	6418,	6738,	2297,	9274,	2310,	6820,	6533,	5732,	6681,	6160,	2447};}
    if (sample == 4)	{long File[] =	{1322,	3834,	4427,	4014,	4668,	4947,	1749,	6783,	1735,	5020,	4628,	4123,	4434,	3902,	1190};}
    if (sample == 5)	{long File[] =	{1474,	4487,	5238,	4970,	5517,	5640,	2043,	7905,	1962,	5863,	5407,	5052,	5367,	4601,	1467};}
    if (sample == 6)	{long File[] =	{788,	2882,	3546,	3071,	3079,	2739,	840,	3327,	867,	2811,	3060,	3106,	3650,	2851,	784};}
    if (sample == 7)	{long File[] =	{584,	1988,	1944,	1286,	911,	571,	129,	469,	133,	540,	879,	1335,	2022,	1952,	583};}
    if (sample == 8)	{long File[] =	{3,	12,	10,	9,	9,	7,	1,	12,	6,	12,	12,	11,	7,	15,	4};}
    if (sample == 9)	{long File[] =	{2414,	7573,	10649,	11257,	13522,	15052,	5265,	21740,	5379,	15070,	13445,	11099,	10732,	7731,	2434};}
    if (sample == 10)	{long File[] =	{53,	186,	249,	278,	338,	398,	139,	589,	145,	392,	360,	251,	273,	175,	57};}
    if (sample == 11)	{long File[] =	{59,	184,	306,	276,	318,	404,	137,	651,	141,	404,	403,	247,	262,	172,	48};}
    if (sample == 12)	{long File[] =	{1537,	3651,	3975,	3616,	3912,	4243,	1429,	5849,	1534,	4190,	3955,	3654,	3973,	3577,	1534};}
    if (sample == 13)	{long File[] =	{1126,	3069,	3461,	3226,	3541,	3868,	1324,	5262,	1338,	3777,	3523,	3129,	3563,	3053,	1143};}
    if (sample == 14)	{long File[] =	{1688,	5264,	6357,	5951,	6600,	7194,	2406,	9457,	2433,	6952,	6560,	5969,	6456,	5251,	1817};}
    if (sample == 15)	{long File[] =	{102,	233,	281,	231,	269,	267,	114,	420,	108,	295,	276,	236,	282,	235,	98};}
    if (sample == 16)	{long File[] =	{1,	0,	1,	0,	0,	0,	0,	1,	0,	2,	0,	1,	2,	2,	0};}
    */
  }
  //  ** Variable -> <<<<<   phi   >>>>>
  //  *** Bin edges:{-3,-2.5,-2,-1.5,-1,-0.5,0,0.5,1,1.5,2,2.5,3};}
  else if ( variable[v] == "phi" ) {
    //BaselineSelection_v9

    if (sample == 0)	{long File[] =	{5907,	5986,	6263,	6025,	6170,	6173,	5953,	6092,	6157,	6054,	6222,	5992};}
    if (sample == 1)	{long File[] =	{3887,	3758,	3969,	3942,	3939,	3933,	3878,	3989,	4023,	3800,	3915,	3931};}
    if (sample == 2)	{long File[] =	{6683,	6688,	6751,	6783,	6678,	6883,	6682,	6922,	6743,	6815,	6689,	6685};}
    if (sample == 3)	{long File[] =	{7099,	7377,	7172,	7159,	7274,	7161,	7098,	7385,	7350,	7177,	7239,	7106};}
    if (sample == 4)	{long File[] =	{4995,	4974,	5096,	5097,	5140,	5064,	4923,	5171,	5160,	5177,	5064,	5057};}
    if (sample == 5)	{long File[] =	{5901,	6082,	6256,	5941,	6049,	6099,	5993,	6130,	6218,	5996,	6169,	6099};}
    if (sample == 6)	{long File[] =	{3163,	3403,	3592,	3494,	3504,	3522,	3517,	3685,	3435,	3340,	3439,	3436};}
    if (sample == 7)	{long File[] =	{1260,	1413,	1590,	1424,	1465,	1365,	1529,	1577,	1504,	1439,	1408,	1370};}
    if (sample == 8)	{long File[] =	{10,	16,	15,	17,	18,	21,	14,	17,	15,	9,	13,	19};}
    if (sample == 9)	{long File[] =	{19318,	19287,	19398,	19271,	19735,	19459,	19124,	19299,	19434,	19292,	19733,	19494};}
    if (sample == 10)	{long File[] =	{413,	468,	417,	454,	460,	448,	423,	444,	444,	471,	456,	440};}
    if (sample == 11)	{long File[] =	{419,	439,	431,	494,	464,	440,	440,	446,	511,	446,	476,	422};}
    if (sample == 12)	{long File[] =	{4633,	4623,	4603,	4637,	4670,	4841,	4701,	4735,	4733,	4633,	4803,	4674};}
    if (sample == 13)	{long File[] =	{5461,	5434,	5487,	5354,	5664,	5560,	5465,	5536,	5687,	5521,	5608,	5432};}
    if (sample == 14)	{long File[] =	{9404,	9615,	9454,	9264,	9597,	9447,	9423,	9483,	9825,	9431,	9358,	9624};}
    if (sample == 15)	{long File[] =	{390,	404,	406,	408,	448,	356,	381,	421,	427,	429,	421,	425};}
    if (sample == 16)	{long File[] =	{2,	1,	1,	0,	0,	1,	1,	1,	3,	1,	1,	2};}

    //BaselineSelection_WithBestPair_v9
    /*
    if (sample == 0)	{long File[] =	{5394,	5462,	5770,	5492,	5649,	5643,	5474,	5571,	5643,	5497,	5688,	5481};}
    if (sample == 1)	{long File[] =	{3733,	3595,	3804,	3773,	3758,	3768,	3720,	3812,	3853,	3643,	3755,	3759};}
    if (sample == 2)	{long File[] =	{6247,	6219,	6227,	6326,	6245,	6448,	6253,	6490,	6254,	6357,	6219,	6245};}
    if (sample == 3)	{long File[] =	{6427,	6708,	6502,	6492,	6581,	6517,	6448,	6696,	6678,	6482,	6555,	6472};}
    if (sample == 4)	{long File[] =	{4443,	4445,	4552,	4531,	4618,	4503,	4370,	4594,	4608,	4637,	4528,	4523};}
    if (sample == 5)	{long File[] =	{5184,	5337,	5482,	5220,	5301,	5328,	5272,	5421,	5484,	5267,	5443,	5355};}
    if (sample == 6)	{long File[] =	{2756,	2988,	3075,	3023,	3034,	3009,	3081,	3198,	2955,	2903,	2928,	2948};}
    if (sample == 7)	{long File[] =	{1082,	1207,	1352,	1199,	1257,	1144,	1312,	1348,	1272,	1228,	1177,	1187};}
    if (sample == 8)	{long File[] =	{3,	13,	10,	9,	13,	15,	12,	13,	9,	5,	10,	13};}
    if (sample == 9)	{long File[] =	{12223,	11969,	12208,	12109,	12505,	12211,	12158,	12061,	12235,	12088,	12409,	12292};}
    if (sample == 10)	{long File[] =	{279,	309,	297,	330,	335,	310,	303,	312,	297,	334,	306,	299};}
    if (sample == 11)	{long File[] =	{299,	311,	301,	370,	319,	317,	320,	309,	364,	321,	322,	303};}
    if (sample == 12)	{long File[] =	{3949,	3936,	3932,	3987,	4015,	4142,	4034,	4084,	4048,	3994,	4150,	4028};}
    if (sample == 13)	{long File[] =	{3508,	3464,	3478,	3445,	3628,	3578,	3540,	3548,	3714,	3519,	3615,	3480};}
    if (sample == 14)	{long File[] =	{6281,	6492,	6368,	6196,	6554,	6317,	6384,	6500,	6590,	6290,	6312,	6484};}
    if (sample == 15)	{long File[] =	{264,	278,	282,	251,	303,	246,	267,	275,	282,	277,	302,	271};}
    if (sample == 16)	{long File[] =	{2,	0,	0,	0,	0,	1,	0,	1,	3,	1,	0,	1};}
    */
  }
  //  ** Variable -> <<<<<   pair_newTuneP_mass   >>>>>
  //  *** Bin edges:{20,120,200,500,800,1000,1500,2000,3000};}
  else if ( variable[v] == "mass" ) {
    //BaselineSelection_v9

    if (sample == 0)	{long File[] =	{76385,	0,	0,	0,	0,	0,	0,	0};}
    if (sample == 1)	{long File[] =	{0,	49107,	0,	0,	0,	0,	0,	0};}
    if (sample == 2)	{long File[] =	{0,	0,	84856,	0,	0,	0,	0,	0};}
    if (sample == 3)	{long File[] =	{0,	0,	0,	90626,	0,	0,	0,	0};}
    if (sample == 4)	{long File[] =	{0,	0,	0,	0,	63645,	0,	0,	0};}
    if (sample == 5)	{long File[] =	{0,	0,	0,	0,	0,	76253,	0,	0};}
    if (sample == 6)	{long File[] =	{0,	0,	0,	0,	0,	0,	43287,	0};}
    if (sample == 7)	{long File[] =	{0,	0,	0,	0,	0,	0,	0,	17752};}
    if (sample == 8)	{long File[] =	{75,	83,	33,	0,	0,	0,	0,	0};}
    if (sample == 9)	{long File[] =	{70831,	109701,	60749,	1913,	282,	196,	71,	34};}
    if (sample == 10)	{long File[] =	{1432,	2501,	1596,	35,	9,	8,	0,	3};}
    if (sample == 11)	{long File[] =	{1427,	2577,	1599,	50,	8,	5,	1,	0};}
    if (sample == 12)	{long File[] =	{12292,	29049,	16750,	723,	93,	62,	12,	3};}
    if (sample == 13)	{long File[] =	{50215,	10898,	7609,	390,	52,	37,	15,	6};}
    if (sample == 14)	{long File[] =	{95411,	15107,	8210,	440,	58,	40,	9,	3};}
    if (sample == 15)	{long File[] =	{1123,	2586,	1338,	66,	5,	5,	4,	4};}
    if (sample == 16)	{long File[] =	{4,	7,	4,	0,	0,	0,	0,	0};}

    //BaselineSelection_WithBestPair_v9
    /*
    if (sample == 0)	{long File[] =	{69854,	0,	0,	0,	0,	0,	0,	0};}
    if (sample == 1)	{long File[] =	{0,	47030,	0,	0,	0,	0,	0,	0};}
    if (sample == 2)	{long File[] =	{0,	0,	79157,	0,	0,	0,	0,	0};}
    if (sample == 3)	{long File[] =	{0,	0,	0,	82256,	0,	0,	0,	0};}
    if (sample == 4)	{long File[] =	{0,	0,	0,	0,	56776,	0,	0,	0};}
    if (sample == 5)	{long File[] =	{0,	0,	0,	0,	0,	66993,	0,	0};}
    if (sample == 6)	{long File[] =	{0,	0,	0,	0,	0,	0,	37401,	0};}
    if (sample == 7)	{long File[] =	{0,	0,	0,	0,	0,	0,	0,	15120};}
    if (sample == 8)	{long File[] =	{48,	58,	24,	0,	0,	0,	0,	0};}
    if (sample == 9)	{long File[] =	{44402,	70939,	36631,	1066,	157,	116,	28,	23};}
    if (sample == 10)	{long File[] =	{1005,	1789,	1063,	18,	3,	3,	0,	2};}
    if (sample == 11)	{long File[] =	{998,	1876,	1102,	27,	5,	4,	0,	0};}
    if (sample == 12)	{long File[] =	{10591,	25358,	13962,	586,	78,	44,	8,	2};}
    if (sample == 13)	{long File[] =	{33521,	6518,	4117,	184,	25,	25,	8,	5};}
    if (sample == 14)	{long File[] =	{67413,	8484,	4175,	228,	30,	20,	4,	1};}
    if (sample == 15)	{long File[] =	{746,	1821,	827,	39,	4,	3,	4,	3};}
    if (sample == 16)	{long File[] =	{4,	5,	1,	0,	0,	0,	0,	0};}
    */
  }
  //  ** Variable -> <<<<<   tag_nVertices   >>>>>
  //  ** Bin edges:		{0.5,	2.5,	4.5,	6.5,	8.5,	10.5,	12.5,	14.5,	16.5,	18.5,	20.5,	22.5,	24.5,	26.5,	28.5,	30.5,	32.5,	34.5,	36.5,	38.5,	40.5,	42.5,	44.5,	46.5,	48.5,	49.5};
  else if ( variable[v] == "vertex" ) {
    //BaselineSelection_v9

    if (sample == 0)	{long File[] =	{100,	808,	2608,	5116,	7799,	9760,	10333,	9508,	8613,	6595,	5205,	3871,	2507,	1521,	962,	492,	315,	151,	68,	22,	13,	9,	5,	0,	0};}
    if (sample == 1)	{long File[] =	{75,	542,	1610,	3346,	5018,	6310,	6545,	6165,	5461,	4334,	3331,	2408,	1553,	1039,	632,	331,	223,	93,	53,	27,	5,	5,	1,	0,	0};}
    if (sample == 2)	{long File[] =	{87,	837,	2794,	5688,	8460,	10950,	11541,	10720,	9189,	7399,	5887,	4112,	2868,	1832,	1081,	703,	326,	186,	117,	41,	20,	11,	6,	0,	0};}
    if (sample == 3)	{long File[] =	{116,	924,	3095,	6154,	9320,	11293,	12057,	11286,	10048,	7973,	6276,	4538,	3000,	1956,	1172,	688,	356,	193,	82,	59,	26,	11,	1,	0,	0};}
    if (sample == 4)	{long File[] =	{97,	689,	2059,	4408,	6393,	7850,	8443,	8080,	7005,	5812,	4237,	3062,	2089,	1462,	883,	506,	285,	136,	80,	37,	18,	8,	3,	3,	0};}
    if (sample == 5)	{long File[] =	{101,	811,	2612,	5174,	7584,	9626,	10051,	9631,	8592,	6868,	5096,	3748,	2464,	1584,	1032,	574,	339,	189,	94,	44,	14,	16,	5,	0,	2};}
    if (sample == 6)	{long File[] =	{54,	457,	1448,	2931,	4452,	5577,	5773,	5504,	4643,	3865,	2995,	2112,	1427,	863,	533,	335,	169,	78,	49,	9,	8,	4,	0,	0,	1};}
    if (sample == 7)	{long File[] =	{27,	192,	627,	1256,	1859,	2250,	2464,	2229,	1959,	1629,	1171,	854,	582,	399,	214,	156,	78,	34,	15,	7,	3,	2,	1,	0,	0};}
    if (sample == 8)	{long File[] =	{0,	1,	8,	10,	23,	26,	25,	22,	22,	14,	11,	13,	8,	4,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0};}
    if (sample == 9)	{long File[] =	{303,	2351,	7730,	15902,	24473,	30729,	32788,	31026,	26790,	22026,	16800,	12105,	8220,	5315,	3266,	1830,	1082,	523,	261,	141,	63,	27,	17,	6,	0};}
    if (sample == 10)	{long File[] =	{6,	56,	167,	360,	550,	745,	747,	653,	601,	507,	412,	314,	176,	118,	84,	32,	37,	7,	5,	5,	1,	1,	0,	0,	0};}
    if (sample == 11)	{long File[] =	{3,	63,	202,	374,	567,	749,	726,	710,	612,	509,	358,	299,	212,	134,	52,	37,	21,	23,	6,	5,	4,	1,	0,	0,	0};}
    if (sample == 12)	{long File[] =	{58,	619,	2073,	3883,	6079,	7459,	8068,	7388,	6429,	5161,	3970,	2853,	1935,	1295,	727,	451,	276,	160,	48,	25,	11,	9,	5,	1,	0};}
    if (sample == 13)	{long File[] =	{79,	682,	2247,	4661,	7175,	8647,	9397,	8840,	7554,	6102,	4670,	3458,	2272,	1536,	843,	493,	281,	172,	61,	32,	10,	7,	1,	1,	1};}
    if (sample == 14)	{long File[] =	{145,	1187,	4057,	7885,	11951,	15271,	16243,	14903,	13195,	10616,	8213,	5751,	3894,	2535,	1554,	927,	511,	216,	110,	62,	26,	17,	6,	2,	0};}
    if (sample == 15)	{long File[] =	{7,	41,	143,	312,	522,	669,	663,	658,	560,	476,	365,	259,	174,	100,	90,	49,	16,	12,	11,	3,	1,	0,	0,	0,	0};}
    if (sample == 16)	{long File[] =	{0,	0,	0,	1,	2,	2,	1,	2,	1,	0,	2,	1,	0,	2,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0};}

    //BaselineSelection_WithBestPair_v9
    /*
    if (sample == 0)	{long File[] =	{90,	760,	2427,	4738,	7216,	8989,	9434,	8696,	7996,	5991,	4742,	3445,	2238,	1365,	829,	403,	276,	118,	59,	17,	12,	7,	5,	0,	0};}
    if (sample == 1)	{long File[] =	{75,	529,	1548,	3231,	4859,	6101,	6307,	5931,	5237,	4119,	3153,	2272,	1446,	980,	581,	297,	201,	84,	48,	25,	5,	0,	1,	0,	0};}
    if (sample == 2)	{long File[] =	{81,	799,	2629,	5365,	7978,	10176,	10839,	10056,	8609,	6874,	5398,	3821,	2660,	1668,	968,	621,	291,	173,	94,	37,	13,	5,	2,	0,	0};}
    if (sample == 3)	{long File[] =	{103,	847,	2848,	5623,	8527,	10358,	11076,	10203,	9072,	7212,	5644,	4093,	2699,	1724,	988,	606,	308,	168,	74,	49,	24,	9,	1,	0,	0};}
    if (sample == 4)	{long File[] =	{91,	613,	1860,	3908,	5769,	7083,	7563,	7195,	6309,	5161,	3790,	2696,	1814,	1244,	750,	442,	252,	118,	58,	31,	18,	5,	3,	3,	0};}
    if (sample == 5)	{long File[] =	{96,	718,	2340,	4616,	6656,	8600,	8842,	8492,	7502,	6003,	4403,	3263,	2112,	1385,	887,	495,	293,	162,	71,	35,	9,	7,	4,	0,	2};}
    if (sample == 6)	{long File[] =	{51,	401,	1272,	2544,	3864,	4877,	4992,	4729,	3976,	3360,	2578,	1832,	1201,	723,	450,	287,	142,	64,	37,	9,	7,	4,	0,	0,	1};}
    if (sample == 7)	{long File[] =	{25,	159,	539,	1074,	1607,	1950,	2103,	1873,	1654,	1357,	1014,	705,	496,	346,	176,	135,	65,	27,	12,	4,	3,	2,	0,	0,	0};}
    if (sample == 8)	{long File[] =	{0,	1,	6,	4,	17,	17,	21,	11,	15,	11,	7,	9,	5,	4,	2,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0};}
    if (sample == 9)	{long File[] =	{201,	1455,	4965,	9969,	15412,	19606,	20790,	19666,	16903,	13755,	10569,	7401,	5070,	3273,	1981,	1080,	648,	316,	151,	79,	35,	20,	10,	5,	0};}
    if (sample == 10)	{long File[] =	{3,	35,	119,	247,	392,	520,	529,	442,	435,	343,	285,	216,	126,	89,	48,	19,	22,	6,	2,	3,	1,	1,	0,	0,	0};}
    if (sample == 11)	{long File[] =	{3,	42,	149,	271,	410,	525,	510,	498,	444,	384,	251,	183,	149,	87,	42,	28,	10,	18,	3,	4,	1,	0,	0,	0,	0};}
    if (sample == 12)	{long File[] =	{54,	551,	1816,	3411,	5276,	6434,	7023,	6396,	5489,	4345,	3346,	2390,	1626,	1078,	608,	358,	213,	132,	43,	19,	8,	9,	3,	0,	0};}
    if (sample == 13)	{long File[] =	{52,	448,	1474,	3090,	4554,	5601,	6110,	5679,	4874,	3842,	2921,	2222,	1431,	970,	514,	285,	171,	109,	33,	14,	4,	4,	0,	1,	0};}
    if (sample == 14)	{long File[] =	{92,	774,	2748,	5340,	8124,	10440,	10939,	9995,	8955,	7187,	5412,	3819,	2597,	1668,	1046,	605,	312,	150,	74,	47,	14,	10,	6,	0,	0};}
    if (sample == 15)	{long File[] =	{3,	31,	101,	206,	350,	444,	456,	465,	388,	311,	234,	162,	119,	64,	55,	31,	11,	7,	6,	2,	1,	0,	0,	0,	0};}
    if (sample == 16)	{long File[] =	{0,	0,	0,	0,	1,	2,	0,	2,	1,	0,	2,	1,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0};}
    */
  }
  //  ** Variable -> <<<<<   pair_deltaR   >>>>>
  //    ** Bin edges:		{0,	0.5,	0.75,	1,	1.5,	2,	2.5,	3,	3.5,	4,	4.5,	5,	6};
  else if ( variable[v] == "deltaR" ) {
    //BaselineSelection_v9
    /*
    if (sample == 0)	{long File[] =	{166,	486,	1187,	5376,	8258,	9187,	19937,	31788,	0,	0,	0,	0};}
    if (sample == 1)	{long File[] =	{0,	4,	38,	218,	706,	2031,	10775,	33253,	2068,	14,	0,	0};}
    if (sample == 2)	{long File[] =	{0,	0,	14,	125,	635,	2169,	12713,	54269,	12583,	2149,	195,	4};}
    if (sample == 3)	{long File[] =	{0,	0,	0,	13,	131,	785,	7601,	59266,	16701,	4876,	1136,	117};}
    if (sample == 4)	{long File[] =	{0,	0,	0,	2,	30,	222,	3593,	41065,	13152,	4301,	1135,	145};}
    if (sample == 5)	{long File[] =	{0,	0,	0,	1,	8,	176,	3139,	46837,	17795,	6289,	1775,	233};}
    if (sample == 6)	{long File[] =	{0,	0,	0,	0,	1,	27,	1023,	23865,	11357,	5208,	1615,	191};}
    if (sample == 7)	{long File[] =	{0,	0,	0,	0,	0,	4,	161,	6182,	6116,	3764,	1601,	180};}
    if (sample == 8)	{long File[] =	{0,	6,	11,	11,	5,	10,	51,	92,	5,	0,	0,	0};}
    if (sample == 9)	{long File[] =	{1500,	5773,	7105,	17108,	25411,	43515,	74002,	56198,	10731,	2112,	303,	19};}
    if (sample == 10)	{long File[] =	{36,	98,	105,	326,	530,	1017,	1820,	1355,	240,	49,	8,	0};}
    if (sample == 11)	{long File[] =	{32,	77,	132,	295,	558,	1077,	1841,	1381,	224,	48,	2,	0};}
    if (sample == 12)	{long File[] =	{84,	212,	379,	1171,	2360,	7090,	20823,	21093,	4599,	988,	167,	18};}
    if (sample == 13)	{long File[] =	{446,	2091,	4611,	15497,	15345,	10540,	9746,	8165,	2109,	564,	99,	9};}
    if (sample == 14)	{long File[] =	{1227,	4043,	8708,	28622,	29331,	20166,	16116,	9416,	1324,	266,	52,	7};}
    if (sample == 15)	{long File[] =	{9,	40,	54,	185,	296,	610,	1780,	1796,	274,	65,	19,	3};}
    if (sample == 16)	{long File[] =	{0,	0,	1,	0,	0,	1,	1,	6,	5,	1,	0,	0};}
    */
    //BaselineSelection_WithBestPair_v9
    if (sample == 0)	{long File[] =	{77,	190,	537,	3273,	6515,	8463,	19509,	31290,	0,	0,	0,	0};}
    if (sample == 1)	{long File[] =	{0,	1,	12,	102,	416,	1602,	10242,	32612,	2029,	14,	0,	0};}
    if (sample == 2)	{long File[] =	{0,	0,	4,	32,	225,	1208,	10775,	52340,	12275,	2102,	192,	4};}
    if (sample == 3)	{long File[] =	{0,	0,	0,	0,	23,	239,	4910,	55531,	15692,	4653,	1091,	117};}
    if (sample == 4)	{long File[] =	{0,	0,	0,	0,	4,	42,	1826,	37593,	12119,	3995,	1059,	138};}
    if (sample == 5)	{long File[] =	{0,	0,	0,	1,	0,	32,	1404,	41959,	16007,	5726,	1651,	213};}
    if (sample == 6)	{long File[] =	{0,	0,	0,	0,	0,	8,	351,	20751,	10005,	4633,	1471,	182};}
    if (sample == 7)	{long File[] =	{0,	0,	0,	0,	0,	1,	43,	5164,	5268,	3251,	1433,	166};}
    if (sample == 8)	{long File[] =	{0,	1,	4,	6,	3,	6,	36,	72,	2,	0,	0,	0};}
    if (sample == 9)	{long File[] =	{786,	2819,	3747,	10197,	16132,	28216,	47498,	35816,	6664,	1294,	183,	10};}
    if (sample == 10)	{long File[] =	{21,	43,	66,	197,	376,	725,	1285,	957,	168,	40,	5,	0};}
    if (sample == 11)	{long File[] =	{20,	50,	77,	182,	383,	798,	1320,	986,	156,	39,	1,	0};}
    if (sample == 12)	{long File[] =	{54,	129,	223,	739,	1761,	5916,	18175,	18471,	4101,	891,	155,	14};}
    if (sample == 13)	{long File[] =	{137,	773,	2100,	9460,	11246,	7827,	6302,	4873,	1257,	359,	64,	5};}
    if (sample == 14)	{long File[] =	{508,	1746,	4606,	18813,	22131,	15522,	10651,	5589,	622,	134,	31,	2};}
    if (sample == 15)	{long File[] =	{3,	16,	23,	82,	160,	413,	1255,	1262,	175,	45,	13,	0};}
    if (sample == 16)	{long File[] =	{0,	0,	1,	0,	0,	1,	1,	4,	3,	0,	0,	0};}
  }
  /*
  //  ** Variable -> <<<<<   tkIso/pair_newTuneP_probe_pt   >>>>>
  //    *** Bin edges:{0,0.005,0.01,0.015,0.02,0.025,0.03,0.035,0.04,0.045,0.05,0.055,0.06,0.065,0.07,0.075,0.08,0.085,0.09,0.095,0.1,0.105,0.11};
  else if ( variable[v] == "TkIsoRel" ) {
    if (sample == 0){long File[] ={1039724,110521,86515,57402,39327,27931,20143,14522,10853,8313,6176,4856,3591,2987,2372,1959,1543,1248,1000,835,0,1};}
    if (sample == 1){long File[] ={41869,5324,3380,2100,1396,951,624,421,311,249,176,127,117,110,85,63,41,40,44,28,0,0};}
    if (sample == 2){long File[] ={78434,9622,4700,2381,1414,885,534,377,254,209,157,99,88,85,62,58,46,38,39,44,0,0};}
    if (sample == 3){long File[] ={97837,7324,2234,886,457,279,187,147,96,83,72,49,42,34,43,41,29,33,24,14,1,0};}
    if (sample == 4){long File[] ={77788,3646,891,366,194,135,91,69,56,39,40,38,27,33,28,24,18,13,15,19,1,0};}
    if (sample == 5){long File[] ={111372,3548,928,366,238,157,124,76,77,59,52,42,28,31,26,27,25,18,23,21,1,1};}
    if (sample == 6){long File[] ={102451,1864,499,221,127,104,71,58,58,64,39,37,27,18,25,19,18,9,18,10,0,0};}
    if (sample == 7){long File[] ={117960,1332,406,186,138,103,88,63,59,51,40,38,30,20,26,19,17,15,11,18,1,0};}
    if (sample == 8){long File[] ={281,0,1,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0};}
    if (sample == 9){long File[] ={262236,9137,5650,3575,2482,1706,1295,993,781,580,453,377,363,318,240,249,186,172,180,152,0,0};}
    if (sample == 10){long File[] ={6126,208,153,83,52,35,17,18,12,7,7,5,8,9,4,5,5,3,3,2,0,0};}
    if (sample == 11){long File[] ={6135,214,141,89,65,30,28,18,14,12,5,8,9,8,5,2,4,3,3,5,0,0};}
    if (sample == 12){long File[] ={65603,2827,1762,1037,739,460,305,221,152,131,80,82,52,59,47,43,28,22,21,25,0,0};}
    if (sample == 13){long File[] ={63648,5344,3436,2112,1481,965,660,499,367,290,238,152,130,126,94,70,53,61,62,33,0,0};}
    if (sample == 14){long File[] ={99107,10526,6919,4441,2954,1947,1479,1102,798,630,468,397,297,272,210,180,172,133,119,93,0,0};}
    if (sample == 15){long File[] ={8589,6,1,3,1,3,2,2,0,3,1,3,2,3,1,3,4,1,5,0,0,0};}
    if (sample == 16){long File[] ={451,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0};}
  }
  */
  else {
    cout << "WARNING: This macro recognizes only some variable names:" << endl;
    cout << "\t\"pt\", \"eta\", \"phi\", \"mass\" and \"vertex\"" << endl;
    cout << "Verify if their names are right." << endl;
    gSystem->Exit(0);
  }

  for ( Int_t bin_position = 0; bin_position < (sizeof(File) / sizeof(long)); bin_position++ ) {
    if ( bin_position == bin )
      entries_per_bin = File[bin_position];
  }
}
