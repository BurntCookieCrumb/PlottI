// ~~ PlotTING CLASS ~~

// ----------------------------------------------------------------------------
//
//  Class for basic Plotting functionality
//
// ----------------------------------------------------------------------------

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
#include "TStyle.h"
#include "TH2.h"
#include "TF1.h"
#include "TRandom.h"
#include "TImage.h"
#include "TTimeStamp.h"
#include "TMath.h"

#include "TString.h"

#include <iostream>
#include <typeinfo>

// ----------------------------------------------------------------------------
//                              PLOT BASE CLASS
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

  virtual void Draw() {}

  void SetCanvasStyle(TH1* mainHist);
  void SetPadStyle(TH1* mainHist, TString xTitle, TString yTitle, Float_t xUp, Float_t xLow, Float_t yUp, Float_t yLow);
  static void SetHistogramProperties(TH1* hist, Color_t color, Style_t style, Size_t size);
  static void SetFunctionProperties(TF1* func, Color_t color, Style_t style, Size_t size);
  void SetProperties(TObject* obj, Int_t index);

  void SetCanvasDimensions(Float_t cWidth, Float_t cHeight);
  void SetCanvasMargins(Float_t rMargin, Float_t lMargin, Float_t tMargin, Float_t bMargin);
  void SetCanvasOffsets(Float_t xOffset, Float_t yOffset);

  virtual void SetRanges(Float_t xUp, Float_t xLow, Float_t yUp, Float_t yLow);
  static void SetPosition(TPave* obj, Float_t x1, Float_t x2, Float_t y1, Float_t y2);
  static void SetPosition(TPave* obj, Int_t x1, Int_t x2, Int_t y1, Int_t y2);

  void SetLegendPositions(TString* pos);
  void SetMode(Mode m);
  void SetArrays(Color_t *col, Style_t *mark, Size_t *siz);
  void SetOptions(TString opt);


protected:

  void SetRangesAuto(TH1* hist);
  void SetPositionAuto(TLegend* obj);
  void SetUpPad(TPad* pad);
  void DrawArray(TObjArray* array);

  TPad    *mainPad;
  TCanvas *canvas;

  static Color_t *colors;
  static Style_t *markers;
  static Size_t  *sizes;

  TString titleX;
  TString titleY;

  Float_t width;
  Float_t height;
  Float_t offsetX;
  Float_t offsetY;
  Float_t rightMargin;
  Float_t leftMargin;
  Float_t topMargin;
  Float_t bottomMargin;

  Float_t yRangeLow;
  Float_t yRangeUp;
  Float_t xRangeLow;
  Float_t xRangeUp;
  Bool_t  ranges;

  static Style_t font;
  static Style_t label;

  static TString options;
  static TString* positions;

};

// ---- Constructors ----------------------------------------------------------

Plot::Plot():
  mainPad(nullptr),
  canvas(nullptr),
  titleX(""),
  titleY(""),
  width(0),
  height(0),
  offsetX(0),
  offsetY(0),
  rightMargin(0),
  leftMargin(0),
  topMargin(0),
  bottomMargin(0),
  yRangeLow(0),
  yRangeUp(100),
  xRangeLow(0),
  xRangeUp(100),
  ranges(kFALSE)
{
}

Plot::Plot(TString xTitle, TString yTitle):
  mainPad(nullptr),
  canvas(nullptr),
  titleX(xTitle),
  titleY(yTitle),
  width(0),
  height(0),
  offsetX(0),
  offsetY(0),
  rightMargin(0),
  leftMargin(0),
  topMargin(0),
  bottomMargin(0),
  yRangeLow(0),
  yRangeUp(100),
  xRangeLow(0),
  xRangeUp(100),
  ranges(kFALSE)
{
}

// ---- Static Member Variables -----------------------------------------------

Color_t*  Plot::colors {nullptr};
Style_t*  Plot::markers {nullptr};
Size_t*   Plot::sizes {nullptr};

Style_t   Plot::font {43};
Style_t   Plot::label {28};

