#ifndef UART_protocol_hpp
#define UART_protocol_hpp

#include <iostream>
#include <Arduino.h>

inline namespace UARTprotocol{
    class UARTEncoder{
        public:
        UARTEncoder(uint8_t);
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
        int Encode(uint8_t adr);
        int GetByteData(uint8_t*,size_t);

        template<typename T>
        int SendData(HardwareSerial& u,T obj,uint8_t adr){
            int status = 0;
            status = SetRawData(obj);
            if(!status)return status;
            status = Encode(adr);
            if(!status)return status;
            uint8_t buf[256] = {0};
            size_t size = GetByteData(buf,256);
            u.write(buf,size);
            return 0;
        }

        private:
        uint8_t src_data[256] = {0};
        uint8_t enc_data[256] = {0};
        size_t src_size = 0;
        size_t enc_size = 0;
        uint8_t dev_number = 0;
    };

    class UARTDecoder{
        private:
        #define BUF_SIZE 384

        public:
        UARTDecoder(uint8_t);
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

        template<typename T>
        int RecvData(HardwareSerial& u,T& obj){
            int status = 0;
            while(!status){
                uint8_t buf[BUF_SIZE] = {0};
                size_t s = u.readBytes(buf,BUF_SIZE);
                status = SetByteData(buf,s);
                if(!status)break;
                status = Decode();
                if(status == 2)status = 0;
            }
            if(status != 1)return status;
            return GetDecData(obj);
        }

        private:
        uint8_t src_data[BUF_SIZE] = {0x01};
        int itr_b = 0,itr_e = 0,cursor = 0;
        uint8_t dec_data[BUF_SIZE] = {0x01};
        size_t dec_size = 0;
        uint8_t dev_number = 0;
    };
}

#endif