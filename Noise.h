#ifndef NOISE_H
#define NOISE_H
class noiseClass
{
public:
    int distribution,int_red, int_green, int_blue,pro_red,pro_green,pro_blue,edit_num;
    noiseClass(){
        distribution=0;
        int_red=0;
        int_green=0;
        int_blue=0;
        pro_red=0;
        pro_green=0;
        pro_blue=0;
        edit_num=0;
    }

    noiseClass(int dst,int red1,int green1,int blue1, int red2,int green2,int blue2,int edit_num2){
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
