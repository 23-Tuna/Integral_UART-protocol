#include "../UART_protocol.cpp"
#include <iostream>

struct Test1{
    int data;
    char c;
    float f;
    int ary[2] = {0};
};

class Test2{
    public:
    int data;
    char c;
    Test2(int a,char b) : data(a),c(b){};
    void pri(){
        std::cout<<c<<data<<std::endl;
    }
};

int main(){
    Test1 t1{
        32,
        'a',
        3.14,
        {1,2}
    };
    Test2 t2(36,'a');
    UARTEncoder enc(1);
    UARTDecoder dec(4);
    Test1 r1;
    Test2 r2(0,'b');
    uint8_t buf1[256],buf2[256];
    enc.SetRawData(t1);
    enc.Encode(3);
    enc.GetByteData(buf1,256);
    dec.SetByteData(buf1,256);
    dec.Decode();
    dec.GetDecData(r1);
    /*std::cout<<r1.data<<std::endl;
    std::cout<<r1.c<<std::endl;
    std::cout<<r1.f<<std::endl;
    std::cout<<r1.ary[0]<<std::endl;
    std::cout<<r1.ary[1]<<std::endl;*/
    std::cout<<enc.SetRawData(t2)<<std::endl;
    enc.Encode(4);
    enc.GetByteData(buf2,256);
    dec.DeInit();
    dec.SetByteData(buf2,256);
    std::cout<<dec.Decode()<<std::endl;
    dec.GetDecData(r2);
    /*r2.pri();*/
    return 0;
}