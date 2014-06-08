#include <vector>
#include <math.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1.h>
#include <THStack.h>
#include <TLegend.h>
#include <TVectorD.h>
#include <TGraphErrors.h>
#include "Optimization_Settings.h"

void get_optimizations ( TH1F *, TH1F *, TGraphErrors* &, TGraphErrors* & );

void do_optimization ( TH1F *, TH1F *, TGraphErrors* &, Bool_t );

Double_t Eff_Uncert ( Int_t, Int_t, Int_t );

// Main function
void Optimization_Cut () {

  // Read input ROOT file
  stringstream InputFile;
  InputFile << input_directory << input_file;
  TFile *file = TFile::Open( InputFile.str().c_str() );

  vector <TH1F*> histo;

  TCanvas *canvas = new TCanvas("canvas", "canvas");
  canvas->cd();
  gPad->SetLogy();
  gStyle->SetOptStat(kFALSE);


  // Total number of histograms inside the input file 
  Int_t n = file->GetListOfKeys()->GetEntries();
  cout << "*\t Total number of histograms: " << n << endl << "*" << endl;;

  // Loop over the number of histograms (inside the input ROOT file)
  // and save them into TH1F vectors.
  for ( Int_t i = 0; i < n; i++ ) {
    string histo_name = file->GetListOfKeys()->At(i)->GetName();
    cout << "Histo name: " << histo_name << endl;
    histo.push_back( new TH1F() );
    histo[i] = (TH1F*)file->Get( histo_name.c_str() );
  }

  // Histograms to add DY and background samples in stacks
  TH1F *histo_DY  = new TH1F("histo_DY" , "histo_DY" , nBins, xMin, xMax);
  TH1F *histo_Bkg = new TH1F("histo_Bkg", "histo_Bkg", nBins, xMin, xMax);

  for ( Int_t i = 1; i < 9; i++ )  histo_DY->Add(  histo[i] ); // Add DY samples in stack
  for ( Int_t i = 9; i < n; i++ )  histo_Bkg->Add( histo[i] ); // Add bkg samples in stack

  histo_DY->SetTitle( histo_title.c_str() );
  histo_DY->GetYaxis()->SetTitle( y_title.c_str() );
  histo_DY->GetXaxis()->SetTitle( x_title.c_str() );
  histo_DY->SetLineColor(DY_color);
  histo_DY->SetLineWidth(DY_width);
  histo_DY->SetFillColor(DY_color);
  histo_DY->SetFillStyle(DY_fill_style);

  histo_Bkg->SetLineColor(Bkg_color);
  histo_Bkg->SetLineWidth(Bkg_width);
  histo_Bkg->SetFillColor(Bkg_color);
  histo_Bkg->SetFillStyle(Bkg_fill_style);

  // Drell-Yan and background histograms are superimposed
  histo_DY->Draw("histo");       // Draw DY first
  histo_Bkg->Draw("histo same"); // Then draw background
  histo[0]->Draw("ep same");     // And finally draw data

  //======================================================================================
  // This canvas will show curves of significance
  TCanvas *canvas_significance = new TCanvas("canvas_significance", "canvas_significance");
  canvas_significance->cd();
  gPad->SetLogy();
  gPad->SetGrid();
  gPad->SetTicks();
  gPad->SetMargin(leftMargin, rightMargin, bottomMargin, topMargin);

  TMultiGraph  *mg    = new TMultiGraph();
  TGraphErrors *punzi = 0;
  TGraphErrors *other = 0;

  // Provide DY and Bkg histograms and get curves of significance
  get_optimizations ( histo_DY, histo_Bkg, punzi, other );

  cout << "*\t Punzi:\t x = " << punzi_x_in_maximum_y
       << "\t ->\t maximum y = " << (double)punzi_maximum_y << endl;
  cout << "*\t Other:\t x = " << other_x_in_maximum_y
       << "\t ->\t maximum y = " << (double)other_maximum_y << endl;

  punzi->SetLineColor(punzi_color);
  other->SetLineColor(other_color);
  punzi->SetMarkerColor(punzi_color);
  other->SetMarkerColor(other_color);
  punzi->SetLineWidth(significance_width);
  other->SetLineWidth(significance_width);
  punzi->SetMarkerSize(significance_size);
  other->SetMarkerSize(significance_size);
  punzi->SetMarkerStyle(significance_marker);
  other->SetMarkerStyle(significance_marker);

  mg->SetTitle( histo_title.c_str() );

  mg->Add(punzi);
  mg->Add(other);

  mg->Draw("APC");

  // In TMultiGraph, X and Y axis have to be set up after being drawn
  mg->GetXaxis()->SetTitle( x_title.c_str() );
  mg->GetYaxis()->SetTitle( significance_y.c_str() );
  mg->GetYaxis()->SetTitleOffset( y_title_offset );
  mg->GetYaxis()->SetRangeUser( yMin, yMax );

  // Draw an arrow to show where is the optimal point for this curve of significance
  TArrow *punzi_line = new TArrow( punzi_x_in_maximum_y, punzi->GetHistogram()->GetMaximum(),
				   punzi_x_in_maximum_y, punzi->GetHistogram()->GetMinimum(), 0.02 );
  punzi_line->SetLineColor(punzi_color);
  punzi_line->SetLineWidth(significance_width);
  punzi_line->Draw();

  // Draw an arrow to show where is the optimal point for this curve of significance
  TArrow *other_line = new TArrow( other_x_in_maximum_y, other->GetHistogram()->GetMaximum(),
				   other_x_in_maximum_y, punzi->GetHistogram()->GetMinimum(), 0.02 );
  other_line->SetLineColor(other_color);
  other_line->SetLineWidth(significance_width);
  other_line->Draw();

  gPad->Modified();

  stringstream labels;
  labels.str("");

  TLegend *l2 = new TLegend(0.4, 0.55, 0.91, 0.7);
  l2->SetBorderSize(1);
  l2->SetFillColor(0);
  labels << punzi_y_label << ", Maximum at " << variable << " = " << punzi_x_in_maximum_y << " " << unit;
  l2->AddEntry(punzi , labels.str().c_str(), "pl");
  labels.str("");
  labels << other_y_label << ", Maximum at " << variable << " = " << other_x_in_maximum_y << " " << unit;;
  l2->AddEntry(other , labels.str().c_str(), "pl");
  l2->Draw();



  canvas->cd();

  // Draw an arrow to show where is the optimal point for this curve of significance
  punzi_line = new TArrow( punzi_x_in_maximum_y, histo_DY->GetMaximum(),
			   punzi_x_in_maximum_y, histo_DY->GetMinimum(), 0.02 );
  punzi_line->SetLineColor(punzi_color);
  punzi_line->SetLineWidth(significance_width);
  punzi_line->Draw();

  // Draw an arrow to show where is the optimal point for this curve of significance
  other_line = new TArrow( other_x_in_maximum_y, histo_DY->GetMaximum(),
			   other_x_in_maximum_y, histo_DY->GetMinimum(), 0.02 );
  other_line->SetLineColor(other_color);
  other_line->SetLineWidth(significance_width);
  other_line->Draw();

  TLegend *l = new TLegend(0.5, 0.6, 0.89, 0.89);
  l->SetBorderSize(0);
  l->SetFillColor(0);
  l->AddEntry(histo[0]  , "Data RunC, 7.05 fb^{-1}" , "pl");
  l->AddEntry(histo_DY  , "All Drell-Yan Samples"   , "lf");
  l->AddEntry(histo_Bkg , "All Background Samples"  , "lf");
  labels.str("");
  labels << punzi_y_label << ", " << variable
	 << " = " << punzi_x_in_maximum_y << " " << unit;
  l->AddEntry(punzi_line, labels.str().c_str()     , "l");
  labels.str("");
  labels << other_y_label << ", " << variable
	 << " = " << other_x_in_maximum_y << " " << unit;
  l->AddEntry(other_line, labels.str().c_str()     , "l");
  l->Draw();

  // Saving canvas of distributions
  labels.str("");
  labels << output_name << ".png";
  canvas->SaveAs(labels.str().c_str());
  labels.str("");
  labels << output_name << ".pdf";
  canvas->SaveAs(labels.str().c_str());
  labels.str("");
  labels << output_name << ".root";
  canvas->SaveAs(labels.str().c_str());

  // Saving canvas of significances
  labels.str("");
  labels << "Significance_" << output_name << ".png";
  canvas_significance->SaveAs(labels.str().c_str());
  labels.str("");
  labels << "Significance_" << output_name << ".pdf";
  canvas_significance->SaveAs(labels.str().c_str());
  labels.str("");
  labels << "Significance_" << output_name << ".root";
  canvas_significance->SaveAs(labels.str().c_str());
}
