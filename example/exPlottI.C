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

  Legend* l = new Legend(main, "Black Histo\n White Histo\n", "lp lp", "", 2,"l");
  Legend* ll = new Legend(l, "ll"); // copy first legend so that we can place it seperately

  TString info = TString("Black and White Histogram\n");
  info.Append("Example\n");
  TLegend* lInfo =  new Legend(info.Data(), 2);
  main->Add(lInfo);

  // --- Marker ----------------------------------------------------------------

  vector<Color_t> colors = {kBlack, kBlack, kGray+3};
  vector<Style_t> markers = {kFullCircle, kOpenCircle, kFullCircle};
  vector<Size_t>  sizes = {2., 2., 2.};

  // --- Canvasses -------------------------------------------------------------

  Legend::SetPosition(lInfo, 0.2, 0.3, 0.85, 0.75); // static function
  Legend::SetPosition(l, 0.43, 0.6, 0.2, 0.32);
  ll->SetPosition(0.43, 0.6, 0.05, 0.22); // non-static function

  SquarePlot square = SquarePlot(main, "x", "count");
  square.SetStyle(colors, markers, sizes);
  square.SetMode(Plot::Presentation);
  square.SetRanges(-3, 3, .1, 400);
  square.Draw(TString("Square.png"));
  // square.Draw(TString("Square.pdf"));

  main->AddBefore(lInfo, ll); //replace l in main with ll

  SingleRatioPlot rat = SingleRatioPlot(main, ratio, "x", "count", "ratio");
  rat.SetOffset(0, 2); // determines where the (ratio) entries start in the style arrays
  rat.SetRanges(-3, 3, -10, 400, 0.5, 3.2);
  rat.Draw(TString("Ratio.png"));
  // rat.Draw(TString("Ratio.pdf"));





}
