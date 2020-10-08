// ~~ PLOTTING ~~

// -----------------------------------------------------------------------------
// Create example plots to demonstrate functionality of PlottI
//
// -----------------------------------------------------------------------------

// == Includes ==

#include "../Plot.h"

// == Namespace ==

// -----------------------------------------------------------------------------
// plotting
// -----------------------------------------------------------------------------

void exPlottI(){

  // --- Define Histograms -----------------------------------------------------

  TH1D* black = new TH1D("black", "", 100, -3, 3);
  black->Sumw2();
  black->FillRandom("gaus", 10000);

  TH1D* white = new TH1D("white", "", 100, -3, 3);
  white->Sumw2();
  white->FillRandom("gaus", 10000);

  TH1D* grey = (TH1D*)black->Clone("grey");
  grey->Divide(white);

  // --- Create TObjArrays -----------------------------------------------------

  TObjArray* main = new TObjArray();
  main->Add(black);
  main->Add(white);

  TObjArray* ratio = new TObjArray();
  ratio->Add(grey);

  // --- Legends ---------------------------------------------------------------

  TLegend* l = Legend(main, "Black Histo\n White Histo\n", "lp lp").GetLegendPointer();

  TString info = TString("Black and White Histogram\n");
  info.Append("Example\n");
  TLegend* lInfo = Legend(info.Data(), 2).GetLegendPointer();
  main->Add(lInfo);

  // --- Marker ----------------------------------------------------------------

  vector<Color_t> colors = {kBlack, kBlack, kGray+3};
  vector<Style_t> markers = {kFullCircle, kOpenCircle, kFullCircle};
  vector<Size_t>  sizes = {2., 2., 2.};

  // --- Canvasses -------------------------------------------------------------

  Legend::SetPosition(lInfo, 0.2, 0.3, 0.85, 0.75);
  Legend::SetPosition(l, 0.43, 0.6, 0.2, 0.32);

  SquarePlot square = SquarePlot(main, "x", "count");
  square.SetStyle(colors, markers, sizes);
  square.SetMode(Plot::Presentation);
  square.SetRanges(-3, 3, .1, 400);
  square.Draw(TString("Square.png"));

  SingleRatioPlot rat = SingleRatioPlot(main, ratio, "x", "count", "ratio");
  rat.SetOffset(2); // determines where the ratio entries start in the style arrays
  rat.SetRanges(-3, 3, -10, 400, 0.5, 3.2);
  rat.Draw(TString("Ratio.png"));





}
