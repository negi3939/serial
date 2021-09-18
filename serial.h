#ifndef _SERIAL_H_
#define _SERIAL_H_

class Serial{
    protected:
        int fd;
        unsigned char *buf;
        struct termios tio,oldtio;                      // シリアル通信設定
        struct timeval start_time,init_time,end_time;   //時間
        int baudRate;                                   //速度設定
        char *portname;                                 //デバイスファイル名
        std::ofstream log;                              //ファイル出力
        uint8_t *crc8ccit_table;                        //CRC計算用テーブル
        uint8_t crc8ccit_poly;
    public:
        Serial();
        Serial(int baundrate);
        Serial(int baundrate,char *devname);
        int init();                                                                         //ポートの初期化
        void gen_crc8ccit_table();                                                          //CRCテーブル作成
        uint8_t calc_crc8ccit(uint8_t *data,uint8_t len);                                   //CRC計算
        virtual int read_trush();                                                           //read表示だけ
        virtual int read_get(uint8_t *buf8t,int len);                                       //read()から取得
        virtual int read_get(uint8_t *buf8t,int len,uint8_t headbyte);                      //read()から取得　ヘッダ指定
        virtual int read_get(uint8_t *buf8t,int len,int timeoutms);                         //read()から取得　ヘッダ指定
        virtual int write_string(std::string str);                                          //文字列書き込み
        int close_s();          
        ~Serial();        
};

#endif