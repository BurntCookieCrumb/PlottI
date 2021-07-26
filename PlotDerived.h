
// ~~ PlotTING CLASS ~~

// ----------------------------------------------------------------------------
//
// This file contains the derived classes for the plotting functionalities
// Currently the following derived classes are available:
//  - SquarePlot: Simple Plot in Square formmat with one pad
//  - RatioPlot: Simple Plot in Rectangle format with one pad that is meant
//               to display Ratios
//  - SingleRatioPlot: Rectangle Plot with two pads. The upper pads are for the
//                     distributions while the lower pad is for the
//                     corresponding ratios
//  - HeatMapPlot: Simple Plot in Square format for drawing one TH2 (heatmap)
//                 and one corresponding legend
//
// ----------------------------------------------------------------------------


#define DERIVED_H

// ----------------------------------------------------------------------------
//                              SQUARE PLOT CLASS
// ----------------------------------------------------------------------------

//! Class for a simple square-format plot
class SquarePlot : public Plot
{

public:

  SquarePlot(TObjArray* array, TString xTitle, TString yTitle);
  virtual ~SquarePlot() {}

  /*virtual*/ void Draw(TString outname);

private:

  TObjArray* plotArray;       //!< Array containing all objects to be plotted

};

// ---- Constructor -----------------------------------------------------------

//! Constructor
SquarePlot::SquarePlot(TObjArray* array, TString xTitle, TString yTitle): Plot(xTitle, yTitle),
  plotArray(array)
{

  EnsureAxes(array->At(0), "Main Array");

  SetCanvasDimensions(1000, 1000);
  SetCanvasMargins(0.13, 0.03, 0.05, 0.13);
  SetCanvasOffsets(1.3, 1.5);

  options = std::vector<std::string> (array->GetEntries(), "SAME");

}

// ---- Member Functions ------------------------------------------------------

void SquarePlot::Draw(TString outname){

  /** Main function for Drawing **/

  std::cout << "-----------------------------" << std::endl;
  std::cout << "     Plot Square Canvas:" << std::endl;
  std::cout << "-----------------------------" << std::endl;

  if (broken){
    std::cout << "Due to one or more \033[1;33mFATAL ERRORS\033[0m your Plot will not be drawn" << std::endl;
    std::cout << "-----------------------------" << std::endl << std::endl;
    return;
  }

  canvas  = new TCanvas("canvas", "SQUARE", 10, 10, width+10, height+10);
  canvas->cd();

  mainPad = new TPad("mainPad", "Distribution", 0, 0, 1, 1);
  SetUpPad(mainPad, logX, logY);
  SetUpStyle(plotArray->At(0), titleX, titleY, xRangeUp, xRangeLow, yRangeUp, yRangeLow);
  mainPad->Draw();
  mainPad->cd();

  DrawArray(plotArray, mOffset);

  canvas->Update();
  canvas->SaveAs(outname.Data());
  delete canvas;

  std::cout << "-----------------------------" << std::endl << std::endl;

}

// ----------------------------------------------------------------------------
//                              RATIO ONLY PLOT CLASS
// ----------------------------------------------------------------------------

//! Class for a rectangle plot displaying ratios

class RatioPlot : public Plot
{

public:

  RatioPlot(TObjArray* rArray, TString xTitle, TString yTitle);
  virtual ~RatioPlot() {};

  /*virtual*/ void Draw(TString outname);
  /*virtual*/ void DrawRatioArray(TObjArray* array, Int_t off, Int_t offOpt = 0);
  void SetUpperOneLimit(Double_t up);
  void ToggleOne() {drawone = !drawone;};

protected:

  TObjArray* plotArray;      //!< Array containing all objects to be plotted

  TLine*   one {nullptr};    //!< Horizontal TLine which will be included to every ratio at height 1
  Double_t oneUp {0};        //!< Upper bound on x-Range for TLine one

  Bool_t drawone {kTRUE};

};

// ---- Constructor -----------------------------------------------------------

