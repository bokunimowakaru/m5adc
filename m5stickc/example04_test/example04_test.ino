/*******************************************************************************
Example 04: ADC for Loopback Tester for M5StickC
・A/Dコンバータの読み値の誤差をグラフ表示します

                                          Copyright (c) 2019-2020 Wataru KUNINO
********************************************************************************
【参考文献】
Arduino IDE 開発環境イントール方法：
https://github.com/m5stack/M5Stack/blob/master/docs/getting_started_ja.md
https://docs.m5stack.com/#/en/related_documents/Arduino_IDE

M5Stack Arduino Library API 情報：
https://docs.m5stack.com/#/ja/api
https://docs.m5stack.com/#/en/arduino/arduino_api
*******************************************************************************/

#include <M5StickC.h>                           // M5StickC用ライブラリ
#define DAC_PIN 26                              // HAT部の GPIO 26 ピン(DAC2)
#define ADC_PIN 36                              // HAT部の GPIO 36 ピン(ADC1_0)

void setup(){                                   // 起動時に一度だけ実行する関数
    M5.begin();                                 // M5StickC用ライブラリの起動
    pinMode(DAC_PIN, OUTPUT);                   // HAT部のGPIO26を出力に
    pinMode(ADC_PIN, ANALOG);                   // HAT部のGPIO36をアナログ入力に
}

void loop(){                                    // 繰り返し実行する関数
    int dac, adc, x, y;                         // 変数dacとadc,x,yを定義

    /* 罫線描画部 */
    M5.Lcd.fillScreen(BLACK);                   // LCDを消去
    M5.Lcd.drawRect(0, 0, 80, 160, BLUE);       // 座標0,0から80x160の箱を描画
    for(y = 0; y < 159; y += 20) M5.Lcd.drawLine(0,y,79,y, BLUE);   // 罫線Y描画
    for(x = 0; x < 79; x += 20) M5.Lcd.drawLine(x,0,x,159, BLUE);   // 罫線X描画

    /* グラフ作成部 */
    for(dac = 0; dac < 256; dac++){             // 変数dac=0～255まで繰り返し
        dacWrite(DAC_PIN, dac);                 // 変数dacの値をDAC出力
        adc = analogRead(ADC_PIN);              // ADC値をadcへ代入
        x = dac * 80 / 255;                     // X軸の位置を計算
        M5.Lcd.drawPixel(x, 160 - 160 * dac / 255, GREEN);  // DAC値をプロット
        M5.Lcd.drawPixel(x, 160 - 160 * adc /4095, WHITE);  // ADC値をプロット
        y = 80 - adc / 16 + dac;                // DAC出力とADC入力の誤差を計算
        M5.Lcd.drawPixel(x, 80, RED);           // 誤差0の値(80)をプロット
        M5.Lcd.drawPixel(x, y, WHITE);          // 誤差値をプロット
    }
    delay(1000);
}
