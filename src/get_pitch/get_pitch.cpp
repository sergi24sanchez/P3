/// @file

#include <iostream>
#include <fstream>
#include <string.h>
#include <errno.h>
//Añadimos librerias extra
#include <cmath>
#include "ffft/FFTReal.h"

#include "wavfile_mono.h"
#include "pitch_analyzer.h"

#include "docopt.h"

#define FRAME_LEN   0.030 /* 30 ms. */
#define FRAME_SHIFT 0.015 /* 15 ms. */

using namespace std;
using namespace upc;

static const char USAGE[] = R"(
get_pitch - Pitch Detector 

Usage:
    get_pitch [options] <input-wav> <output-txt>
    get_pitch (-h | --help)
    get_pitch --version

Options:
    -h, --help  Show this screen
    --version   Show the version of the project

Arguments:
    input-wav   Wave file with the audio signal
    output-txt  Output file: ASCII file with the result of the detection:
                    - One line per frame with the estimated f0
                    - If considered unvoiced, f0 must be set to f0 = 0
)";

int main(int argc, const char *argv[]) {
	/// \TODO 
	///  Modify the program syntax and the call to **docopt()** in order to
	///  add options and arguments to the program.
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
        {argv + 1, argv + argc},	// array of arguments, without the program name
        true,    // show help if requested
        "2.0");  // version string

	std::string input_wav = args["<input-wav>"].asString();
	std::string output_txt = args["<output-txt>"].asString();

  // Read input sound file
  unsigned int rate;
  vector<float> x;
  if (readwav_mono(input_wav, rate, x) != 0) {
    cerr << "Error reading input file " << input_wav << " (" << strerror(errno) << ")\n";
    return -2;
  }

  int n_len = rate * FRAME_LEN;
  int n_shift = rate * FRAME_SHIFT;
  float max_value = *max_element(x.begin(),x.end());
  // Define analyzer
  PitchAnalyzer analyzer(n_len, rate, PitchAnalyzer::HAMMING, 50, 500);

  /// \TODO
  /// Preprocess the input signal in order to ease pitch estimation. For instance,
  /// central-clipping or low pass filtering may be used.
  /// \DONE

//Center-Clipping 1
#if 0
  //float max_value = *max_element(x.begin(),x.end());
  float center_value = 0.015*max_value;
  //Apply center clipping
  for (unsigned int i = 0; i < x.size(); i++){
    x[i] = x[i] / max_value; //Normalizamos

    if( x[i] > center_value){
      x[i] = x[i] - center_value;}

    else if(x[i] < -center_value){
      x[i] = x[i] + center_value;
    }
    else x[i] = 0;
  }
#endif

//Center-Clipping 2
#if 0
  //float max_value = *max_element(x.begin(),x.end());
  //Apply center clipping
  for (unsigned int i = 0; i < x.size(); i++){
    if(abs(x[i]) / max_value < 0.015) 
      x[i] = 0.0F;
  }
#endif

//Low-Pass Filter
#if 1
  // Definimos FFTReal
   ffft::FFTReal <float> fft_object1 (1024); //Creamos el objeto de la clase FFTReal (dónde estan las funciones)

   //Filtrado Paso-Bajo en frecuencia
 // ==================================================================================================
   //Hacemos la transformada de fourier de la señal x
   std::vector<float> Xlow; ///vector coeficientes frecuencia
   Xlow.resize(x.size());
   fft_object1.do_fft(Xlow.data(),x.data());


   //Definimos el filtro Paso-Bajo
   std::vector<float> lpf;
   lpf.resize(x.size());
   float cutoff_fl = 2000; //frecuencia de corte del filtro
   int k_low = (cutoff_fl/rate)*x.size(); //Posicion de en la DFT

   for (int n = 0;n < k_low;n++) lpf[n] = 1;
   for(int n = k_low; n < x.size(); n++) lpf[n] = 0;

   for(int n = 0 ; n<Xlow.size();n++) Xlow[n] *= lpf[n];  //Filtramos la señal

   //Antitransformamos
   fft_object1.do_ifft (Xlow.data(), x.data());
   fft_object1.rescale (x.data());
 // ==================================================================================================*/
#endif

//High-Pass Filter
#if 0
  // Definimos FFTReal
   ffft::FFTReal <float> fft_object2 (1024); //Creamos el objeto de la clase FFTReal (dónde estan las funciones)

   //Filtrado Paso-Alto en frecuencia
 // ==================================================================================================
   //Hacemos la transformada de fourier de la señal x
   std::vector<float> Xhigh; ///vector coeficientes frecuencia
   Xhigh.resize(x.size());
   fft_object2.do_fft(Xhigh.data(),x.data());


   //Definimos el filtro Paso-Alto
   std::vector<float> hpf;
   hpf.resize(x.size());
   float cutoff_fh = 40; //frecuencia de corte del filtro
   int k = (cutoff_fh/rate)*x.size(); //Posicion de en la DFT

   for(int n = 0;n < k; n++) hpf[n] = 0;
   for(int n = k; n < x.size(); n++) hpf[n] = 1;

   for(int n = 0 ; n < Xhigh.size(); n++) Xhigh[n] *= hpf[n];  //Filtramos la señal

   //Antitransformamos
   fft_object2.do_ifft (Xhigh.data(), x.data());
   fft_object2.rescale (x.data());
 // ==================================================================================================*/
#endif
  // Iterate for each frame and save values in f0 vector
  vector<float>::iterator iX;
  vector<float> f0;
  for (iX = x.begin(); iX + n_len < x.end(); iX = iX + n_shift) {
    float f= analyzer(iX, iX + n_len);
    f0.push_back(f);
  }

  /// \TODO
  /// Postprocess the estimation in order to supress errors. For instance, a median filter
  /// or time-warping may be used.
  ///
#if 0

#endif

  // Write f0 contour into the output file
  ofstream os(output_txt);
  if (!os.good()) {
    cerr << "Error reading output file " << output_txt << " (" << strerror(errno) << ")\n";
    return -3;
  }

  os << 0 << '\n'; //pitch at t=0
  for (iX = f0.begin(); iX != f0.end(); ++iX) 
    os << *iX << '\n';
  os << 0 << '\n';//pitch at t=Dur

  return 0;
}