//! Constructor
RatioPlot::RatioPlot(TObjArray* rArray, TString xTitle, TString yTitle) : Plot(xTitle, yTitle),
  plotArray(rArray)
{

  EnsureAxes(rArray->At(0), "Main Array");

  SetCanvasDimensions(1000, 600);
  SetCanvasMargins(0.15, 0.03, 0.05, 0.2);
  SetCanvasOffsets(1.1, 1.);

  options = std::vector<std::string>(rArray->GetEntries(), "SAME");

}

// ---- Member Functions ------------------------------------------------------

void RatioPlot::Draw(TString outname){

  /** Main function for Drawing **/

  std::cout << "-----------------------------" << std::endl;
  std::cout << "     Plot Ratio Canvas:" << std::endl;
  std::cout << "-----------------------------" << std::endl;

  if (broken){
    std::cout << "Due to one or more \033[1;33mFATAL ERRORS\033[0m your Plot will not be drawn" << std::endl;
    std::cout << "-----------------------------" << std::endl << std::endl;
    return;
  }

  canvas  = new TCanvas("canvas", "RATIO", /*10*/0, /*10*/0, width/*+10*/, height/*+10*/);
  canvas->cd();

  mainPad = new TPad("mainPad", "Ratio", 0, 0, 1, 1);
  SetUpPad(mainPad, logX, logY);
  SetUpStyle(plotArray->At(0), titleX, titleY, xRangeUp, xRangeLow, yRangeUp, yRangeLow);
  mainPad->Draw();
  mainPad->cd();

  DrawRatioArray(plotArray, mOffset);

  canvas->Update();
  canvas->SaveAs(outname.Data());
  delete canvas;

  std::cout << "-----------------------------" << std::endl << std::endl;

}

void RatioPlot::DrawRatioArray(TObjArray* array, Int_t off, Int_t offOpt){

  /** Draws a single Ratio TObjArray in the chosen Pad **/

  if (drawone){
    one = new TLine(xRangeLow, 1., (oneUp ? oneUp : xRangeUp), 1.);
    array->Add(one);
    options.push_back("SAME");
    SetLineProperties(one, kBlack, 9, 3.);
  }

  DrawArray(array, off, offOpt);


}

void RatioPlot::SetUpperOneLimit(Double_t up){

  /** Sets upper limit on line in ratio at value one **/

  oneUp = up;

}

// ----------------------------------------------------------------------------
//                         SINGLE RATIO PLOT CLASS
// ----------------------------------------------------------------------------

//! Class for a rectangle plot with the distributions in the upper pad and the ratios in the lower pad

class SingleRatioPlot : public RatioPlot
{

public:

  SingleRatioPlot(TObjArray* mainArray, TObjArray* ratioArray, TString xTitle, TString yTitle, TString ratioTitle);
  virtual ~SingleRatioPlot() {};

  /*virtual*/ void Draw(TString outname);

  void SetPadFraction(Double_t frac);
  void SetCanvasOffsets(Float_t xOffset, Float_t yOffset, Float_t rOffset = 0);
  void SetOffset(Int_t off, Int_t roff);
  virtual void SetRanges(Float_t xLow, Float_t xUp, Float_t yLow, Float_t yUp, Float_t rLow, Float_t rUp);
  virtual void SetOptions(std::string optns, std::string postns);

private:

  static Float_t padFrac;        //!< Fraction of the Canvas used for the ratio pad

  TPad* ratioPad {nullptr};      //!< Pad containing the ratio plot
  TString ratioTitle;            //!< Title for Y-axis of Ratios

  TObjArray* ratioArray;         //!< Array containing all ratios to be plotted

  Float_t offsetR {0.};          //!< Offset for Y-Title of the ratio
  Float_t rRangeUp {1.2};        //!< Upper Y-axis range of the ratio
  Float_t rRangeLow {0.8};       //!< Lower Y-axis range of the ratio

  static Int_t rOffset;          //!< Offset for index of ratio objects in style arrays

};

// ---- Static Member Variables -----------------------------------------------

Float_t SingleRatioPlot::padFrac {.3}; //0.35
Int_t   SingleRatioPlot::rOffset  {1};

// ---- Cunstructor -----------------------------------------------------------

