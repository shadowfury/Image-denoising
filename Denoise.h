#ifndef DENOISE_H
#define DENOISE_H
class denoiseClass
{
public:
    int method,patch_size, search_window, pow;
    denoiseClass(){
        method=0;
        patch_size=0;
        search_window=0;
        pow=0;
    }

    denoiseClass(int dst,int ps,int sw,int h){
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