TString   Plot::options {TString("SAME")};
TString*  Plot::positions {nullptr};

// ---- Member Functions ------------------------------------------------------

void Plot::SetCanvasStyle(TH1* mainHist){

  mainHist->SetTitle("");
  mainHist->SetTitleFont(43);
  mainHist->SetTitleSize(0);
  mainHist->GetXaxis()->SetTitleOffset(offsetX);
  mainHist->GetYaxis()->SetTitleOffset(offsetY);
  mainHist->GetXaxis()->SetTickSize(0.03);
  mainHist->GetYaxis()->SetTickSize(0.03);
  mainHist->SetTitleSize(label, "X");
  mainHist->SetTitleSize(label, "Y");
  mainHist->SetTitleFont(font,  "X");
  mainHist->SetTitleFont(font,  "Y");
  mainHist->SetLabelFont(font,  "X");
  mainHist->SetLabelFont(font,  "Y");
  mainHist->SetLabelSize(label, "X");
  mainHist->SetLabelSize(label, "Y");
  mainHist->SetStats(kFALSE);

}

void Plot::SetPadStyle(TH1* mainHist, TString xTitle, TString yTitle, Float_t xUp, Float_t xLow, Float_t yUp, Float_t yLow){

  mainHist->GetXaxis()->SetRangeUser(xLow, xUp);
  mainHist->GetYaxis()->SetRangeUser(yLow, yUp);
  mainHist->SetXTitle(xTitle);
  mainHist->SetYTitle(yTitle);

}

void Plot::SetHistogramProperties(TH1* hist, Color_t color, Style_t style, Size_t size){

  hist->SetMarkerColor(color);
  hist->SetMarkerStyle(style);
  hist->SetMarkerSize(size);

  hist->SetLineWidth(2);
  hist->SetLineColor(color);

}

void Plot::SetFunctionProperties(TF1* func, Color_t color, Style_t style, Size_t size){

  func->SetMarkerColor(color);
  func->SetMarkerStyle(style);
  func->SetMarkerSize(size);
  
  func->SetLineWidth(2);
  func->SetLineColor(color);

}

void Plot::SetProperties(TObject* obj,  Int_t index){

  if (obj->InheritsFrom("TH1") && markers) {
    SetHistogramProperties((TH1*)obj, colors[index], markers[index], sizes[index]);
  }

  else if (obj->InheritsFrom("TF1") && markers){
    SetFunctionProperties((TF1*)obj, colors[index], markers[index], sizes[index]);
  }

  else if (obj->InheritsFrom("TLegend")){
    ((TLegend*)obj)->SetTextFont(font);
    ((TLegend*)obj)->SetTextSize(label);
    ((TLegend*)obj)->SetBorderSize(0);
    if (positions) SetPositionAuto((TLegend*)obj);
  }

  else std::cout << "Missing Class:" << obj->ClassName() << std::endl;

}

void Plot::SetCanvasDimensions(Float_t cWidth, Float_t cHeight){

    width  = cWidth;
    height = cHeight;

}

void Plot::SetCanvasMargins(Float_t rMargin, Float_t lMargin, Float_t tMargin, Float_t bMargin){

  rightMargin  = rMargin;
  leftMargin   = lMargin;
  topMargin    = tMargin;
  bottomMargin = bMargin;

}

void Plot::SetCanvasOffsets(Float_t xOffset, Float_t yOffset){

  offsetX = xOffset;
  offsetY = yOffset;

}

void Plot::SetRanges(Float_t xUp, Float_t xLow, Float_t yUp, Float_t yLow){

  xRangeUp  = xUp;
  xRangeLow = xLow;
  yRangeUp  = yUp;
  yRangeLow = yLow;

  ranges = kTRUE;

}

void Plot::SetRangesAuto(TH1* hist){

  yRangeUp   = 1.1*hist->GetMaximum();
  yRangeLow  = 0.9*hist->GetMinimum();

  xRangeUp   = 1.1*hist->GetBinCenter(0);
  xRangeLow  = 0.9*hist->GetBinCenter(hist->GetNbinsX());

}

