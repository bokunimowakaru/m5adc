/*******************************************************************************
Example 07: Wi-Fi Illumination Sensor for M5StickC Wi-Fi
・照度センサ NJL7502L (新日本無線) の読み値を Wi-Fi UDP送信します

                                          Copyright (c) 2019-2020 Wataru KUNINO
*******************************************************************************/

#include <M5StickC.h>                           // M5StickC用ライブラリ
#include <WiFi.h>                               // ESP32用WiFiライブラリ
#include <WiFiUdp.h>                            // UDP通信を行うライブラリ
#define SSID "iot-core-esp32"                   // 無線LANアクセスポイントのSSID
#define PASS "password"                         // パスワード
#define PORT 1024                               // 送信のポート番号
#define DEVICE "illum_5,"                       // デバイス名(5字+"_"+番号+",")
#define ADC_PIN 36                              // HAT部の GPIO 36 ピン(ADC1_0)
#define LOAD_KOHM 1.000                         // 照度センサの負荷抵抗
IPAddress IP;                                   // ブロードキャストIP保存用
int range = 0;                                  // 自動レンジ用
int count = 0;                                  // UDP送信タイミング用カウンタ
float sum = 0.;                                 // 照度の総和保存用

void setup(){                                   // 起動時に一度だけ実行する関数
    M5.begin();                                 // M5StickC用ライブラリの起動
    M5.Axp.ScreenBreath(7 + 3);                 // LCDの輝度を3に設定
    pinMode(ADC_PIN, ANALOG);                   // HAT部のGPIO36をアナログ入力に
    delay(500);                                 // 電源安定待ち時間処理0.5秒
    WiFi.mode(WIFI_STA);                        // 無線LANを【子機】モードに設定
    WiFi.begin(SSID,PASS);                      // 無線LANアクセスポイントへ接続
    while(WiFi.status() != WL_CONNECTED){       // 接続に成功するまで待つ
        delay(500);                             // 待ち時間処理
        M5.Lcd.print('.');                      // 進捗表示
    }
    IP = WiFi.localIP();                        // IPアドレスを取得
    IP[3] = 255;                                // ブロードキャストアドレスに
}

void loop(){                                    // 繰り返し実行する関数
    WiFiUDP udp;                                // UDP通信用の変数(オブジェクト)
    int adc;                                    // 変数adcを定義
    float mv, lux;                              // 変数mvとluxを定義
    adc = analogRead(ADC_PIN);                  // ADC値をadcへ代入
    mv = (float)adc * 3300. / 4095.;            // ADC値を電圧に変換してmvへ代入
    lux = 100. * mv / LOAD_KOHM / 33.;          // 照度値を計算
    sum += lux;                                 // 変数sumに照度を加算
    Serial.printf("adc=%d, mv=%f, ", adc, mv);  // ADC値と電圧値mvをシリアル出力
    Serial.printf("lux=%f, avr=%f\n", lux, sum / (float)count);    // 照度を出力
    if((lux <= 900. && range != 3) || range == 0){  // 照度値が900(lx)以下時
        range = 3;                                  // レンジを10^3に
        analogMeterInit("lx", "Illum.", 0, 1000);   // メータのレンジ設定
    }
    if(lux >= 1000. && range != 4){                 // 照度値1000(lx)以上時
        range = 4;                                  // レンジを10^4に
        analogMeterInit("lx", "Illum.", 0, 10000);  // メータのレンジ設定
    }
    analogMeterNeedle(lux);                     // 照度値をメータ表示
    if(count >= 50){                            // カウンタ変数countが50に到達
        udp.beginPacket(IP, PORT);              // UDP送信先を設定
        udp.printf("%s%.3f\n", DEVICE, sum / 50.);  // デバイス名と照度値を送信
        udp.endPacket();                        // UDP送信の終了(実際に送信する)
        count = 0;                              // カウンタ変数countに0を代入
        sum = 0.;                               // 照度平均値を0にリセット
    }
    count++;                                    // カウンタ変数countに1を加算
    delay(100);                                 // 0.1秒（100ms）の待ち時間処理
}
