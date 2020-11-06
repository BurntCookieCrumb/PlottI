// ~~ PlotTING CLASS ~~

// ----------------------------------------------------------------------------
//
// Class for basic Plotting functionality
//
// ----------------------------------------------------------------------------

#ifndef Plotting
#define Plotting

// --- INCLUDES ---------------------------------------------------------------

#include "TH1.h"
#include "THn.h"
#include "TLatex.h"
#include "TObjArray.h"
#include "TPad.h"
#include "TCanvas.h"

#include "TLegend.h"
#include "TPaveText.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TMultiGraph.h"
#include "TStyle.h"
#include "TH2.h"
#include "TF1.h"
#include "TRandom.h"
#include "TImage.h"
#include "TTimeStamp.h"
#include "TMath.h"

#include "TString.h"

#include <iostream>
#include <vector>
#include <typeinfo>

// ----------------------------------------------------------------------------
//
//                         PLOT BASE CLASS
//
// ----------------------------------------------------------------------------

class Plot
{

public:

  enum Mode : unsigned int {
    Presentation,
    Thesis,
    Auto
  };

  Plot();
  Plot(TString xTitle, TString yTitle);
  virtual ~Plot() {}

  /*virtual*/ void Draw() {}

  template <class PO> static void SetLineProperties(PO* pobj, Color_t color, Style_t lstyle, Size_t lwid = 2.);
  template <class PO> static void SetMarkerProperties(PO* pobj, Color_t color, Style_t mstyle, Size_t msize = 3.);
  template <class PO> static void SetPlottjectProperties(PO* pobj, Color_t color, Style_t mstyle, Size_t msize = 3., Style_t lstyle = 1, Size_t lwid= 2., std::string title = "");
  static void CleanUpHistogram(TH1* hist, Double_t factor = 2.);
  void SetProperties(TObject* obj, Int_t index);

  void SetCanvasDimensions(Float_t cWidth, Float_t cHeight);
  void SetCanvasMargins(Float_t rMargin, Float_t lMargin, Float_t tMargin, Float_t bMargin);
  void SetCanvasOffsets(Float_t xOffset, Float_t yOffset);
  /*virtual*/ void SetLog(Bool_t xLog = kFALSE, Bool_t yLog = kTRUE);
  /*virtual*/ void SetRanges(Float_t xLow, Float_t xUp, Float_t yLow, Float_t yUp);
  void SetOffset(Int_t off);

  void SetMode(Mode m);
  void SetStyle(std::vector<Color_t> col, std::vector<Style_t> mark, std::vector<Size_t> siz = {}, std::vector<Style_t> lstyl = {}, std::vector<Size_t> lwid = {});
  void SetOptions(TString opt);
  void SetOptions(std::vector<std::string> optns);
  void SetOptions(std::string optns, std::string postns, Int_t off = 0);
  void SetOption(std::string opt, Int_t pos);

protected:

  void EnsureAxes(TObject* first, std::string arrayName = "");
  template <class AO> void SetCanvasStyle(AO* first);
  template <class AO> void SetPadStyle(AO* first, TString xTitle, TString yTitle, Float_t xUp, Float_t xLow, Float_t yUp, Float_t yLow);
  template <class AO> void SetRangesAuto(AO* first);
  void SetUpStyle(TObject* first, TString xTitle, TString yTitle, Float_t xUp, Float_t xLow, Float_t yUp, Float_t yLow);
  void SetUpPad(TPad* pad, Bool_t xLog, Bool_t yLog);
  void DrawArray(TObjArray* array, Int_t off = 0, Int_t offOpt = 0);

  TPad    *mainPad {nullptr};
  TCanvas *canvas  {nullptr};

  static std::vector<Color_t> colors;
  static std::vector<Style_t> markers;
  static std::vector<Style_t> lstyles;
  static std::vector<Size_t>  sizes;
  static std::vector<Size_t>  lwidths;
  std::vector<std::string>    options;

  TString titleX;
  TString titleY;