//! Constructor
SingleRatioPlot::SingleRatioPlot(TObjArray* mainArray, TObjArray* rArray, TString xTitle, TString yTitle, TString rTitle) : RatioPlot(mainArray, xTitle, yTitle),
  ratioTitle(rTitle),
  ratioArray(rArray)
{

  EnsureAxes(mainArray->At(0), "Main Array");
  EnsureAxes(rArray->At(0), "Ratio Array");

  SetCanvasDimensions(1000, 1200);
  SetCanvasMargins(0.13, 0.03, 0.05, 0.3);
  SetCanvasOffsets(4., 2., 2.);

  options = std::vector<std::string>(mainArray->GetEntries()+rArray->GetEntries(), "SAME");

}

// ---- Member Functions ------------------------------------------------------

void SingleRatioPlot::Draw(TString outname){

  /** Main function for Drawing **/

  std::cout << "-----------------------------" << std::endl;
  std::cout << "  Plot Single Ratio Canvas:  " << std::endl;
  std::cout << "-----------------------------" << std::endl;

  if (broken){
    std::cout << "Due to one or more \033[1;33mFATAL ERRORS\033[0m your Plot will not be drawn" << std::endl;
    std::cout << "-----------------------------" << std::endl << std::endl;
    return;
  }

  canvas  = new TCanvas("canvas", "SINGLE RATIO", 10, 10, width+10, height+10);
  canvas->cd();

  mainPad = new TPad("mainPad", "Distribution", 0, padFrac, 1, 1);
  SetUpPad(mainPad, logX, logY);
  SetUpStyle(plotArray->At(0), "", titleY, xRangeUp, xRangeLow, yRangeUp, yRangeLow);
  ((TH1*)plotArray->At(0))->GetXaxis()->SetLabelSize(0);
  ((TH1*)plotArray->At(0))->GetXaxis()->SetLabelColor(kWhite);
  mainPad->SetBottomMargin(0.);
  mainPad->Draw();

  ratioPad = new TPad("ratioPad", "Ratio", 0, 0, 1, padFrac);
  SetUpPad(ratioPad, logX, kFALSE);
  ratioPad->SetTopMargin(0.);
  SetUpStyle(ratioArray->At(0), titleX, ratioTitle, xRangeUp, xRangeLow, rRangeUp, rRangeLow);
  ((TH1*)ratioArray->At(0))->GetYaxis()->SetTitleOffset(offsetR);
  ratioPad->Draw();

  mainPad->cd();
  DrawArray(plotArray, mOffset);
  ratioPad->cd();
  DrawRatioArray(ratioArray, rOffset, plotArray->GetEntries());

  canvas->Update();
  canvas->SaveAs(outname.Data());
  delete canvas;

  std::cout << "-----------------------------" << std::endl << std::endl;

}

void SingleRatioPlot::SetCanvasOffsets(Float_t xOffset, Float_t yOffset, Float_t rOffset){

  /** Set the Title Offsets **/

  offsetX = xOffset;
  offsetY = yOffset;
  offsetR = rOffset ? rOffset : yOffset;

}

void SingleRatioPlot::SetPadFraction(Double_t frac){

  /** Sets Fraction of the Pad taken by the Ratio **/

  padFrac = frac;

}

void SingleRatioPlot::SetOffset(Int_t off, Int_t roff){

  /** Sets the offset of the ratio style properties,
  namely where the ratio markers are in the marker, color and size arrays **/

  mOffset = off;
  rOffset = roff;

}

void SingleRatioPlot::SetRanges(Float_t xLow, Float_t xUp, Float_t yLow, Float_t yUp, Float_t rLow, Float_t rUp){

  /** Sets the Ranges for the Pads **/

  xRangeUp  = xUp;
  xRangeLow = xLow;
  yRangeUp  = yUp;
  yRangeLow = yLow;
  rRangeUp  = rUp;
  rRangeLow = rLow;

  ranges = kTRUE;

}

