float MAX(float a, float b){
    if(a > b){
        return a;
    }
    else{
        return b;
    }
/*
  return (a > b) ? a : b;
*/
}

float MIN(float a, float b){
    if(a < b){
        return a;
    }
    else{
        return b;
    }
}
float ABS(float a){
    if(a < 0){
        return (-1)*a;
    }
    
}

float CLAMP(float a,float b, float c){
    
    if (a<b){
        a = b;
    }
    else if (a>c){
        a = c;
    }
    return a;
}

int REMAINDER(int a, int b){
    return (a%b+b)%b;
}
