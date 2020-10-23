/*******************************************************************************
Example 07: Wi-Fi Illumination Sensor for M5StickC Plus
・照度センサ NJL7502L (新日本無線) の読み値を Wi-Fi UDP送信します

                                          Copyright (c) 2019-2020 Wataru KUNINO
*******************************************************************************/

#include <M5StickCPlus.h>                       // M5StickCPlus用ライブラリ
#include <WiFi.h>                               // ESP32用WiFiライブラリ
#include <WiFiUdp.h>                            // UDP通信を行うライブラリ
#define SSID "iot-core-esp32"                   // 無線LANアクセスポイントのSSID
#define PASS "password"                         // パスワード
#define PORT 1024                               // 送信のポート番号
#define DEVICE "illum_5,"                       // デバイス名(5字+"_"+番号+",")
#define ADC_PIN 36                              // HAT部の GPIO 36 ピン(ADC1_0)
#define LOAD_KOHM 1.000                         // 照度センサの負荷抵抗
IPAddress IP;                                   // ブロードキャストIP保存用
int count = 0;                                  // UDP送信タイミング用カウンタ

void setup(){                                   // 起動時に一度だけ実行する関数
    M5.begin();                                 // M5StickC用ライブラリの起動
    M5.Axp.ScreenBreath(7 + 3);                 // LCDの輝度を3に設定
    pinMode(ADC_PIN, ANALOG);                   // HAT部のGPIO36をアナログ入力に
    gpio_pulldown_dis(GPIO_NUM_25);             // M5StickC PlusでG36を使用する
    gpio_pullup_dis(GPIO_NUM_25);                  // ときはG25をオープンにする
    delay(500);                                 // 電源安定待ち時間処理0.5秒
    WiFi.mode(WIFI_STA);                        // 無線LANを【子機】モードに設定
    WiFi.begin(SSID,PASS);                      // 無線LANアクセスポイントへ接続
    while(WiFi.status() != WL_CONNECTED){       // 接続に成功するまで待つ
        delay(500);                             // 待ち時間処理
        M5.Lcd.print('.');                      // 進捗表示
    }
    IP = WiFi.localIP();                        // IPアドレスを取得
    IP[3] = 255;                                // ブロードキャストアドレスに
    analogMeterInit("lx", "Illum.", 0, 1000);   // メータの背景表示
}

void loop(){                                    // 繰り返し実行する関数
    int adc, i;                                 // 変数adcとiを定義
    float mv, lux = 0;                          // 変数mvとluxを定義
    for(i = 0; i < 30; i++){                    // 30回の繰り返し処理
        adc = analogRead(ADC_PIN);              // ADC値をadcへ代入
        mv = (float)adc * 3300. / 4095.;        // ADC値を電圧に変換してmvへ代入
        lux += 100. * mv/ LOAD_KOHM/ 33. /30.;  // 照度値を計算
        delay(10);                              // 0.01秒（10ms）の待ち時間処理
    }                                           // 以上は、約0.3秒の処理
    analogMeterNeedle(lux);                     // 照度値をメータ表示
    count++;                                    // カウンタ変数countに1を加算
    if(count < 10) return;                      // 変数countが10未満ならloopへ
    
    WiFiUDP udp;                                // UDP通信用の変数(オブジェクト)
    udp.beginPacket(IP, PORT);                  // UDP送信先を設定
    udp.printf("%s%.0f\n", DEVICE, lux);        // デバイス名と照度値を送信
    udp.endPacket();                            // UDP送信の終了(実際に送信する)
    count = 0;                                  // カウンタ変数countに0を代入
}
