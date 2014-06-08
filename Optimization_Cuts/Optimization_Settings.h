// Provide directory path containing the input ROOT file
string input_directory =
  "/home/assantos/Physics/CERN/CMS/Muon_POG/Task_v15_OptimizationCuts_16May2014/InputFiles_v1_18May2014/";
// Provide the name of the input ROOT file
string input_file      = "Histograms_distribution_TP_ID_v17_BaselineSelections_v10_tag_MET10.root";

// Provide name for the output file which will contain the distributions
string output_name = "MET_baselineSelection_v10_08Jun2014";

double xMin   = 0;   // Starting point in X axis of the distribution
double xMax   = 500; // End point in X axis of the distribution
double step   = 5;   // Size of each bin of the distribution
double xLimit = 300; // End point in X axis to compute the highest significance
int    nBins  = (int)(xMax - xMin)/step; // Number of bins for the distribution
int    Nbins  = (xLimit - xMin)/step;    // Number of bins the curve of significance

static const int N = Nbins; // Number of bins the curve of significance

// In case of wanting to scale DY histo [DY->Scale(1/scale)] in curve other than "Punzi"
double scale       = 1.0;

// Settings for Drell-Yan and background distributions
int DY_color       = 4;
int DY_width       = 2;
int DY_fill_style  = 3005;

int Bkg_color      = 2;
int Bkg_width      = 2;
int Bkg_fill_style = 3004;

double yMin          = 0.001;
double yMax          = 800;
float y_title_offset = 1.5;
string histo_title   = "";
string y_title       = "Events";
string x_title       = "#slash{E}_{T} [GeV]";

// Provide information about the variable analyzed
string variable      = "#slash{E}_{T}";
string unit          = "[GeV]";

// Settings for the Pad
float leftMargin   = 0.12;
float rightMargin  = 0.08;
float bottomMargin = 0.12;
float topMargin    = 0.08;

// Computing uncertainties for curve of significance
// UpLow = 1 -> upper uncertainty
// UpLow = 0 -> lower uncertainty
// UpLow > 1 -> the lowest uncertainty
int UpLow = 2;

// Declaring and initializing values from significances
double punzi_x_in_maximum_y = 0;
double other_x_in_maximum_y = 0;
double punzi_maximum_y      = 0;
double other_maximum_y      = 0;

// Setting curves of significance
int    punzi_color         = 30;
int    other_color         = 46;
int    significance_width  = 2;
int    significance_size   = 0.6;
int    significance_marker = 20;
string significance_y      = "Significance"; // Text in Y axis
// Provide text for legends in the curve of significances
string punzi_y_label       = "#epsilon_{DY}/#left[1 + #sqrt{Bkg}#right]";
string other_y_label       = "DY/#sqrt{DY + Bkg}";


//=======================================================================================
//=======================================================================================
// This function clones DY and backg histograms,
// providing them to functions where the curves
// of significance will be created in.
void get_optimizations ( TH1F *DY, TH1F *Bkg, TGraphErrors* &punzi, TGraphErrors* &other ) {

  TH1F *hDY    = (TH1F*)DY->Clone("clone_DY");
  TH1F *hBkg   = (TH1F*)Bkg->Clone("clone_Dkg");

  // Create curve based on "Punzi" significance
  do_optimization ( hDY, hBkg, punzi, true  ); // "true" for "Punzi"
  // Create curve vased on "other" significance
  do_optimization ( hDY, hBkg, other, false ); // "false" for "other"
}