void SingleRatioPlot::SetOptions(std::string optns, std::string postns){

  /** Set the plot options for a few specific plottjects,
      mind that any legend or pave object is also included in the options
      \b \c ; will seperate different arrays **/

  std::istringstream options(optns);
  std::istringstream positions(postns);

  TString* opt = new TString();
  TString* pos = new TString();

  opt->ReadToDelim(options, ';');
  pos->ReadToDelim(positions, ';');
  SetOption(opt->Data(), pos->Atoi());

  opt->ReadLine(options);
  pos->ReadLine(positions);
  Plot::SetOptions(opt->Data(), pos->Data(), plotArray->GetEntries());

}


// ----------------------------------------------------------------------------
//                         HEAT MAP PLOT CLASS
// ----------------------------------------------------------------------------

//! Class for plotting a single TH2 (heatmap) and one corresponding legend

class HeatMapPlot : public Plot
{

public:
  HeatMapPlot(TObjArray* plotArray, TString xTitle, TString yTitle, TString zTitle = "count");
  HeatMapPlot(TH2* map, TLegend* l, TString xTitle, TString yTitle, TString zTitle = "count");
  ~HeatMapPlot() {};

  void Draw(TString outname);

  void SetProperties(TH2* map, std::string title = "");
  void SetCanvasOffsets(Float_t xOffset, Float_t yOffset, Float_t zOffset);
  /*virtual*/ void SetLog(Bool_t xLog = kFALSE, Bool_t yLog = kTRUE, Bool_t zLog = kFALSE);
  virtual void SetRanges(Float_t xLow, Float_t xUp, Float_t yLow, Float_t yUp, Float_t zLow, Float_t zUp);


private:

  void EnsureTH2(TObject* first, std::string arrayName);

  void SetCanvasStyle(TH2* first);
  void SetPadStyle(TH2* first, TString xTitle, TString yTitle, TString zTitle, Float_t xUp, Float_t xLow, Float_t yUp, Float_t yLow, Float_t zUp, Float_t zLow);
  void SetUpPad(TPad* pad, Bool_t xLog, Bool_t yLog, Bool_t zLog = kFALSE);


  TString titleZ {"count"};       //!< Title of Z-axis

  Float_t offsetZ {0};            //!< Offset of Z title
  Bool_t  logZ {kFALSE};          //!< Should Z-axis be logarithmic?
  Float_t zRangeUp {0};           //!< Upper Z-axis range
  Float_t zRangeLow {0};          //!< Lower Z-axis range

  TObjArray *plotArray {nullptr};      //!< Legend corresponding to the heatmap

};

// ---- Cunstructor -----------------------------------------------------------

//! Constructor
HeatMapPlot::HeatMapPlot(TObjArray* array, TString xTitle, TString yTitle, TString zTitle) : Plot(xTitle, yTitle),
titleZ(zTitle),
plotArray(array)
{

  EnsureTH2(array->At(0), "Heatmap Array");

  SetCanvasDimensions(1000, 850);
  SetCanvasMargins(0.13, 0.2, 0.05, .13);
  SetCanvasOffsets(1.3, 1.4, 1.4);

  options = std::vector<std::string>(array->GetEntries(), "SAME");
  options[0] = "SAME COLZ";

}

//! Constructor
HeatMapPlot::HeatMapPlot(TH2* map, TLegend* l, TString xTitle, TString yTitle, TString zTitle) : Plot(xTitle, yTitle),
titleZ(zTitle)
{

  // EnsureTH2(array->At(0), "Heatmap Array");
  plotArray = new TObjArray();
  plotArray->Add(map);
  plotArray->Add(l);

  SetCanvasDimensions(1000, 850);
  SetCanvasMargins(0.13, 0.2, 0.05, .13);
  SetCanvasOffsets(1.3, 1.5, 1.5);

  options = {"SAME COLZ", "SAME"};

}

// ---- Member Functions ------------------------------------------------------