  Float_t width {0};
  Float_t height {0};
  Float_t offsetX {0};
  Float_t offsetY {0};
  Float_t rightMargin {0};
  Float_t leftMargin {0};
  Float_t topMargin {0};
  Float_t bottomMargin {0};
  Bool_t  logX {kFALSE};
  Bool_t  logY {kFALSE};

  Float_t yRangeLow {0};
  Float_t yRangeUp {100};
  Float_t xRangeLow {0};
  Float_t xRangeUp {100};

  Bool_t  ranges {kFALSE};
  Bool_t  broken {kFALSE};

  static Bool_t  styles;

  static Style_t font;
  static Style_t label;

  static Int_t   mOffset;

};

// ---- Constructors ----------------------------------------------------------

Plot::Plot():
  titleX(""),
  titleY("")
{
}

Plot::Plot(TString xTitle, TString yTitle):
  titleX(xTitle),
  titleY(yTitle)
{
}

// ---- Static Member Variables -----------------------------------------------

std::vector<Color_t>  Plot::colors;
std::vector<Style_t>  Plot::markers;
std::vector<Style_t>  Plot::lstyles;
std::vector<Size_t>   Plot::sizes;
std::vector<Size_t>   Plot::lwidths;

Style_t Plot::font {43};
Style_t Plot::label {28};

Bool_t  Plot::styles {kFALSE};
Int_t   Plot::mOffset {0};

// ---- Member Functions ------------------------------------------------------

template <class AO>
void Plot::SetCanvasStyle(AO* first){

  /** Set general style features of the Canvas and Pads **/

  first->GetXaxis()->SetTitleOffset(offsetX);
  first->GetYaxis()->SetTitleOffset(offsetY);
  first->GetXaxis()->SetTickSize(0.03);
  first->GetYaxis()->SetTickSize(0.03);
  first->GetXaxis()->SetTitleSize(label);
  first->GetYaxis()->SetTitleSize(label);
  first->GetXaxis()->SetTitleFont(font);
  first->GetYaxis()->SetTitleFont(font);
  first->GetXaxis()->SetLabelFont(font);
  first->GetYaxis()->SetLabelFont(font);
  first->GetXaxis()->SetLabelSize(label);
  first->GetYaxis()->SetLabelSize(label);

}

template <class AO>
void Plot::SetPadStyle(AO* first, TString xTitle, TString yTitle, Float_t xUp, Float_t xLow, Float_t yUp, Float_t yLow){

  /** Set style aspects of the pads **/

  if (first->InheritsFrom("TGraph")) first->GetXaxis()->SetLimits(xLow, xUp);
  else first->GetXaxis()->SetRangeUser(xLow, xUp);
  first->GetYaxis()->SetRangeUser(yLow, yUp);
  first->GetXaxis()->SetTitle(xTitle);
  first->GetYaxis()->SetTitle(yTitle);

}

void Plot::SetUpStyle(TObject* first, TString xTitle, TString yTitle, Float_t xUp, Float_t xLow, Float_t yUp, Float_t yLow){

  /* Set Style aspects of pad and canvas */

  if (first->InheritsFrom("TH1")){
    if (!ranges) SetRangesAuto((TH1*)first);
    SetPadStyle((TH1*)first, xTitle, yTitle, xUp, xLow, yUp, yLow);
    SetCanvasStyle((TH1*)first);
  }
  else if (first->InheritsFrom("TF1")){
    if (!ranges) SetRangesAuto((TF1*)first);
    SetPadStyle((TF1*)first, xTitle, yTitle, xUp, xLow, yUp, yLow);
    SetCanvasStyle((TF1*)first);
  }
  else if (first->InheritsFrom("TMultiGraph")){
    SetPadStyle((TMultiGraph*)first, xTitle, yTitle, xUp, xLow, yUp, yLow);
    SetCanvasStyle((TMultiGraph*)first);
  }

}

