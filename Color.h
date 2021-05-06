
// ~~ Colors ~~

// ----------------------------------------------------------------------------
//
// This file contains various functionalities for easy use of colors in ROOT
// - Predefined Colors and Palettes
//
// ----------------------------------------------------------------------------

#define COLOR_H

// ----------------------------------------------------------------------------
//                                Functionality
// ----------------------------------------------------------------------------

//! Structure for saving RGB colors
struct color {

  //! Constructor
  color(Float_t red, Float_t green, Float_t blue) : r(red), g(green), b(blue)
  {
    index = TColor::GetColor(r, g, b);
  }

  Float_t r; //!< red part of color
  Float_t g; //!< green part of color
  Float_t b; //!< blue part of color

  Color_t index; //!< ROOT color index of color

};

//! Class for saving color gradients (palettes)
class ColorGradient
{

public:

  ColorGradient();
  ColorGradient(Int_t nPoints, const vector<color> &rgbEndpoints, const vector<Double_t> &stops = {}, Float_t alpha = 1);
  ~ColorGradient() {};

  vector<Int_t>   GetPalette();
  vector<Color_t> GetGradient();
  Int_t           GetNpoints();

private:

  Int_t firstColorIndex {0};      //!< first color index of the color gradient
  Int_t nColorPoints {0};         //!< number of points in the color gradient

};

//! Constructor
ColorGradient::ColorGradient(Int_t nPoints, const vector<color> &rgbEndpoints, const vector<Double_t> &stops, Float_t alpha):
  nColorPoints(nPoints)
{

  /** Generate color gradient with \p nPoints colors from \p rgbEndpoints \p alpha
      transparency.
      \param[in] nPoints       Number of colors that will be generated from the color endpoints
      \param[in] rgbEndpoints  Color endpoints in RGB format using \ref color structure
      \param[in] stops         Determines spacing of colors in gradient,
                               if not given the colors will be spaced evenly
      \param[in] alpha         Determines \c alpha value of colors
  **/

  Int_t nColors = rgbEndpoints.size();

  vector<Double_t> red;
  vector<Double_t> green;
  vector<Double_t> blue;
  vector<Double_t> length;

  if (stops.empty()){
    for(Int_t col = 0; col < nColors; col++){
       length.push_back((Float_t)col/(nColors-1));
    }
    std::cout << std::endl;
  }
  else if (stops.size() != nColors){
    std::cout << "\033[1;31mERROR:\033 Number of given stops does not match number of given colors!!" << std::endl;
    throw;
  }
  else length = std::move(stops);

  for (const color& rgb : rgbEndpoints) {

    red.push_back(rgb.r);
    green.push_back(rgb.g);
    blue.push_back(rgb.b);

  }

  firstColorIndex = TColor::CreateGradientColorTable(nColors, length.data(), red.data(), green.data(), blue.data(), nPoints, alpha);

}

vector<Int_t> ColorGradient::GetPalette(){

  /** Return stored palette as vector of Int_t **/

  std::vector<Int_t> gradientColors(nColorPoints);
  std::iota(gradientColors.begin(), gradientColors.end(), firstColorIndex);
  return gradientColors;

}

vector<Color_t> ColorGradient::GetGradient(){

  /** Return stored palette as vector of Color_t **/

  std::vector<Color_t> gradientColors(nColorPoints);
  std::iota(gradientColors.begin(), gradientColors.end(), firstColorIndex);
  return gradientColors;

}

Int_t ColorGradient::GetNpoints(){

  /** Return the number of color points of the stored palette **/

  return nColorPoints;

}

// ----------------------------------------------------------------------------
//                              Predefined Colors
// ----------------------------------------------------------------------------

color blue    {0.00, 0.00, 1.00};
color green   {0.00, 1.00, 0.00};
color red     {1.00, 0.00, 0.00};
color cyan    {0.00, 1.00, 1.00};
color yellow  {1.00, 1.00, 0.00};
color magenta {1.00, 0.00, 1.00};
color purple  {0.40, 0.00, 0.60};

color alice_red     {0.851, 0.027, 0.094};
color alice_blue    {0.012, 0.039, 0.549};
color alice_grey    {0.169, 0.220, 0.251};
color alice_orange  {0.949, 0.475, 0.059};
color alice_rosered {0.749, 0.255, 0.255};

// ----------------------------------------------------------------------------
//                              Predefined Palettes
// ----------------------------------------------------------------------------

vector<color> rgbRainbow = {blue, cyan, green, yellow, red, magenta};
ColorGradient rainbow = ColorGradient(20, rgbRainbow);

vector<color> rgbAlice = {alice_grey, alice_blue, alice_red, alice_rosered, alice_orange};
ColorGradient alice_logo = ColorGradient(100, rgbAlice);

vector<color> purple_and_yellow = {purple, yellow};
ColorGradient purple_to_yellow = ColorGradient(100, purple_and_yellow);

//