void HeatMapPlot::Draw(TString outname){

  /** Main function for Drawing **/

  std::cout << "-----------------------------" << std::endl;
  std::cout << "     Plot Heat Map:" << std::endl;
  std::cout << "-----------------------------" << std::endl;

  if (broken){
    std::cout << "Due to one or more \033[1;33mFATAL ERRORS\033[0m your Plot will not be drawn" << std::endl;
    std::cout << "-----------------------------" << std::endl << std::endl;
    return;
  }

  canvas  = new TCanvas("canvas", "HEATMAP", 10, 10, width+10, height+10);
  canvas->cd();

  mainPad = new TPad("mainPad", "Distribution", 0, 0, 1, 1);
  SetUpPad(mainPad, logX, logY, logZ);
  SetCanvasStyle((TH2*)plotArray->At(0));
  SetPadStyle((TH2*)plotArray->At(0), titleX, titleY, titleZ, xRangeUp, xRangeLow, yRangeUp, yRangeLow, zRangeUp, zRangeLow);
  mainPad->Draw();
  mainPad->cd();

  DrawArray(plotArray, 0);

  canvas->Update();
  canvas->SaveAs(outname.Data());
  delete canvas;

  std::cout << "-----------------------------" << std::endl << std::endl;

}

void HeatMapPlot::EnsureTH2(TObject* first, std::string arrayName){

  if (!first) {

    std::cout << "\033[1;33mFATAL ERROR:\033[0m First entry in array doesn't exist!!" << std::endl;
    broken = kTRUE;
    return;

  }

  if (!first->InheritsFrom("TH2")){

    std::cout << "\033[1;33mFATAL ERROR:\033[0m First entry in array must be a TH2 "
    << "\033[1;36m(" << arrayName << ")\033[0m" << std::endl;
    std::cout << "             In case you don't want to draw a TH2, please use SquarePlot." << std::endl;
    broken = kTRUE;
    return;

  }

}

void HeatMapPlot::SetProperties(TH2* map, std::string title){

  /** Manages internal setting of properties for TH2 heatmap plots **/

  map->SetContour(100);
  map->SetStats(kFALSE);
  map->SetTitle(title.data());

}

void HeatMapPlot::SetCanvasStyle(TH2* first){

  /** Set general style features of the Canvas and Pads **/

  Plot::SetCanvasStyle(first);

  first->GetZaxis()->SetTitleOffset(offsetZ);
  first->GetZaxis()->SetTickSize(0.03);
  first->GetZaxis()->SetTitleSize(label);
  first->GetZaxis()->SetTitleFont(font);
  first->GetZaxis()->SetLabelFont(font);
  first->GetZaxis()->SetLabelSize(label);

}

void HeatMapPlot::SetPadStyle(TH2* first, TString xTitle, TString yTitle, TString zTitle, Float_t xUp, Float_t xLow, Float_t yUp, Float_t yLow, Float_t zUp, Float_t zLow){

  /** Set style aspects of the pads **/

  Plot::SetPadStyle(first, xTitle, yTitle, xUp, xLow, yUp, yLow);

  first->GetZaxis()->SetRangeUser(zLow, zUp);
  first->GetZaxis()->SetTitle(zTitle);

}

void HeatMapPlot::SetUpPad(TPad* pad, Bool_t xLog, Bool_t yLog, Bool_t zLog){

  /** Sets up a Pad for Plotting **/

  Plot::SetUpPad(pad, xLog, yLog);

  if (zLog){
    if (zRangeLow > 0) pad->SetLogz(1);
    else std::cout << "\033[1;31mERROR in SetLog:\033[0m Z-Ranges must be above zero! Logarithm not set!!" << std::endl;
  }

}

void HeatMapPlot::SetCanvasOffsets(Float_t xOffset, Float_t yOffset, Float_t zOffset){

  /** Set the Title Offsets **/

  offsetX = xOffset;
  offsetY = yOffset;
  offsetZ = zOffset;

}

void HeatMapPlot::SetLog(Bool_t xLog, Bool_t yLog, Bool_t zLog){

  /** Sets wether X and/or Y axis will be displayed logarithmically **/

  logX = xLog;
  logY = yLog;
  logZ = zLog;

}

void HeatMapPlot::SetRanges(Float_t xLow, Float_t xUp, Float_t yLow, Float_t yUp, Float_t zLow, Float_t zUp){

  /** Set the Ranges **/

  xRangeUp  = xUp;
  xRangeLow = xLow;
  yRangeUp  = yUp;
  yRangeLow = yLow;
  zRangeUp  = zUp;
  zRangeLow = zLow;

  ranges = kTRUE;

}


//