void Plot::CleanUpHistogram(TH1* hist, Double_t factor){

  /** Sets bin contents of bins with too large uncertainties to 0 **/
  /** \todo{Get it to work} **/

  if (!hist){
    std::cout << "\033[1;31mERROR:\033[0m histogram to be cleaned up does not exist!" << std::endl;
    return;
  }

}

template <class PO>
void Plot::SetLineProperties(PO* pobj, Color_t color, Style_t lstyle, Size_t lwid){

  /** Set style properties of lines **/

  pobj->SetLineStyle(lstyle);
  pobj->SetLineWidth(lwid);
  pobj->SetLineColor(color);

}

template <class PO>
void Plot::SetMarkerProperties(PO* pobj, Color_t color, Style_t mstyle, Size_t msize){

  /** Set style properties of markers **/

  pobj->SetMarkerColor(color);
  pobj->SetMarkerStyle(mstyle);
  pobj->SetMarkerSize(msize);

}

template <class PO>
void Plot::SetPlottjectProperties(PO* pobj, Color_t color, Style_t mstyle, Size_t msize, Style_t lstyle, Size_t lwid, std::string title){

  /** Set style properties of plottable objects **/

  pobj->SetTitle(title.data());
  SetMarkerProperties(pobj, color, mstyle, msize);
  SetLineProperties(pobj, color, lstyle, lwid);

}

void Plot::SetProperties(TObject* obj, Int_t index){

  /** Manages internal setting of properties for all plottable objects **/

  Double_t size, lwidth; Int_t lstyle;
  size   = !(sizes.empty())   ? sizes[index]   : 2.;
  lstyle = !(lstyles.empty()) ? lstyles[index] : 1;
  lwidth = !(lwidths.empty()) ? lwidths[index] : 2.;

  if (obj->InheritsFrom("TPave")){ //TLegend
    ((TLegend*)obj)->SetTextFont(font);
    ((TLegend*)obj)->SetTextSize(label);
    ((TLegend*)obj)->SetBorderSize(0);
  }

  else if (!styles) return;
  else if (index >= markers.size()) return;

  else if (obj->InheritsFrom("TH1")) {
    SetPlottjectProperties((TH1*)obj, colors[index], markers[index], size, lstyle, lwidth);
    ((TH1*)obj)->SetStats(kFALSE);
  }
  else if (obj->InheritsFrom("TF1")){
    SetPlottjectProperties((TF1*)obj, colors[index], markers[index], size, lstyle, lwidth);
  }
  else if (obj->InheritsFrom("TGraph")){
    SetPlottjectProperties((TGraph*)obj, colors[index], markers[index], size, lstyle, lwidth);
  }
  else if (obj->InheritsFrom("TMultiGraph")){
    TIter iMultiGraph(((TMultiGraph*)obj)->GetListOfGraphs());
    while (TObject* graph = iMultiGraph()){
      if (!graph) continue;
      if (index >= markers.size()) break;
      SetPlottjectProperties((TGraph*)graph, colors[index], markers[index], size, lstyle, lwidth);
      index++;
    }
  }
  else if (obj->InheritsFrom("TLine")){
    SetLineProperties((TLine*)obj, colors[index], lstyle, lwidth);
  }
  else{
    std::cout << "\033[1;34mMissing Class \033[0m" << obj->ClassName() << std::endl;
  }

}

void Plot::SetCanvasDimensions(Float_t cWidth, Float_t cHeight){

  /** Set Dimensions of the Canvas **/

  width  = cWidth;
  height = cHeight;

}

void Plot::SetCanvasMargins(Float_t rMargin, Float_t lMargin, Float_t tMargin, Float_t bMargin){

  /** Set the Margins of the Canvas **/

  rightMargin  = rMargin;
  leftMargin   = lMargin;
  topMargin    = tMargin;
  bottomMargin = bMargin;

}

void Plot::SetCanvasOffsets(Float_t xOffset, Float_t yOffset){

  /** Set the Title Offsets **/

  offsetX = xOffset;
  offsetY = yOffset;

}