void Plot::SetPosition(TPave* obj, Float_t x1, Float_t x2, Float_t y1, Float_t y2){

  obj->SetX1NDC(x1);
  obj->SetX2NDC(x2);
  obj->SetY1NDC(y1);
  obj->SetY2NDC(y2);

}

void Plot::SetPosition(TPave* obj, Int_t x1, Int_t x2, Int_t y1, Int_t y2){

  obj->SetX1(x1);
  obj->SetX2(x2);
  obj->SetY1(y1);
  obj->SetY2(y2);

}

void Plot::SetLegendPositions(TString* pos){

  positions = pos;

}

void Plot::SetPositionAuto(TLegend* obj){

  std::cout << obj->GetNRows() << std::endl;
  std::cout << obj->GetNColumns() << std::endl;

}

void Plot::SetMode(Mode m){

  switch(m){

    case Presentation:
      font = 43; //43
      label = 40;//37;
      break;

    case Thesis:
      break;

    case Auto:
      break;

    default:
      break;
  }

 }

void Plot::SetArrays(Color_t *col, Style_t *mark, Size_t *siz){

  colors  = col;
  markers = mark;
  sizes   = siz;

}

void Plot::SetOptions(TString opt){

  options = opt;

}

void Plot::SetUpPad(TPad* pad){

  pad->SetFillStyle(4000);
  pad->SetTopMargin(topMargin);
  pad->SetBottomMargin(bottomMargin);
  pad->SetRightMargin(rightMargin);
  pad->SetLeftMargin(leftMargin);
  pad->SetTickx(1);
  pad->SetTicky(1);

}

void Plot::DrawArray(TObjArray* array){

  Int_t nPlots = array->GetEntries();

  SetCanvasStyle((TH1D*)array->At(0));

  for (Int_t plot = 0; plot < nPlots; plot++){

    std::cout << " -> Draw " << array->At(plot)->ClassName() << ": "
              << array->At(plot)->GetName() << std::endl;

    if(!array->At(plot)) {
      std::cout << "Plot object No " << plot << " is broken!" << std::endl;
      return;
    }

    SetProperties((TH1D*)array->At(plot), plot);

    array->At(plot)->Draw(options.Data());

  }

}

// ----------------------------------------------------------------------------
//                              SQUARE PLOT CLASS
// ----------------------------------------------------------------------------

class SquarePlot : public Plot
{

public:

  SquarePlot(TObjArray* array, TString xTitle, TString yTitle);
  virtual ~SquarePlot() {};

  virtual void Draw(TString outname);

private:

  TPad* mainPad;
  TObjArray* plotArray;

};

SquarePlot::SquarePlot(TObjArray* array, TString xTitle, TString yTitle): Plot(xTitle, yTitle){

  if (!(array->At(0)->InheritsFrom("TH1")) and !(array->At(0)->InheritsFrom("TF1"))){

    std::cout << "First entry in Array should be a Histogram or Function" << std::endl;
    return;

  }

  plotArray = array;

  SetCanvasDimensions(1000, 1000);
  SetCanvasMargins(0.07, .15, 0.07, .15);
  SetCanvasOffsets(1.3, 1.5);

}

void SquarePlot::Draw(TString outname){

  std::cout << "----------------------------" << std::endl;
  std::cout << "  Plot Square Canvas:" << std::endl;

  canvas  = new TCanvas("canvas", "SQUARE", 10, 10, width+10, height+10);
  canvas->cd();

  mainPad = new TPad("mainPad", "Distribution", 0, 0, 1, 1);
  SetUpPad(mainPad);
  if (!ranges) SetRangesAuto((TH1D*)plotArray->At(0));
  SetPadStyle((TH1*)plotArray->At(0), titleX, titleY, xRangeUp, xRangeLow, yRangeUp, yRangeLow);
  mainPad->Draw();
  mainPad->cd();

  DrawArray(plotArray);

  canvas->SaveAs(outname.Data());

  std::cout << "----------------------------" << std::endl;

}
// ----------------------------------------------------------------------------
//                              RATIO PLOT CLASS
// ----------------------------------------------------------------------------

