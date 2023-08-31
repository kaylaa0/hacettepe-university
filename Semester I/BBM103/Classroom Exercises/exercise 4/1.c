#include <stdio.h>

float recpower(a, b){
    float hold=1;
    int x=0;
    for(x;x<b;x++){
        hold=hold*a;
    }
    return hold;
}
int main(int argc, char*argv[]){
    float result;
    int a,b;
    a = atoi(argv[1]);
    b = atoi(argv[2]);
    if(a>0){
        if(b>0){
            result = recpower(a,b);
        }
        else if(b==0){
            result=1;
        }
        else{
            b = -b;
            result = 1/recpower(a,b);
    } }
    if(a<0){
        if(b%2==0){
            a=-a;
            if(b>0){
                result = recpower(a,b);
            }
            else if(b==0){
                result=1;
            }
            else{
                b = -b;
                result = 1/recpower(a,b);
            }}
        else{
            a=-a;
            if(b>0){
                result = -recpower(a,b);
            }
            else if(b==0){
                result=1;
            }
            else{
                b = -b;
                result = -1/recpower(a,b);
            }
        }
    }
    printf("%d to the power of %d = %f", a,b ,result);
}

