#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#include <iomanip>


#include "keyboard.h"
#include "serial.h"


Serial::Serial(){
    baudRate = B9600;
    buf = new unsigned char[255];
    portname = new char[12];
    portname = "/dev/ttyS4";
    init();
}

Serial::Serial(int baundrate){
    baudRate = baundrate;
    buf = new unsigned char[255];
    init();
}
Serial::Serial(int baundrate,char *devname){
    buf = new unsigned char[255];
    int sizename = 30;
    portname = new char[sizename];
    for(int ii=0;ii<sizename;ii++){
        portname[ii] = devname[ii];
    }
    std::cout << portname << std::endl;
    baudRate = baundrate;
    init();
}

int Serial::init(){

    fd = open(portname, O_RDWR);
    if (fd < 0){
        std::cout<< "open error!" <<std::endl;
        return -1;
    }
    tio.c_cflag += CREAD;               // 受信有効
    tio.c_cflag += CLOCAL;              // ローカルライン（モデム制御なし）
    tio.c_cflag += CS8;                 // データビット:8bit
    tio.c_cflag += 0;                   // ストップビット:1bit
    tio.c_cflag += 0;                   // パリティ:None

    cfsetispeed( &tio, baudRate );   
    cfsetospeed( &tio, baudRate );

    cfmakeraw(&tio);                    // RAWモード

    tcsetattr( fd, TCSANOW, &tio );     // デバイスに設定を行う

    ioctl(fd, TCSETA, &tio);

    return 0;
}

int Serial::read_s(){
    int len,finishf=1;
    long count = 0;
    int offset=0;
    while(finishf) {
        len = read(fd, buf, sizeof(buf));
        for(int ii = 0; ii < len; ii++) {
            std::cout << buf[ii] << std::flush;
            if(buf[ii]=='\n'){
                finishf = 0;
            }   
        }
    }
    std::cout << std::endl;
    return 0;
}

int Serial::read_s(uint8_t *buf8t,int len,uint8_t headbyte){
    int leng = 0,finishf=1;
    long count = 0;
    while(leng<len){
        if(leng<1){
            leng = read(fd, buf8t, 1);
            if(buf8t[0]!=headbyte){
                leng = 0;
            }
        }else{
            leng += read(fd, buf8t+leng, len-leng);
        }
    }
   
    for(int ii = 0; ii < leng; ii++) {
        std::cout << std::setfill('0') << std::setw(2) << std::hex << (int)buf8t[ii] << std::flush;            
    }

    std::cout << std::endl;

    return 0;
}



int Serial::write_s(std::string str){
    ssize_t ret = 0;
    int num = str.size();
    const char* buff = str.c_str();
    std::string strclear = "\r\n";
    const char* buffcl = strclear.c_str();
    ret = write(fd, buffcl, 2);
    ret = write(fd, buff, num);
    ret = write(fd, buffcl, 2);
    //std::cout << "send:" << buff << std::endl; 
    return ret;
}

int Serial::close_s(){
    close(fd);
    return 0;
}

Serial::~Serial(){
    close_s();
}

#if defined(SERIAL_IS_MAIN)
int main(){
    char devname[] = "/dev/ttyS11";
    int boardrate = B115200;
    Serial *ser = new Serial(boardrate,devname);
    keyboard ky;
    uint8_t buf8t[5];
    uint8_t headbyte=0x0a;
    while(1){
        ser->read_s(buf8t,5,headbyte);
        if(ky.kbhit()){
            break;
        }
    }
    //uniservo->read_s();
    delete ser;
    return 0;
}
#endif