class SingleRatioPlot : public Plot
{

public:

  SingleRatioPlot(TObjArray* mainArray, TObjArray* ratioArray, TString xTitle, TString yTitle, TString ratioTitle);
  virtual ~SingleRatioPlot() {};

  virtual void Draw(TString outname);

  void SetPadFraction(Double_t frac);
  virtual void SetRanges(Float_t xUp, Float_t xLow, Float_t yUp, Float_t yLow, Float_t rUp, Float_t rLow);

private:

  static Float_t padFrac;

  TPad* mainPad;
  TPad* ratioPad;

  TString ratioTitle;

  TObjArray* plotArray;
  TObjArray* ratioArray;

  Float_t rRangeUp;
  Float_t rRangeLow;

};

// ---- Static Member Variables -----------------------------------------------

Float_t SingleRatioPlot::padFrac {0.25};

// ---- Cunstructor -----------------------------------------------------------

SingleRatioPlot::SingleRatioPlot(TObjArray* mainArray, TObjArray* rArray, TString xTitle, TString yTitle, TString rTitle) : Plot(xTitle, yTitle){

  if (!(mainArray->At(0)->InheritsFrom("TH1")) and !(mainArray->At(0)->InheritsFrom("TF1"))){

    std::cout << "First entry in Main Array should be a Histogram or Function" << std::endl;
    return;

  }

  if (!(rArray->At(0)->InheritsFrom("TH1")) and !(rArray->At(0)->InheritsFrom("TF1"))){

    std::cout << "First entry in Ratio Array should be a Histogram or Function" << std::endl;
    return;

  }

  ratioTitle = rTitle;

  plotArray = mainArray;
  ratioArray = rArray;

  SetCanvasDimensions(1000, 1200);
  SetCanvasMargins(0.07, .15, 0.07, .3);
  SetCanvasOffsets(4.5, 1.7);

}

// ---- Member Functions ------------------------------------------------------

void SingleRatioPlot::Draw(TString outname){

  std::cout << "----------------------------" << std::endl;
  std::cout << "  Plot Single Ratio Canvas:" << std::endl;

  canvas  = new TCanvas("canvas", "SINGLE RATIO", 10, 10, width+10, height+10);
  canvas->cd();

  mainPad = new TPad("mainPad", "Distribution", 0, padFrac, 1, 1);
  SetUpPad(mainPad);
  mainPad->SetBottomMargin(0.);
  if (!ranges) SetRangesAuto((TH1D*)plotArray->At(0));
  SetPadStyle((TH1*)plotArray->At(0), titleX, titleY, xRangeUp, xRangeLow, yRangeUp, yRangeLow);
  mainPad->Draw();

  ratioPad = new TPad("ratioPad", "Ratio", 0, 0, 1, padFrac);
  SetUpPad(ratioPad);
  ratioPad->SetTopMargin(0.);
  if (!ranges) SetRangesAuto((TH1D*)ratioArray->At(0));
  SetPadStyle((TH1*)ratioArray->At(0), titleX, ratioTitle, xRangeUp, xRangeLow, rRangeUp, rRangeLow);
  ratioPad->Draw();

  mainPad->cd();
  DrawArray(plotArray);

  ratioPad->cd();
  DrawArray(ratioArray);

  canvas->SaveAs(outname.Data());

  std::cout << "----------------------------" << std::endl;

}

// ------ Member Functions ----------------------------------------------------

void SingleRatioPlot::SetPadFraction(Double_t frac){

  padFrac = frac;

}

void SingleRatioPlot::SetRanges(Float_t xUp, Float_t xLow, Float_t yUp, Float_t yLow, Float_t rUp, Float_t rLow){

  xRangeUp  = xUp;
  xRangeLow = xLow;
  yRangeUp  = yUp;
  yRangeLow = yLow;
  rRangeUp  = rUp;
  rRangeLow = rLow;

  ranges = kTRUE;

}

















//