void Plot::SetLog(Bool_t xLog, Bool_t yLog){

  /** Sets wether X and/or Y axis will be displayed logarithmically **/
  /** \todo{Find out why logx sometimes doesnt work} **/

  logX = xLog;
  logY = yLog;

}

void Plot::SetRanges(Float_t xLow, Float_t xUp, Float_t yLow, Float_t yUp){

  /** Set the Ranges **/

  xRangeUp  = xUp;
  xRangeLow = xLow;
  yRangeUp  = yUp;
  yRangeLow = yLow;

  ranges = kTRUE;

}

void Plot::SetOffset(Int_t off){

  /** Sets the offset of the style properties markers **/

  mOffset = off;

}

template <class AO>
void Plot::SetRangesAuto(AO* first){

  /** Automatically determine good ranges **/
  /** \todo{implement possibility for ranges to be negative}
      \todo{implement for TF1 and TGraph} **/


  yRangeUp   = 1.1*first->GetMaximum();
  yRangeLow  = 0.9*first->GetMinimum();

  // xRangeUp   = 1.1*first->GetBinCenter(0);
  // xRangeLow  = 0.9*first->GetBinCenter(first->GetNbinsX());

}

void Plot::SetMode(Mode m){

  /** Set the mode of the program, based on what the plots will be used for **/

  switch(m){

    case Presentation:
      font = 43; //43
      label = 40;//37;
      break;

    case Thesis:
      font = 43; //43
      label = 30;//37;
      break;

    case Auto:
      break;

    default:
      break;
  }

 }

void Plot::SetStyle(std::vector<Color_t> col, std::vector<Style_t> mark, std::vector<Size_t> siz, std::vector<Style_t> lstyl, std::vector<Size_t> lwid){

  /** Set style arrays for the histograms and functions **/

  colors  = std::move(col);
  markers = std::move(mark);

  if (!siz.empty())    sizes   = std::move(siz);
  else sizes.clear();  //??
  if (!lstyl.empty())  lstyles = std::move(lstyl);
  else lstyles.clear();  //??
  if (!lwid.empty())   lwidths = std::move(lwid);
  else lwidths.clear();  //??

  styles = kTRUE;

}

void Plot::SetOptions(TString opt){

  /** Set one plot option for all plottjects **/

  Int_t size = options.size();
  options.clear();
  options.resize(size, opt.Data());

}

void Plot::SetOptions(std::vector<std::string> optns){

  /** Set the plot options for the plottjects,
      mind that any legend or pave object must also be included **/

  options = std::move(optns);

}

void Plot::SetOptions(std::string optns, std::string postns, Int_t off){

  /** Set the plot options \p optns for a few specific plottjects at positions \p postns,
      \p off will be an offset added to all individual positions;
      mind that any legend or pave object is also included in the options **/

  std::istringstream options(optns);
  std::istringstream positions(postns);

  TString* opt = new TString();
  TString* pos = new TString();

  opt->ReadLine(options);
  pos->ReadToken(positions);

  while(!opt->IsNull() && !pos->IsNull()) {

    SetOption(opt->Data(), pos->Atoi() + off);
    std::cout << "- " << opt->Data() << " " << pos->Data() << std::endl;

    opt->ReadLine(options);
    pos->ReadToken(positions);

  }

}

void Plot::SetOption(std::string opt, Int_t pos){

  /** Set the plot option for a specific plottject
      mind that any legend or pave object is also included in the options **/

  if (pos < options.size()) options[pos] = opt;
  else std::cout << "\033[1;31mERROR in Set Options:\033[0m Position \033[1;34m" << pos << "\033[0m is out of range!" << std::endl;

}

