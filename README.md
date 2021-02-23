# Header
important headers for various applications


---> See documentation of PlottI:
https://krschmit-cern.github.io/Header/index.html

---> Please find examples in the example folder (:

# TODOs
- Member Legend::Legend (Legend &lgnd) -- Get it to work (Copy Constructor), maybe derive this class from TLegend to gain access to fPrimitives
- Member Legend::SetPositionAuto ()  -- Get it to work
- Member Plot::CleanUpHistogram (TH1 *hist, Double_t factor=2.) -- Get it to work
- Member Plot::SetLog (Bool_t xLog=kFALSE, Bool_t yLog=kTRUE) -- Find out why logx sometimes doesnt work
- Member Plot::SetRangesAuto (AO *first) -- Test for TF1 and TGraph
