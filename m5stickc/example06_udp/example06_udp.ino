/*******************************************************************************
Example 06: ADC Input Voltage UDP Sender for M5StickC
・A/Dコンバータの読み値をWi-Fi UDP送信します

                                          Copyright (c) 2019-2020 Wataru KUNINO
*******************************************************************************/

#include <M5StickC.h>                           // M5StickC用ライブラリ
#include <WiFi.h>                               // ESP32用WiFiライブラリ
#include <WiFiUdp.h>                            // UDP通信を行うライブラリ
#define SSID "iot-core-esp32"                   // 無線LANアクセスポイントのSSID
#define PASS "password"                         // パスワード
#define PORT 1024                               // 送信のポート番号
#define DEVICE "adcnv_5,"                       // デバイス名(5字+"_"+番号+",")
#define ADC_PIN 36                              // HAT部の GPIO 36 ピン(ADC1_0)
IPAddress IP;                                   // ブロードキャストIP保存用

void setup(){                                   // 起動時に一度だけ実行する関数
    M5.begin();                                 // M5StickC用ライブラリの起動
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
    int adc;                                    // 変数adcを定義
    float mv;                                   // 浮動小数点数型変数mvを定義
    adc = analogRead(ADC_PIN);                  // ADC値をadcへ代入
    mv = (float)adc * 3300. / 4095.;            // ADC値を電圧に変換してmvへ代入
    
    /* LCD表示 */
    M5.Lcd.fillScreen(BLACK);                   // LCDを消去
    M5.Lcd.setCursor(0, 4, 1);                  // 文字座標と文字サイズ1を設定
    M5.Lcd.println(IP);                         // UDP送信先IPアドレスを表示
    M5.Lcd.setCursor(0, 40, 2);                 // 文字座標と文字サイズ2を設定
    M5.Lcd.drawRect(0, 140, 80, 16, WHITE);     // バー表示枠を描画
    M5.Lcd.fillRect(2, 142, 76 * adc / 4095, 12, GREEN);    // バー表示
    M5.Lcd.printf("adc\n%d\n\n", adc);          // ADC値をLCDに表示
    M5.Lcd.printf("voltage\n%.3f V", mv / 1000); // 電圧値VをLCDに表示

    /* UDP送信 */
    WiFiUDP udp;                                // UDP通信用の変数(オブジェクト)
    udp.beginPacket(IP, PORT);                  // UDP送信先を設定
    udp.printf("%s%f\n", DEVICE, mv / 1000);    // デバイス名と電圧値(V)を送信
    udp.endPacket();                            // UDP送信の終了(実際に送信する)
    delay(5000);                                // 5秒の待ち時間処理
}
