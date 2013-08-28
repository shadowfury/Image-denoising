#ifndef RGBMY_H
#define RGBMY_H
class rgb_my{
private:
    double red_r,green_g,blue_b;
public:
    rgb_my(double red=0,double green=0,double blue=0){
        red_r=red;
        green_g=green;
        blue_b=blue;
    }
    inline void setRed(double r){
        red_r=r;
    }
    inline void setBlue(double b){
        blue_b=b;
    }
    inline void setGreen(double g){
        green_g=g;
    }
    inline double red() const{
        return red_r;
    }
    inline double blue() const{
        return blue_b;
    }
    inline double green() const{
        return green_g;
    }
    double color(int count) const{
        if (count==0) return red_r;
        if (count==1) return green_g;
        if (count==2) return blue_b;
    }
    void setColor(double color,int count){
        if (count==0) red_r=color;
        if (count==1) green_g=color;
        if (count==2) blue_b=color;
    }
    rgb_my& operator =(const rgb_my &right){
        this->red_r=right.red();
        this->blue_b=right.blue();
        this->green_g=right.green();
        return *this;
    }
};
#endif // RGBMY_H
