# 単に実行
 $ make
 $ ./serial.exe で実行する.実行後にopen errorと出た場合はポートを確認．またfalseと出た場合はボーレートや相手のパケットを確認
 $ ./serial.exe /dev/ttyS1 B115200 でポートやボーレートを指定可能

# class Serialを使う
## コンストラクタ
 ボーレートやポート名を引数にする．ポート名はchar*かstringで指定．
 crc計算の定数を指定することも可能．
## シリアル受信
 read_trush()は受信したデータを生でターミナルに出力．相手が文字コードで送信する場合はこれで確認．
 read_get(uint8_t *buf8t,int len,uint8_t headbyte)では先頭バイトを指定して受信を行う．
 read_get(uint8_t *buf8t,int len)でパケットのバイト数を指定して取得．ただしパケットの末尾はcrc8を入れる前提．crc8の計算が合わない場合は0を返す．
 read_get(uint8_t *buf8t,int len,int timeoutms)ではさらに指定したmsの時間でタイムアウトを行う．
## シリアル送信 
 write_string(std::string str)で文字列をcharに変換して送信．
 write_raw(uint8_t *buf8t,int len)はそのまま指定したバイト数を送信．
 write_wcrc(uint8_t *buf8t,int len)はパケットの最後をcrcの計算結果に書き換えて送信．