void Plot::SetUpPad(TPad* pad, Bool_t xLog, Bool_t yLog){

  /** Sets up a Pad for Plotting **/

  pad->SetFillStyle(4000);
  pad->SetTopMargin(topMargin);
  pad->SetBottomMargin(bottomMargin);
  pad->SetRightMargin(rightMargin);
  pad->SetLeftMargin(leftMargin);
  pad->SetTickx(1);
  pad->SetTicky(1);

  if (xLog){
    if (xRangeLow > 0) pad->SetLogx(1);
    else std::cout << "\033[1;31mERROR in SetLog:\033[0m X-Ranges must be above zero! Logarithm not set!!" << std::endl;
  }
  if (yLog){
    if (yRangeLow > 0) pad->SetLogy(1);
    else std::cout << "\033[1;31mERROR in SetLog:\033[0m Y-Ranges must be above zero! Logarithm not set!!" << std::endl;
  }

}

void Plot::EnsureAxes(TObject* first, std::string arrayName){

  /* Ensure that first object in array to be plotted has well defined axes */

  if (!first) {

    std::cout << "\033[1;33mFATAL ERROR:\033[0m First entry in array doesn't exist!!" << std::endl;
    broken = kTRUE;
    return;

  }

  if (!(first->InheritsFrom("TH1")) && !(first->InheritsFrom("TF1")) && !(first->InheritsFrom("TMultiGraph"))){

    std::cout << "\033[1;33mFATAL ERROR:\033[0m First entry in array must have axes "
    << "\033[1;36m(" << arrayName << ")\033[0m" << std::endl;
    broken = kTRUE;
    return;

  }

}

void Plot::DrawArray(TObjArray* array, Int_t off, Int_t offOpt){

  /** Draws a single TObjArray in the chosen Pad **/

  Int_t nPlots = array->GetEntries();
  std::string opt;

  for (Int_t plot = 0; plot < nPlots; plot++){

    if(!array->At(plot)) {
      std::cout << "\033[1;31mERROR:\033[0m Plot object No " << plot << " is broken! Will be skipped." << std::endl;
      continue;
    }

    if (array->At(plot)->InheritsFrom("TGraph")) opt = TString(options[plot+offOpt]).ReplaceAll("SAME","").Data();
    else opt = options[plot+offOpt].data();

    std::cout << " -> Draw " << array->At(plot)->ClassName() << ": "
              << array->At(plot)->GetName() << " as " << opt << std::endl;

    SetProperties(array->At(plot), plot + off);
    array->At(plot)->Draw(opt.data());

  }

}

// ----------------------------------------------------------------------------
//                              SQUARE PLOT CLASS
// ----------------------------------------------------------------------------

class SquarePlot : public Plot
{

public:

  SquarePlot(TObjArray* array, TString xTitle, TString yTitle);
  virtual ~SquarePlot() {}

  /*virtual*/ void Draw(TString outname);

private:

  TObjArray* plotArray;

};

// ---- Constructor -----------------------------------------------------------

