#ifndef DENOISE_H
#define DENOISE_H
class denoiseClass
{
public:
    enum method{
        simple_squares=0,
        NLM,
        NLM_fast,
        NLM_fast_FFT,
        NLM_multyThread
    }enumField;

    int patch_size, search_window, pow;
    method denoisingMethod;
    denoiseClass(method val=simple_squares,int ps=0,int sw=0,int h=0): enumField(val){
        denoisingMethod=val;
        patch_size=ps;
        search_window=sw;
        pow=h;

    }
    denoiseClass& operator =(const denoiseClass &right){
        denoisingMethod = right.denoisingMethod;
        patch_size=right.patch_size;
        search_window=right.search_window;
        pow=right.pow;
        return *this;
    }
    ~denoiseClass(){}

};

#endif // NOISE_H
