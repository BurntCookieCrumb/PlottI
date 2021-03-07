
// ----------------------------------------------------------------------------
//
//                         ADDITIONAL FUNCTIONALITY
//
// ----------------------------------------------------------------------------

/*! \file */

#define FUNC_H

template <class AO>
Int_t GetXfirstFilledBin(AO* hst){

  /** Finds the first bin from the left with content > 0,
      if all bins (except last) are empty 0 is returned **/

  Int_t bin;

  for (bin = 0; bin < hst->GetNbinsX(); bin++){
    if (hst->GetBinContent(bin) > 0) break;
  }

  return (bin < hst->GetNbinsX()) ? bin : 0;

}

template <class AO>
Int_t GetXlastFilledBin(AO* hst){

  /** Finds the first bin from the right with content > 0,
      if all bins (except first) are empty last bin is returned **/

  Int_t bin;

  for (bin = hst->GetNbinsX(); bin > 0; bin--){
    if (hst->GetBinContent(bin) > 0) break;
  }

  return (bin > 0) ? bin : hst->GetNbinsX();

}

void CleanUpHistogram(TH1* hist, Double_t factor){

  /** Sets bin contents of bins with too large uncertainties to zero. **/
  /** The function finds the first bin (from the left) where the errorbar exceeds
      more than 100*\p factor percent of the range that is spanned by the histogram
      values and sets the content of this and all following bins to zero**/

  if (!hist){
    std::cout << "\033[1;31mERROR:\033[0m histogram to be cleaned up does not exist!" << std::endl;
    return;
  }

  Int_t cutoff = hist->GetNbinsX();
  Float_t range = TMath::Abs(hist->GetMaximum() - hist->GetMinimum());

  for (Int_t bin = 0; bin <= cutoff; bin++){

      Float_t error   =  2*hist->GetBinError(bin);
      if (error/range >= factor) cutoff = bin;

  }

  for (Int_t bin = cutoff; bin < hist->GetNbinsX(); bin++){

    hist->SetBinContent(bin, 0);
    hist->SetBinError(bin, 0);

  }

}
