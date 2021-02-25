
#define DERIVED_H

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
  SetCanvasMargins(0.07, .15, 0.07, .15);
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
  options.push_back("SAME");

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
  /*virtual*/ void SetRanges(Float_t xLow, Float_t xUp, Float_t yLow, Float_t yUp, Float_t rLow, Float_t rUp);
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

Float_t SingleRatioPlot::padFrac {0.3}; //0.25
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
  SetUpStyle(plotArray->At(0), "", titleY, xRangeUp, xRangeLow, yRangeUp, yRangeLow);
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
