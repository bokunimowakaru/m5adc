/*******************************************************************************
Example 04: ADC for Loopback Tester for M5Stack [SDカード保存]
・A/Dコンバータの読み値の誤差をグラフ表示し、Micro SD カードにデータを保存します

　adc_19700101000000.csv CSV形式データファイル
　adc_19700101000000.bmp 画像ファイル

                                          Copyright (c) 2019-2020 Wataru KUNINO
********************************************************************************
【参考文献】M5Stack_Screen_Capture screenServer.ino
https://github.com/fukuen/M5Stack_Screen_Capture
*******************************************************************************/

#include <M5Stack.h>                            // M5Stack用ライブラリ
#define DAC_PIN 26                              // GPIO 26 ピン(DAC2)
#define ADC_PIN 36                              // GPIO 36 ピン(ADC1_0)

void setup(){                                   // 起動時に一度だけ実行する関数
    M5.begin();                                 // M5Stack用ライブラリの起動
    pinMode(DAC_PIN, OUTPUT);                   // GPIO26を出力に
    pinMode(ADC_PIN, ANALOG);                   // GPIO36をアナログ入力に
}

void loop(){                                    // 繰り返し実行する関数
    int dac, adc, x, y, prev;                   // 変数dacとadc,x,yを定義

    M5.Lcd.fillScreen(BLACK);                   // LCDを消去
    M5.Lcd.setCursor(2, 2, 2);
    M5.Lcd.drawRect(0, 0, 320, 232, BLUE);      // 座標0,0から320x234の箱を描画
    for(y = 0; y < 231; y += 29) M5.Lcd.drawLine(0,y,319,y, BLUE);  // 罫線Y描画
    for(x = 0; x < 319; x += 40) M5.Lcd.drawLine(x,0,x,231, BLUE);  // 罫線X描画

    char csvfile[40];
    char filename[40];
    struct tm *timenow;
    time_t now = time(NULL);
    timenow = gmtime(&now);
    strftime(filename, sizeof(filename), "%Y%m%d%H%M%S.csv", timenow);
    sprintf(csvfile, "%s_%s", "/adc_", filename);
    File file = SD.open(csvfile, FILE_WRITE);
    prev = -1;
    for(x = 0; x < 320; x++){                   // 変数x=0～319まで繰り返し
        dac = 255 * x / 319;                    // DAC出力値(0～255)を設定
        dacWrite(DAC_PIN, dac);                 // 変数dacの値をDAC出力
        adc = analogRead(ADC_PIN);              // ADC値をadcへ代入
        M5.Lcd.drawPixel(x, 232 - 232 * dac / 255, GREEN);  // DAC値をプロット
        M5.Lcd.drawPixel(x, 232 - 232 * adc /4095, WHITE);  // ADC値をプロット
        y = 116 - adc / 16 + dac;               // DAC出力とADC入力の誤差を計算
        M5.Lcd.drawPixel(x, 116, RED);          // 誤差0の値(80)をプロット
        M5.Lcd.drawPixel(x, y, WHITE);          // 誤差値をプロット
        if(file && prev != dac){
            char s[256];
            snprintf(s, 256, "%d, %d\n", 16 * dac, adc);
            file.print(s);
            prev = dac;
        }
    }
    if(file){
        file.close();
        bmpScreenServer("/adc");                // スクリーンショットを保存
    }else{
        M5.Lcd.println("SD Card ERROR");
        delay(1000);
    }
    M5.Speaker.begin();                         // M5Stack用スピーカの起動
    M5.Speaker.tone(440);                       // スピーカ出力 440Hzの音を出力
    delay(100);                                 // 100msの待ち時間処理
    M5.Speaker.end();                           // スピーカ出力を停止する
    M5.Lcd.println("Press the center button [B]");
    while(!M5.BtnB.read()) delay(10);           // Bボタンが押されるまで待機
}
