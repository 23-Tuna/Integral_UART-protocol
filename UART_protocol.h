#ifndef UART_protocol_hpp
#define UART_protocol_hpp

#include <iostream>

inline namespace UARTprotocol{
    class UARTEncoder{
        public:
        template<typename T>
        int SetRawData(T obj){
            src_size = sizeof(obj);
            if(src_size>256){
                return -1;
            }else{
                memcpy(src_data,&obj,src_size);
                return 0;
            }
        }
        template<typename T>
        int GetRawData(T& obj){
            if(sizeof(T) == src_size){
                memcpy(&obj,src_data,src_size);
                return 0;
            }else{
                return -1;
            }
        }
        int Encode();
        int GetByteData(uint8_t*,size_t);

        private:
        uint8_t src_data[256] = {0};
        uint8_t enc_data[256] = {0};
        size_t src_size = 0;
        size_t enc_size = 0;
    };

    class UARTDecoder{
        public:
        int SetByteData(uint8_t*,size_t);
        int Decode();
        template<typename T>
        int GetDecData(T& obj){
            if(sizeof(T) == dec_size){
                memcpy(&obj,dec_data,dec_size);
                return 0;
            }
            return -1;
        }
        int DeInit();

        private:
        #define BUF_SIZE 384
        uint8_t src_data[BUF_SIZE] = {0x01};
        int itr_b = 0,itr_e = 0,cursor = 0;
        uint8_t dec_data[BUF_SIZE] = {0x01};
        size_t dec_size = 0;
    };
}

#endif