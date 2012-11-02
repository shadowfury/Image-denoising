#ifndef DENOISE_H
#define DENOISE_H
class denoiseClass
{
public:
    int method,patch_size, search_window, pow;
    denoiseClass(int dst=0,int ps=0,int sw=0,int h=0){
        method=dst;
        patch_size=ps;
        search_window=sw;
        pow=h;
    }
    denoiseClass& operator =(const denoiseClass &right){
        method = right.method;
        patch_size=right.patch_size;
        search_window=right.search_window;
        pow=right.pow;
        return *this;
    }
};

#endif // NOISE_H