SquarePlot::SquarePlot(TObjArray* array, TString xTitle, TString yTitle): Plot(xTitle, yTitle),
  plotArray(array)
{

  EnsureAxes(array->At(0), "Main Array");

  SetCanvasDimensions(1000, 1000);
  if(yTitle.Contains("frac")) {
    SetCanvasMargins(0.05, .125, 0.05, .1);
  }
  else {
    SetCanvasMargins(0.05, .1, 0.05, .1);
  }
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

class RatioPlot : public Plot
{

public:

  RatioPlot(TObjArray* rArray, TString xTitle, TString yTitle);
  virtual ~RatioPlot() {};

  /*virtual*/ void Draw(TString outname);
  /*virtual*/ void DrawRatioArray(TObjArray* array, Int_t off, Int_t offOpt = 0);
  void SetUpperOneLimit(Double_t up);

protected:

  TObjArray* plotArray;

  TLine*   one {nullptr};
  Double_t oneUp {0};

};

// ---- Constructor -----------------------------------------------------------

RatioPlot::RatioPlot(TObjArray* rArray, TString xTitle, TString yTitle) : Plot(xTitle, yTitle),
  plotArray(rArray)
{

  EnsureAxes(rArray->At(0), "Main Array");

  SetCanvasDimensions(1000, 600);
  SetCanvasMargins(0.07, .15, 0.07, .25);
  SetCanvasOffsets(1., .8);

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

  canvas  = new TCanvas("canvas", "RATIO", 10, 10, width+10, height+10);
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

  one = new TLine(xRangeLow, 1., (oneUp ? oneUp : xRangeUp), 1.);
  array->Add(one);

  DrawArray(array, off, offOpt);
  SetLineProperties(one, kBlack, 9, 3.);

}

void RatioPlot::SetUpperOneLimit(Double_t up){

  /* Sets upper limit on line in ratio at value one */

  oneUp = up;

}

// ----------------------------------------------------------------------------
//                         SINGLE RATIO PLOT CLASS
// ----------------------------------------------------------------------------

class SingleRatioPlot : public RatioPlot
{

public:

  SingleRatioPlot(TObjArray* mainArray, TObjArray* ratioArray, TString xTitle, TString yTitle, TString ratioTitle);
  virtual ~SingleRatioPlot() {};

  /*virtual*/ void Draw(TString outname);

  void SetPadFraction(Double_t frac);
  void SetOffset(Int_t off, Int_t roff);
  /*virtual*/ void SetRanges(Float_t xUp, Float_t xLow, Float_t yUp, Float_t yLow, Float_t rUp, Float_t rLow);
  void SetOptions(std::string optns, std::string postns);

private:

  static Float_t padFrac;

  TPad* ratioPad {nullptr};
  TString ratioTitle;

  TObjArray* ratioArray;

  Float_t rRangeUp {1.2};
  Float_t rRangeLow {0.8};

  static Int_t rOffset;

};

// ---- Static Member Variables -----------------------------------------------

Float_t SingleRatioPlot::padFrac {0.25};
Int_t   SingleRatioPlot::rOffset  {1};

// ---- Cunstructor -----------------------------------------------------------

SingleRatioPlot::SingleRatioPlot(TObjArray* mainArray, TObjArray* rArray, TString xTitle, TString yTitle, TString rTitle) : RatioPlot(mainArray, xTitle, yTitle),
  ratioTitle(rTitle),
  ratioArray(rArray)
{

  EnsureAxes(mainArray->At(0), "Main Array");
  EnsureAxes(rArray->At(0), "Ratio Array");

  SetCanvasDimensions(1000, 1200);
  SetCanvasMargins(0.07, .15, 0.07, .4);
  SetCanvasOffsets(4.5, 1.7);

  options = std::vector<std::string>(mainArray->GetEntries()+rArray->GetEntries(), "SAME");

}

// ---- Member Functions ------------------------------------------------------

void SingleRatioPlot::Draw(TString outname){

  /** Main function for Drawing **/

  std::cout << "-----------------------------" << std::endl;
  std::cout << "  Plot Single Ratio Canvas: " << std::endl;
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
  SetUpStyle(plotArray->At(0), titleX, titleY, xRangeUp, xRangeLow, yRangeUp, yRangeLow);
  mainPad->SetBottomMargin(0.);
  mainPad->Draw();

  ratioPad = new TPad("ratioPad", "Ratio", 0, 0, 1, padFrac);
  SetUpPad(ratioPad, logX, kFALSE);
  ratioPad->SetTopMargin(0.);
  SetUpStyle(ratioArray->At(0), titleX, ratioTitle, xRangeUp, xRangeLow, rRangeUp, rRangeLow);
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
//
//                         LEGEND CLASS
//
// ----------------------------------------------------------------------------

class Legend
{
public:

  Legend();
  Legend(TObjArray* array, std::string entries, std::string opt, std::string title="");
  Legend(std::string obj, std::string entries, std::string opt, Int_t nEntries);
  Legend(std::string entries, Int_t nEntries);
  Legend(Legend& lgnd);
  Legend(TLegend* lgnd);
  ~Legend() {}

  TLegend* GetLegendPointer(){return legend;};
  static void SetPosition(TLegend* l, Float_t x1, Float_t x2, Float_t y1, Float_t y2);
  void SetPositionAuto();

  TLegend* legend;
  std::vector<TH1*> dummy;

};

// ---- Constructors ----------------------------------------------------------

Legend::Legend():
  legend(nullptr),
  dummy(0)
{
}

Legend::Legend(TObjArray* array, std::string entr, std::string opt, std::string title):
  legend(nullptr),
  dummy(0)
{

  legend = new TLegend(0.1, 0.7, 0.3, 0.9);

  std::istringstream entries(entr);
  std::istringstream options(opt);

  TString* option    = new TString();
  TString* entryName = new TString();

  if (title != "") legend->AddEntry((TObject*)0x0, title.data(), "");

  TIter iArray(array);
  while (TObject* obj = iArray()) {

    if (obj->InheritsFrom("TPave")) continue;

    option->ReadToken(options);
    entryName->ReadLine(entries);

    legend->AddEntry(obj, entryName->Data(), option->Data());
  }

  array->Add(legend);

}

Legend::Legend(std::string obj, std::string entr, std::string opt, Int_t nEntries):
legend(nullptr),
dummy(nEntries)
{

  legend = new TLegend(0.1, 0.7, 0.3, 0.9);

  std::istringstream objects(obj);
  std::istringstream entries(entr);
  std::istringstream options(opt);

  TString* option    = new TString();
  TString* entryName = new TString();
  TString* object    = new TString();

  TString* color  = new TString();
  TString* marker = new TString();
  TString* size   = new TString();

  for(Int_t entry = 0; entry < nEntries; entry++){

    object->ReadLine(objects);
    std::istringstream token(object->Data());

    color ->ReadToken(token);
    marker->ReadToken(token);
    size  ->ReadToken(token);

    option->ReadToken(options);
    entryName->ReadLine(entries);

    dummy[entry] = new TH1C();
    Plot::SetPlottjectProperties(dummy[entry], color->Atoi(), marker->Atoi(), size->Atof());

    legend->AddEntry(dummy[entry], entryName->Data(), option->Data());

  }

}

Legend::Legend(std::string entr, Int_t nEntries):
legend(nullptr),
dummy(nEntries)
{

  legend = new TLegend(0.1, 0.7, 0.3, 0.9);

  std::istringstream entries(entr);
  TString* entryName = new TString();

  for(Int_t entry = 0; entry < nEntries; entry++){

    entryName->ReadLine(entries);

    legend->AddEntry((TObject*)0x0, entryName->Data(), "");

  }

}

Legend::Legend(Legend& lgnd):
  legend(nullptr),
  dummy(0)
{
  /** \todo{Get it to work} **/
}

Legend::Legend(TLegend* lgnd):
  legend(nullptr),
  dummy(0)
{
/** \todo{Get it to work, maybe derive this class from TLegend to gain access to fPrimitives} **/
}

// ---- Member Functions ------------------------------------------------------

void Legend::SetPosition(TLegend* l, Float_t x1, Float_t x2, Float_t y1, Float_t y2){

  /** Set the Position of a Legend in relative coordinates **/

  l->SetX1(x1);
  l->SetX2(x2);
  l->SetY1(y1);
  l->SetY2(y2);

}

void Legend::SetPositionAuto(){

  /** Determine automatic placement of Legend based on position strings **/

  /** \todo{Get it to work}**/

  Float_t relLegendWidth  = legend->GetX2NDC() - legend->GetX1NDC();
  Float_t relLegendHeight = legend->GetNRows()*0.05;

  std::cout << relLegendWidth << std::endl;
  std::cout << legend->GetNRows() << std::endl;

  //SetPosition(obj, 0.1, 0.1+relLegendWidth, 0.1, 0.1+relLegendHeight);
  //SetPosition(0.2, 0.2 + relLegendWidth, 0.85-relLegendHeight, 0.85);

}









#endif

//
