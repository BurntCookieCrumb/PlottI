
// Includes

#include <iostream>

#include <string>
#include "TString.h"

#include "TH1.h"
#include "TH2.h"
#include "THn.h"

#include "TFile.h"



// ------------------------------------------------------------------------------------
// helpful functions
// ------------------------------------------------------------------------------------

Double_t xGetMean(TH1 *hist, Double_t pTMin, Double_t pTMax){

    /** function to get the mean of a one dimensional Histogram in range pTMin to pTMax
        containing the yield, which has to be normalized to bin width,
    **/

    if(!hist) return 0;
    //if(hist->GetEntries()<=0) return 0;
    //if(pTMin < hist->GetXaxis()->GetBinLowEdge(1) || pTMax > hist->GetXaxis()->GetBinCenter(hist->GetNbinsX())) return 0;

    Double_t sumWeighted = 0, sum = 0;
    Double_t binCenter, binWidth;
    Double_t yield;

    for(int bin = hist->FindBin(pTMin); bin <= hist->FindBin(pTMax); bin++){

        binCenter = hist->GetBinCenter(bin);
        binWidth = hist->GetBinWidth(bin);
        yield = hist->GetBinContent(bin);

        sumWeighted += yield * binCenter * binWidth;
        sum += yield * binWidth;
    }


    //return sumWeighted/sum;
    if (sum > 0) return sumWeighted/sum;
    else return 0.;


}

Double_t xGetMeanError(TH1 *hist, Double_t pTMin, Double_t pTMax){

    /** function to get the mean error of a one dimensional Histogram in range pTMin to pTMax
        containing the yield, which has to be normalized to bin width,
    **/

    if(!hist) return 0;

    Double_t mean = xGetMean(hist, pTMin, pTMax);

    Double_t errSumWeighted = 0, sum = 0;
    Double_t binCenter, binWidth;

    Double_t yield, yieldError;

    for(int bin = hist->FindBin(pTMin); bin <= hist->FindBin(pTMax); bin++){

        binCenter = hist->GetBinCenter(bin);
        binWidth = hist->GetBinWidth(bin);

        yield = hist->GetBinContent(bin);
        yieldError = hist->GetBinError(bin); //* binCenter;

        errSumWeighted += TMath::Power((binCenter - mean) * yieldError * binWidth, 2);
        sum += yield * binWidth;

    }

    if (sum > 0) return TMath::Sqrt(errSumWeighted) / sum;
    else return 0.;

}


void GetMeanpT(TH1D *meanpT, THnF *source, int pTAxis, int multAxis, int multBins = 200){

    /** Fill a one dimensional Histogram \meanpT with the mean transverse momentum of each
        multiplicity bin of a n dimensional Histogram \source, which should not be normalized
    **/

    TH1D *singleMultpT;

    for(int i = 1; i <= multBins; i++){

          source->GetAxis(multAxis)->SetRange(i,i);
          singleMultpT = source->Projection(pTAxis);

          singleMultpT->Scale(1, "width");

          //meanpT->SetBinContent(i, singleMultpT->GetMean());
          meanpT->SetBinContent(i, xGetMean(singleMultpT, 0.15, 9.99));

          //meanpT->SetBinError(i, singleMultpT->GetMeanError());
          meanpT->SetBinError(i, xGetMeanError(singleMultpT, 0.15, 9.99));

          delete singleMultpT;

      }

}

void GetMeanpT(TH1D *meanpT, TH2D *source, int multBins = 200){

    /** Fill a one dimensional Histogram \meanpT with the mean transverse momentum of each
        multiplicity bin of a 2 dimensional Histogram \source
    **/

    TH1D *singleMultpT;

    for(int i = 1; i <= multBins; i++){

          singleMultpT = source->ProjectionY("singleMultpT", i, i);

          for (int k = 1; k <= singleMultpT->GetXaxis()->GetLast(); k++){

                singleMultpT->SetBinContent(k, (singleMultpT->GetBinContent(k))*(singleMultpT->GetBinCenter(k)));

          }

          //meanpT->SetBinContent(i, singleMultpT->GetMean());
          meanpT->SetBinContent(i, xGetMean(singleMultpT, 0.15, 9.99));

          //meanpT->SetBinError(i, singleMultpT->GetMeanError());
          meanpT->SetBinError(i, xGetMeanError(singleMultpT, 0.15, 9.99));

          delete singleMultpT;

      }

}

// -------------------------------------------------------------------------------------

