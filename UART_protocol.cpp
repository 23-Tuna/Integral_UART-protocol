#include "UART_protocol.hpp"
#include <cstring>

inline namespace UARTprotocol{

    template<typename T>
    int UARTEncoder::SetRawData(T obj){
        src_size = sizeof(obj);
        if(src_size>256){
            return -1;
        }else{
            memcpy(src_data,&obj,src_size);
            return 0;
        }
    }

    template<typename T>
    int UARTEncoder::GetRawData(T& obj){
        if(sizeof(T) == src_size){
            memcpy(&obj,src_data,src_size);
            return 0;
        }else{
            return -1;
        }
    }

    int UARTEncoder::Encode(){
        if(src_size+4>256){
            return -1;
        }
        enc_size = src_size+4;
        enc_data[0] = 0xFF;
        enc_data[enc_size-1] = 0x00;
        uint8_t mark_0 = 2,count_0 = 1;
        uint8_t mark_f = 1,count_f = 2;
        for(int i = 0;i < src_size;i++){
            if(src_data[i] == 0xFF){
                enc_data[mark_f] = count_f;
                mark_f = i + 3;
                count_f = 1;
                count_0++;
            }else if(src_data[i] == 0x00){
                enc_data[mark_0] = count_0;
                mark_0 = i + 3;
                count_0 = 1;
                count_f++;
            }else{
                enc_data[i+3] = src_data[i];
                count_0++;
                count_f++;
            }
        }
        enc_data[mark_0] = count_0;
        enc_data[mark_f] = count_f;
        for(int i=1;i<enc_size-1;i++){
            if(enc_data[i] == 0xFF || enc_data[i] == 0x00){
                return -2;
            }
        }
        return 0;
    }

    int UARTEncoder::GetByteData(uint8_t* buf,size_t numlen){
        if(numlen < enc_size)return -1;
        memcpy(buf,enc_data,enc_size);
        return enc_size;
    }

    int UARTDecoder::SetByteData(uint8_t* buf,size_t numlen){
        if(numlen > BUF_SIZE)return -1;
        for(int i = 0;i < numlen;i++){
            src_data[cursor] = buf[i];
            cursor=(++cursor)%BUF_SIZE;
        }
        return 0;
    }

    int UARTDecoder::Decode(){
        while(itr_b!=cursor&&src_data[itr_b]!=0xFF)
        itr_b=(++itr_b)%BUF_SIZE;
        if(itr_b==cursor)
        itr_b = (BUF_SIZE + itr_b - 1)%BUF_SIZE;
        while(itr_e!=cursor&&src_data[itr_e]!=0x00)
        itr_e=(++itr_e)%BUF_SIZE;
        if(itr_e==cursor)
        itr_e = (BUF_SIZE + itr_e - 1)%BUF_SIZE;

        if(src_data[itr_b]==0xFF&&src_data[itr_e]==0x00){
            dec_size = (BUF_SIZE + itr_e - itr_b - 3)%BUF_SIZE;
            int mark_f = src_data[itr_b+1]-2;
            int mark_0 = src_data[itr_b+2]-1;
            for(int i = 0;i < dec_size;i++){
                if(mark_0 == 0){
                    dec_data[i] = 0x00;
                    mark_0 = src_data[itr_b+3+i];
                }else if(mark_f == 0){
                    dec_data[i] = 0xFF;
                    mark_f = src_data[itr_b+3+i];
                }else{
                    dec_data[i] = src_data[itr_b+3+i];
                }
                mark_0--,mark_f--;
            }
            while(itr_b!=itr_e){
                src_data[itr_b] = 0x01;
                itr_b=(++itr_b)%BUF_SIZE;
            }
            src_data[itr_b] = 0x01;
            return 1;
        }
        return 0;
    }

    template<typename T>
    int UARTDecoder::GetDecData(T& obj){
        if(sizeof(T) == dec_size){
            memcpy(&obj,dec_data,dec_size);
            return 0;
        }
        return -1;
    }

    int UARTDecoder::DeInit(){
        itr_b = itr_e = cursor = 0;
        memset(src_data,0x01,BUF_SIZE);
        memset(dec_data,0x01,BUF_SIZE);
        dec_size = 0;
        return 0;
    }
}