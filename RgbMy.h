#ifndef RGBMY_H
#define RGBMY_H
class rgb_my{
private:
    double red_r,green_g,blue_b;
public:
    void setRed(double r){
        red_r=r;
    }
    void setBlue(double b){
        blue_b=b;
    }
    void setGreen(double g){
        green_g=g;
    }
    double red(){
        return red_r;
    }
    double blue(){
        return blue_b;
    }
    double green(){
        return green_g;
    }
    double color(int count){
        if (count==0) return red_r;
        if (count==1) return green_g;
        if (count==2) return blue_b;
    }
    void setColor(double color,int count){
        if (count==0) red_r=color;
        if (count==1) green_g=color;
        if (count==2) blue_b=color;
    }
};
#endif // RGBMY_H