void GetMoment(TH1* hist, Int_t order, Double_t &moment, Double_t &moment_error,
               Double_t min, Double_t max, bool inv){

    /** function to get the \order-th moment and the corresponding error of a one
        dimensional Histogram in range \min to \max containing the yield which has to be
        normalized to bin width.
        \inv = @true means the histogram contains the invariant yield.
    **/

    if (!hist) exit(0);

    // == Moment ==

    Double_t sumWeighted = 0, sum = 0;
    Double_t binCenter, binWeight, yield;
    Double_t invCorr;

    Double_t weightedQuantity;

    for(int bin = hist->FindBin(min); bin <= hist->FindBin(max); bin++){

        binCenter = hist->GetBinCenter(bin);
        binWeight = hist->GetBinWidth(bin);

        invCorr = (inv) ? binCenter : 1;

        yield = hist->GetBinContent(bin) * invCorr;

        sumWeighted += yield * TMath::Power(binCenter, order) * binWeight;
        sum += yield * binWeight;

    }

    moment = (sum > 0) ? (sumWeighted / sum) : 0;

    // == Error ==

    Double_t errSumWeighted = 0;
    Double_t yieldErr;

    for(int bin = hist->FindBin(min); bin <= hist->FindBin(max); bin++){

        binCenter = hist->GetBinCenter(bin);
        binWeight = hist->GetBinWidth(bin);

        invCorr = (inv) ? binCenter : 1;

        //yieldErr = hist->GetBinError(bin) * invCorr;
        yieldErr = TMath::Sqrt(hist->GetBinContent(bin)) * invCorr;

        errSumWeighted += TMath::Power(yieldErr *
                                       (TMath::Power(binCenter, order) - moment) *
                                       binWeight, 2);

    }

    moment_error = (sum > 0) ? (TMath::Sqrt(errSumWeighted) / sum) : 0;

}

void GetCentralMoment(TH1* hist, Int_t order, Double_t &moment, Double_t &moment_error,
                      Double_t min, Double_t max, bool inv){

    /** function to get the \order-th central moment and the corresponding error of a
        one dimensional Histogram in range \min to \max containing the yield which has to
        be normalized to bin width.
        \inv = @true means the histogram contains the invariant yield.
    **/

    if (!hist) exit(0);

    // == Moment ==

    // Get Mean and Corresponding Error

    Double_t mean, mean_error;

    GetMoment(hist, 1, mean, mean_error, min, max, inv);

    // Get Central Moment

    Double_t sumWeighted = 0, sum = 0;
    Double_t binCenter, binWeight, yield;
    Double_t invCorr;

    Double_t weightedQuantity;

    for(int bin = hist->FindBin(min); bin <= hist->FindBin(max); bin++){

        binCenter = hist->GetBinCenter(bin);
        binWeight = hist->GetBinWidth(bin);

        invCorr = (inv) ? binCenter : 1;

        yield = hist->GetBinContent(bin) * invCorr;

        sumWeighted += yield * TMath::Power(binCenter - mean, order) * binWeight;
        sum += yield * binWeight;

    }


    moment = (sum > 0) ? (sumWeighted / sum) : 0;

    // == Error ==

    Double_t errSumWeighted_1 = 0, errSumWeighted_2 = 0;
    Double_t yieldErr;

    for(int bin = hist->FindBin(min); bin <= hist->FindBin(max); bin++){

        binCenter = hist->GetBinCenter(bin);
        binWeight = hist->GetBinWidth(bin);

        invCorr = (inv) ? binCenter : 1;

        //yieldErr = hist->GetBinError(bin) * invCorr;
        yieldErr = TMath::Sqrt(hist->GetBinContent(bin)) * invCorr;
        yield = hist->GetBinContent(bin) * invCorr;

        errSumWeighted_1 += TMath::Power(yieldErr *
                                       (TMath::Power(binCenter - mean, order) - moment) *
                                       binWeight, 2);

        errSumWeighted_2 += order * TMath::Power(binCenter - mean, order - 1)
                                  * yield * binWeight * mean_error;

    }

    moment_error = (sum > 0) ? (TMath::Sqrt(errSumWeighted_1 +
                                            TMath::Power(errSumWeighted_2, 2)) / sum) : 0;

}

