#ifndef BLUR_H
#define BLUR_H

class blurClass
{
public:
    int radius;
    blurClass(int rad=0){
        radius=rad;
    }
    blurClass& operator =(const blurClass &right){
        radius = right.radius;
        return *this;
    }
};


#endif // BLUR_H