//=======================================================================================
//=======================================================================================
// This function takes the Drell-Yan and background
// histograms to create curves of significance.
// If "significance_type == true", significances based
// on "Punzi" significance is computed in.
// If "significance_type == false", significances based
// on "other" significance is computed in.
void do_optimization ( TH1F *DY, TH1F *Bkg, TGraphErrors* &curve, Bool_t significance_type ) {

  Int_t  bin                      = 0;
  Int_t  minBin                   = DY->FindBin(xMin);
  Int_t  maxBin                   = DY->FindBin(xMax);
  double total_DY_integral        = DY->Integral(minBin, maxBin);
  double total_Bkg_integral       = Bkg->Integral(minBin, maxBin);
  double error_total_DY_integral  = 0;
  double significance             = 0;
  double significance_error       = 0;
  double DY_integral              = 0;
  double Bkg_integral             = 0;

  // Arrows to record X and Y values of the significance.
  // Respective uncertainties are also recorded into arrays.
  double vx[N], vy[N], vex[N], vey[N];

  if ( significance_type  ) {
    cout << "*\t Creating punzi significance..." << endl;

    TH1F *numerator   = new TH1F("numerator_punzi"  , "numerator_punzi"  , ((xLimit - xMin)/step), xMin, xLimit);
    TH1F *denominator = new TH1F("denominator_punzi", "denominator_punzi", ((xLimit - xMin)/step), xMin, xLimit);
  }
  if ( !significance_type ) {
    cout << "*\t Creating other significance..." << endl;

    DY->Scale( 1.0 / scale );
    TH1F *numerator   = new TH1F("numerator_other"  , "numerator_other"  , ((xLimit - xMin)/step), xMin, xLimit);
    TH1F *denominator = new TH1F("denominator_other", "denominator_other", ((xLimit - xMin)/step), xMin, xLimit);
  }

  // Loop over the X axis in steps pre-defined by above and get integrals
  // of DY and backg histograms to compute values of significance
  for ( double upperCut = xMin + step;
	upperCut < (xLimit + ((double)step/2));
	upperCut = upperCut + step ) {

    double integral_error_DY  = 0;
    double integral_error_Bkg = 0;

    // To compute integral in the right range, cannot require
    // Integral(minBin, maxBin), but Integral(minBin, almost maxBin)
    maxBin             = DY->FindBin(upperCut - ((double)step/2));
    DY_integral        = DY->Integral(minBin, maxBin);
    integral_error_DY  = total_DY_integral  * Eff_Uncert ( DY_integral, total_DY_integral, UpLow );

    Bkg_integral       = Bkg->Integral(minBin, maxBin);
    integral_error_Bkg = total_Bkg_integral * Eff_Uncert ( Bkg_integral, total_Bkg_integral, UpLow );

    // Compute significances and their uncertainties, recording them into arrays
    if ( significance_type  ) {
      double efficiency       = (double)DY_integral / total_DY_integral;

      efficiency_error        = Eff_Uncert ( DY_integral, total_DY_integral, UpLow );
      significance            = efficiency / (1 + TMath::Sqrt(Bkg_integral));
      significance_error      = significance *
	                        TMath::Sqrt( ((double)efficiency_error*efficiency_error / (efficiency*efficiency)) +
					     ((double)integral_error_Bkg*integral_error_Bkg /
					      (4 * Bkg_integral * (1 + TMath::Sqrt(Bkg_integral)) *
					       (1 + TMath::Sqrt(Bkg_integral)))) );
    }
    if ( !significance_type ) {
      significance       = (double)DY_integral / TMath::Sqrt(DY_integral + Bkg_integral);
      significance_error = (double)DY_integral / (2*(DY_integral + Bkg_integral));
      significance_error = significance_error *
                           TMath::Sqrt( (integral_error_DY*integral_error_DY) +
					((double)integral_error_Bkg*integral_error_Bkg /
					 (DY_integral + Bkg_integral)) );
    }

    cout << "X: "                << DY->GetBinCenter(maxBin)
	 << "*\t maxBin: "       << maxBin << ",\t Upper Cut: " << upperCut
	 << ",\t DY: "           << DY_integral  << " +/- "     << integral_error_DY
	 << ",\t Bkg: "          << Bkg_integral << " +/- "     << integral_error_Bkg
	 << ",\t significance: " << significance << " +/- "     << significance_error << endl;

    vx[bin]  = (double)DY->GetBinCenter(maxBin);
    vy[bin]  = (double)significance;
    vex[bin] = (double)step/2;
    vey[bin] = (double)significance_error;

    // Get the positions in X and Y in the maximum of the significance
    if ( significance_type  ) {
      if ( significance > punzi_maximum_y ) {
	punzi_maximum_y      = significance;
	punzi_x_in_maximum_y = upperCut;
      }
    }
    if ( !significance_type ) {
      if ( significance > other_maximum_y ) {
	other_maximum_y      = significance;
	other_x_in_maximum_y = upperCut;
      }
    }

    bin = bin + 1;
  } // End of for( )

  cout << "*\t Number of bins: " << N << endl << "*" << endl << "*" << endl;

  // Finally, create the curve of significance
  curve = new TGraphErrors(N, vx, vy, vex, vey);
}


//=======================================================================================
//=======================================================================================
// This function computes uncertainties of efficiencies
Double_t Eff_Uncert ( Int_t pass, Int_t total, Int_t UpperLower ) {
  // Use Clopper-Pearson

  Int_t    fail    = total - pass;
  Double_t eff     = (double)pass/total;
  Double_t alpha   = (1.0 - 0.68540158589942957)/2;

  Double_t hi   = (fail == 0) ? 1.0 : ROOT::Math::beta_quantile(1-alpha, pass+1, fail);
  Double_t errU = hi - eff;

  Double_t lo   = (pass == 0) ? 0.0 : ROOT::Math::beta_quantile(alpha, pass, fail+1);
  Double_t errL = eff - lo;

  if      (UpperLower == 1) return errU; // return upper uncertainty if UpperLower == 1
  else if (UpperLower == 0) return errL; // return lower uncertainty if UpperLower == 0
  else return (errU>errL) ? errU : errL; // return the lowest value  if UpperLower  > 1

}
