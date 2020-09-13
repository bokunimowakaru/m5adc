/*******************************************************************************
Example 08: Wi-Fi Illumination Sensor for M5Stack
・照度センサ NJL7502L (新日本無線) の読み値をクラウドサービスAmbientへ送信します

                                          Copyright (c) 2019-2020 Wataru KUNINO
*******************************************************************************/

#include <M5Stack.h>                            // M5Stack用ライブラリ
#include <WiFi.h>                               // ESP32用WiFiライブラリ
#include <HTTPClient.h>                         // HTTPクライアント用ライブラリ
#define SSID "iot-core-esp32"                   // 無線LANアクセスポイントのSSID
#define PASS "password"                         // パスワード
#define Amb_Id "00000"                          // AmbientのチャネルID 
#define Amb_Key "0000000000000000"              // Ambientのライトキー
#define ADC_PIN 36                              // GPIO 36 ピン(ADC1_0)
#define LOAD_KOHM 1.000                         // 照度センサの負荷抵抗
int count = 0;                                  // 送信タイミング用カウンタ

void setup(){                                   // 起動時に一度だけ実行する関数
    M5.begin();                                 // M5Stack用ライブラリの起動
    M5.Lcd.setBrightness(100);                  // LCDの輝度を100に設定
    pinMode(ADC_PIN, ANALOG);                   // GPIO36 を アナログ入力に
    delay(500);                                 // 電源安定待ち時間処理0.5秒
    WiFi.mode(WIFI_STA);                        // 無線LANを【子機】モードに設定
    WiFi.begin(SSID,PASS);                      // 無線LANアクセスポイントへ接続
    while(WiFi.status() != WL_CONNECTED){       // 接続に成功するまで待つ
        delay(500);                             // 待ち時間処理
        M5.Lcd.print('.');                      // 進捗表示
    }
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
    if(count < 100) return;                     // 変数countが100未満ならloopへ

    HTTPClient http;                            // HTTPリクエスト用インスタンス
    http.begin(                                 // アクセス先のURLを設定
        "http://ambidata.io/api/v2/channels/"   // URL
        + String(Amb_Id)                        // チャネルID
        + "/data"                               // データ送信
    );
    http.addHeader(                             // HTTPヘッダを設定
        "Content-Type",                         // (項目名)データ形式
        "application/json"                      // 　　(値)JSON
    );
    i = http.POST(                              // HTTP POSTを実行
        "{\"writeKey\":\""                      // (項目名)writeKey
        + String(Amb_Key)                       // 　　(値)Ambientのライトキー
        + "\",\"d1\":\""                        // (項目名)d1
        + String(lux)                           // 　　(値)照度値
        + "\"}"
    );
    if(i != 200) M5.Lcd.printf("E(%d) ",i);     // エラー表示
    http.end();                                 // HTTPリクエストを終了
    M5.Lcd.printf("%.0f ",lux);                 // 送信した照度値を表示
    count = 0;                                  // カウンタ変数countに0を代入
}
