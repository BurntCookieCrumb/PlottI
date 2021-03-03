// ~~ PlotTING CLASS ~~


// ----------------------------------------------------------------------------
//
// Class for basic Plotting functionality
//
// ----------------------------------------------------------------------------

#define BASE_H

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

  if (obj->InheritsFrom("TPave")){ //TLegend
    ((TLegend*)obj)->SetTextFont(font);
    ((TLegend*)obj)->SetTextSize(label);
    ((TLegend*)obj)->SetBorderSize(0);
    return;
  }

  if (!styles) return;
  if (index >= markers.size()) return;

  Double_t size, lwidth; Int_t lstyle;
  size   = !(sizes.empty())   ? sizes[index]   : 2.;
  lstyle = !(lstyles.empty()) ? lstyles[index] : 1;
  lwidth = !(lwidths.empty()) ? lwidths[index] : 2.;

  if (obj->InheritsFrom("TH1")) {
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
  /** \todo{Test for TF1 and TGraph} **/

  yRangeUp  = first->GetMaximum();
  yRangeUp  = (yRangeUp < 0) ? 0.8*yRangeUp : 1.2*yRangeUp;

  yRangeLow = first->GetMinimum();
  yRangeLow = (yRangeLow < 0) ? 1.2*yRangeLow : 0.8*yRangeLow;

  Int_t binLow  = GetXfirstFilledBin(((TH1*)first));
  Int_t binHigh = GetXlastFilledBin(((TH1*)first));
  xRangeUp   = ((TH1*)first)->GetBinCenter(binHigh+2);
  xRangeLow  = ((TH1*)first)->GetBinCenter(binLow-1);

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

  pad->SetFillStyle(4100); //4000
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
