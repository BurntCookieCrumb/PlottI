
// ----------------------------------------------------------------------------
//
//                         ADDITIONAL FUNCTIONALITY
//
// ----------------------------------------------------------------------------

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
