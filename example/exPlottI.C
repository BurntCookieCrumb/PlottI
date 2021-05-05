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

  // -------------------------------------------------------------------------
  //            Example 1: Basic Interface Usage
  // -------------------------------------------------------------------------

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
  square.Draw("Square.png");
  // square.Draw(TString("Square.pdf"));

  main->AddBefore(lInfo, ll); //replace l in main with ll

  SingleRatioPlot rat = SingleRatioPlot(main, ratio, "x", "count", "ratio");
  rat.SetOffset(0, 2); // determines where the (ratio) entries start in the style arrays
  rat.SetRanges(-3, 3, -10, 400, 0.5, 3.2);
  rat.Draw("Ratio.png");
  // rat.Draw(TString("Ratio.pdf"));

  RatioPlot just_the_ratio = RatioPlot(ratio, "x", "ratio");
  just_the_ratio.SetOffset(2);
  just_the_ratio.SetRanges(-3, 3, 0.5, 3.2);
  just_the_ratio.Draw("just_ratio.png");

  // -------------------------------------------------------------------------
  //            Example 1.1: Heatmaps
  // -------------------------------------------------------------------------

  TH2I* heat = new TH2I("heat", "", 20, 1, 20, 20, 1, 20);

  for (Int_t binx = 1; binx < 20; binx++){
    for (Int_t biny = 1; biny < 20; biny++){
      heat->SetBinContent(binx, biny, binx + biny + binx*biny);
    }
  }

  Legend* legend = new Legend("This is a heatmap!\n", 1);
  legend->SetPosition(0.2, 0.75, 0.8, 0.87);

  TObjArray* heatArray = new TObjArray();
  heatArray->Add(heat);
  heatArray->Add(legend);

  HeatMapPlot heatMap = HeatMapPlot(heatArray, "X", "Y", "Z");
  heatMap.SetRanges(0, 20, 0, 20, 1, 1E3);
  heatMap.SetPalette(kPastel, kTRUE);
  heatMap.SetLog(kFALSE, kFALSE, kTRUE);
  heatMap.Draw("heat.png");


  // -------------------------------------------------------------------------
  //            Example 2: Userdefined Colors
  // -------------------------------------------------------------------------

  vector<Style_t> markersPal (20, kFullCircle);
  vector<Size_t>  sizesPal   (20, 3.);

  vector<color> rgbRainbow = {blue, green, red};
  ColorGradient rgb_rainbow = ColorGradient(20, rgbRainbow);

  vector<Double_t> spacing = {0., 0.8, 1.};
  ColorGradient blue_green_somered = ColorGradient(20, rgbRainbow, spacing);

  vector<TH1I*> palette(20);
  TObjArray* pal = new TObjArray();

  palette[0] = new TH1I("palette", "", 20, 1, 20);
  pal->Add(palette[0]);

  for (Int_t bin = 1; bin < palette[0]->GetNbinsX(); bin++){
    palette[0]->SetBinContent(bin, 1);
    palette[0]->SetBinError(bin, 0.00001);
  }

  for (Int_t hist = 1; hist < 20; hist++){
    palette[hist] = (TH1I*)palette[0]->Clone(Form("palette_%d", hist));
    palette[hist]->Scale(hist+1);
    pal->Add(palette[hist]);
  }

  // extract color vector from \ref ColorGradient
  vector<Color_t> colors_rainbow = rgb_rainbow.GetGradient();

  SquarePlot direct_usage = SquarePlot(pal, "", "");
  direct_usage.SetOffset(0);
  direct_usage.SetMode(Plot::Presentation);
  direct_usage.SetStyle(colors_rainbow, markersPal, sizesPal);
  direct_usage.SetRanges(0, 20, 0, 21);
  direct_usage.SetOptions("SAME");
  direct_usage.Draw("direct.png");

  SquarePlot palette_usage = SquarePlot(pal, "", "");
  palette_usage.SetRanges(0, 20, 0, 21);
  palette_usage.SetPalette(blue_green_somered, kTRUE);
  palette_usage.SetOptions("SAME PMC PLC PFC");
  palette_usage.Draw("palette.png");

  std::vector<Color_t> RGB_Colors = {blue.index, red.index, green.index};

  TObjArray* indices = new TObjArray();
  indices->Add(palette[0]);
  indices->Add(palette[1]);
  indices->Add(palette[2]);

  SquarePlot indices_usage = SquarePlot(indices, "", "");
  indices_usage.SetStyle(RGB_Colors, markersPal, sizesPal);
  indices_usage.SetRanges(0, 20, 0, 4);
  indices_usage.Draw("indices.png");

  // -------------------------------------------------------------------------
  //            Example 3: Definition of Legends
  // -------------------------------------------------------------------------

  // Legend from TObjArrays

  TLegend* lIndices = new Legend(indices, "First Histogram\n Second\n and Third", "lp lp lp", "", 3,"indices");
  indices->Add(lIndices);

  // dummy markers
  std::string marker_information = Form("%d %d %f \n %d %d %f \n %d %d %f", kBlack, kFullCircle, 3., kBlue+2, kFullSquare, 3., kMagenta+2, kFullDiamond, 4.);
  TLegend* lDummyMarkers = new Legend(marker_information.data(), "Black Full Circle\n Blue Full Square\n Magenta Full Diamond", "lp p lp", 3, "lDummyMarkers");
  indices->Add(lDummyMarkers);

  // information
  TLegend* lInformation = new Legend("Some information \n about your histograms \n or your data", 3, "lInformation");
  indices->Add(lInformation);

  // placing the legends
  Legend::SetPosition(lIndices, 0.45, 0.8, 0.4, 0.55);
  Legend::SetPosition(lDummyMarkers, 0.45, 0.8, 0.6, 0.75);
  Legend::SetPosition(lInformation, 0.2, 0.21, 0.77, 0.87);

  // plot them
  SquarePlot legends = SquarePlot(indices, "", "");
  legends.SetRanges(0, 20, 0, 21);
  legends.SetPalette(kAvocado);
  legends.SetOptions("SAME PMC PLC PFC");
  legends.Draw("legends.png");


}
