// ~~ PlotTING  ~~

// ----------------------------------------------------------------------------
//
// Header including all functional and plotting headers
//
// ----------------------------------------------------------------------------

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
#include "TMarker.h"
#include "TRandom.h"
#include "TImage.h"
#include "TTimeStamp.h"
#include "TMath.h"

#include "TString.h"

#include <iostream>
#include <vector>
#include <typeinfo>

#ifndef COLOR_H
  #include "Color.h"
#endif

#ifndef FUNC_H
  #include "functionality.h"
#endif

#ifndef BASE_H
  #include "PlotBase.h"
#endif

#ifndef DERIVED_H
  #include "PlotDerived.h"
#endif

#ifndef LEGEND_H
  #include "Legend.h"
#endif
