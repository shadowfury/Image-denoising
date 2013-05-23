#ifndef NOISE_H
#define NOISE_H
class noiseClass
{
public:
    int distribution,int_red, int_green, int_blue,pro_red,pro_green,pro_blue,edit_num;
    noiseClass(int dst=0,int red1=0,int green1=0,int blue1=0, int red2=0,int green2=0,int blue2=0,int edit_num2=0){
        distribution=dst;
        int_red=red1;
        int_green=green1;
        int_blue=blue1;
        pro_red=red2;
        pro_green=green2;
        pro_blue=blue2;
        edit_num=edit_num2;
    }
    noiseClass& operator =(const noiseClass &right){
        distribution = right.distribution;
        int_red=right.int_red;
        int_green=right.int_green;
        int_blue=right.int_blue;
        pro_red=right.pro_red;
        pro_green=right.pro_green;
        pro_blue=right.pro_blue;
        edit_num=right.edit_num;
        return *this;
    }
};

#endif // NOISE_H