void GetCumulant (TH1* hist, Int_t order, Double_t &cumulant, Double_t &cumulant_error,
                      Double_t min, Double_t max, bool inv){

    /** function to get the \order-th cumulant and the corresponding error of a
        one dimensional Histogram in range \min to \max containing the yield which has to
        be normalized to bin width.
        \inv = @true means the histogram contains the invariant yield.
    **/

    if (!hist) exit(0);

    // == Cumulants ==

    Double_t m1, m2, m3;
    Double_t m1_error, m2_error, m3_error;

    switch(order){

        case 3: GetMoment(hist, 3, m3, m3_error, min, max, inv);
        case 2: GetMoment(hist, 2, m2, m2_error, min, max, inv);
        case 1: GetMoment(hist, 1, m1, m1_error, min, max, inv); break;

    }

    switch(order){

        case 3  : cumulant = m3 - 3*m2*m1 + 2*m1*m1*m1; break;
        case 2  : cumulant = m2 - m1*m1;                break;
        case 1  : cumulant = m1;                        break;
        default : cumulant = 0;                         break;

    }

    // == ERROR ==

    switch(order){

        case 3  : cumulant_error = TMath::Sqrt(m3_error * m3_error +
                                               9 * m1 * m1 * m2_error * m2_error +
                                               TMath::Power(6 * m1 * m1 - 3 * m2, 2)
                                               * m1_error * m1_error);             break;

        case 2  : cumulant_error = TMath::Sqrt(m2_error * m2_error +
                                               4 * m1 * m1 * m1_error * m1_error); break;

        case 1  : cumulant_error = m1_error;                                       break;

        default : cumulant_error = 0;                                              break;

    }

}

void GetMomentHist(TH1D* dest, THnF *source, Int_t func_axis, Int_t var_axis, Char_t measure,
                   Int_t order, Double_t func_min, Double_t func_max, bool norm = false,
                   bool inv = false){

    /** Fill a one dimensional Histogram \dest with the \order-th
        \measure (moment, cumulant, central moment) of each multiplicity bin of a n
        dimensional Histogram \source.
        \norm = @true means that the input histogram is normalized to bin width.
        \inv = @true means that the input histogram is the invariant yield
    **/

    TH1D *singleMultpT;

    Double_t moment, moment_error;

    for(int multBin = 1; multBin <= source->GetAxis(var_axis)->GetNbins(); multBin ++){

        source->GetAxis(var_axis)->SetRange(multBin, multBin);
        singleMultpT = source->Projection(func_axis);

        if(!norm) singleMultpT->Scale(1, "width");

        switch(measure){

        case 'M': GetMoment(singleMultpT, order, moment, moment_error, func_min, func_max,
                            inv); break;
        case 'Z': GetCentralMoment(singleMultpT, order, moment, moment_error, func_min,
                                   func_max, inv); break;
        case 'K': GetCumulant(singleMultpT, order, moment, moment_error, func_min, func_max,
                              inv); break;
        default : exit(0);

        }

        dest->SetBinContent(multBin, moment);
        dest->SetBinError(multBin, moment_error);

        delete singleMultpT;

    }

}

void GetMomentHist(TH1D* dest, TH2D *source, Char_t measure, Int_t order, Double_t y_min,
                   Double_t y_max, bool norm = true, bool inv = true){

    /** Fill a one dimensional Histogram \dest with the \order-th
        \measure (moment, cumulant, central moment) of each multiplicity bin of a 2
        dimensional Histogram \source
        \norm = @true means that the input histogram is normalized to bin width
        \inv = @true means that the input histogram is the invariant yield
    **/

    TH1D *singleMultpT;

    Double_t moment, moment_error;

    for(int multBin = 1; multBin <= source->GetXaxis()->GetNbins(); multBin ++){

        singleMultpT = source->ProjectionY("singleMultpT", multBin, multBin);

        if(!norm) singleMultpT->Scale(1, "width");

        switch(measure) {

        case 'M': GetMoment(singleMultpT, order, moment, moment_error, y_min, y_max, inv);
                  break;
        case 'Z': GetCentralMoment(singleMultpT, order, moment, moment_error, y_min, y_max,
                                   inv);
                  break;
        case 'K': GetCumulant(singleMultpT, order, moment, moment_error, y_min, y_max, inv);
                  break;


        }

        dest->SetBinContent(multBin, moment);
        dest->SetBinError(multBin, moment_error);

        delete singleMultpT;

    }

}

// --------------------------------------------------------------------------------------

#ifndef plotting

TString niceExp(double x){

    /** Receives a floating point number \x and returns a TString containing a nicer exponential
        representation of \x that can be used in a TLegend
    **/

    TString nString = Form("%3.2e", x);

    nString.ReplaceAll("e", " #upoint 10^{");
    nString.ReplaceAll("+", "");
    nString.ReplaceAll("{0", "{");
    nString.ReplaceAll("{-0", "{-");
    nString.Append("}");

    return nString;

}

TString legendEntry(TString tune, double nEvents){

    /** Formats a nice TString containing the used tune \tune and the number of events \nEvents
        that can be used in a TLegend
    **/

    tune.Append("(");
    tune.Append(niceExp(nEvents));
    tune.Append(" events)");

    return tune;

}

#endif
