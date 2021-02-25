

// ----------------------------------------------------------------------------
//
//                         LEGEND CLASS
//
// ----------------------------------------------------------------------------

#define LEGEND_H

class Legend : public TLegend
{
public:

  Legend();
  Legend(TObjArray* array, std::string entries, std::string opt, std::string title="", std::string name="");
  Legend(std::string obj, std::string entries, std::string opt, Int_t nEntries, std::string name="");
  Legend(std::string entries, Int_t nEntries, std::string name="");
  Legend(Legend& lgnd, std::string name="");
  Legend(Legend* lgnd, std::string name="");
  ~Legend() {}

        Legend* GetLegendPointer()       {return this;}
  const Legend* GetLegendPointer() const {return this;}
  static void SetPosition(TLegend* l, Float_t x1, Float_t x2, Float_t y1, Float_t y2);
  void SetPosition(Float_t x1, Float_t x2, Float_t y1, Float_t y2);
  void SetPositionAuto();

  std::vector<TH1*> dummy;

};

// ---- Constructors ----------------------------------------------------------

Legend::Legend(): TLegend(),
  dummy(0)
{
}

Legend::Legend(TObjArray* array, std::string entr, std::string opt, std::string title, std::string name): TLegend(0.1, 0.7, 0.3, 0.9),
  dummy(0)
{

  if (!array) {
    std::cout << "\033[1;31mERROR:\033[0m Array is empty! Try again!" << std::endl;
    return;
  }

  std::istringstream entries(entr);
  std::istringstream options(opt);

  TString* option    = new TString();
  TString* entryName = new TString();

  if (title != "") AddEntry((TObject*)0x0, title.data(), "");
  if (name  != "") fName = name;

  TIter iArray(array);
  while (TObject* obj = iArray()) {

    if (obj->InheritsFrom("TPave")) continue;

    option->ReadToken(options);
    entryName->ReadLine(entries);

    AddEntry(obj, entryName->Data(), option->Data());
  }

  array->Add(this);

}

Legend::Legend(std::string obj, std::string entr, std::string opt, Int_t nEntries, std::string name): TLegend(0.1, 0.7, 0.3, 0.9),
dummy(nEntries)
{

  if (name  != "") fName = name;

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

    AddEntry(dummy[entry], entryName->Data(), option->Data());

  }

}

Legend::Legend(std::string entr, Int_t nEntries, std::string name): TLegend(0.1, 0.7, 0.3, 0.9),
dummy(nEntries)
{

  if (name  != "") fName = name;

  std::istringstream entries(entr);
  TString* entryName = new TString();

  for(Int_t entry = 0; entry < nEntries; entry++){

    entryName->ReadLine(entries);

    AddEntry((TObject*)0x0, entryName->Data(), "");

  }

}

Legend::Legend(Legend& lgnd, std::string name): TLegend(0.1, 0.7, 0.3, 0.9),
  dummy(0)
{
  if (name  != "") fName = name;
  fPrimitives = new TList();
  TIter prim(lgnd.fPrimitives);
  while(TLegendEntry* entry = (TLegendEntry*)prim()){
    fPrimitives->Add(entry);
  }
}

Legend::Legend(Legend* lgnd, std::string name): TLegend(0.1, 0.7, 0.3, 0.9),
  dummy(0)
{
  if (name  != "") fName = name;
  fPrimitives = new TList();
  TIter prim(lgnd->fPrimitives);
  while(TLegendEntry* entry = (TLegendEntry*)prim()){
    fPrimitives->Add(entry);
  }
}

// ---- Member Functions ------------------------------------------------------

void Legend::SetPosition(TLegend* l, Float_t x1, Float_t x2, Float_t y1, Float_t y2){

  /** Set the Position of a Legend in relative coordinates **/

  l->SetX1(x1);
  l->SetX2(x2);
  l->SetY1(y1);
  l->SetY2(y2);

}

void Legend::SetPosition(Float_t x1, Float_t x2, Float_t y1, Float_t y2){

  /** Set the Position of a Legend in relative coordinates **/

  fX1 = x1;
  fX2 = x2;
  fY1 = y1;
  fY2 = y2;

}

void Legend::SetPositionAuto(){

  /** Determine automatic placement of Legend based on position strings **/

  /** \todo{Get it to work}**/

  // Float_t relLegendWidth  = legend->GetX2NDC() - legend->GetX1NDC();
  // Float_t relLegendHeight = legend->GetNRows()*0.05;

  // std::cout << relLegendWidth << std::endl;
  // std::cout << legend->GetNRows() << std::endl;

  //SetPosition(obj, 0.1, 0.1+relLegendWidth, 0.1, 0.1+relLegendHeight);
  //SetPosition(0.2, 0.2 + relLegendWidth, 0.85-relLegendHeight, 0.85);

}



//
