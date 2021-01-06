/*******************************************************************************
Example 04: ADC for Loopback Tester for M5StickC Plus
・A/Dコンバータの読み値の誤差をグラフ表示します

                                          Copyright (c) 2019-2020 Wataru KUNINO
********************************************************************************
【参考文献】
https://docs.m5stack.com/#/en/core/m5stickc_plus
https://github.com/m5stack/M5StickC-Plus

【M5StickC Plue の注意点】
・電源ONは電源ボタン2秒長押し、OFFは6秒。
・対応ボーレート：1200 ~115200, 250K, 500K, 750K, 1500K
・G36/G25ポートが同一端子に接続されている。使用しないポートをオープンに設定する
*******************************************************************************/

#include <M5StickCPlus.h>                       // M5StickCPlus用ライブラリ
#define DAC_PIN 26                              // HAT部の GPIO 26 ピン(DAC2)
#define ADC_PIN 36                              // HAT部の GPIO 36 ピン(ADC1_0)

void setup(){                                   // 起動時に一度だけ実行する関数
    M5.begin();                                 // M5StickC用ライブラリの起動
    pinMode(DAC_PIN, OUTPUT);                   // HAT部のGPIO26を出力に
    pinMode(ADC_PIN, ANALOG);                   // HAT部のGPIO36をアナログ入力に
    gpio_pulldown_dis(GPIO_NUM_25);             // M5StickC PlusでG36を使用する
    gpio_pullup_dis(GPIO_NUM_25);                  // ときはG25をオープンにする
}

void loop(){                                    // 繰り返し実行する関数
    int dac, adc, x, y;                         // 変数dacとadc,x,yを定義

    /* 罫線描画部 */
    M5.Lcd.fillScreen(BLACK);                   // LCDを消去
    M5.Lcd.drawRect(7, 0, 120, 240, BLUE);      // 座標0,0から135x240の箱を描画
    for(y = 0; y < 239; y += 15) M5.Lcd.drawLine(7,y,127,y, BLUE); // 罫線Y描画
    for(x = 7; x < 134; x += 15) M5.Lcd.drawLine(x,0,x,239, BLUE); // 罫線X描画

    /* グラフ作成部 */
    for(dac = 0; dac < 256; dac++){             // 変数dac=0～255まで繰り返し
        dacWrite(DAC_PIN, dac);                 // 変数dacの値をDAC出力
        adc = analogRead(ADC_PIN);              // ADC値をadcへ代入
        x = dac * 120 / 255 + 7;                // X軸の位置を計算
        M5.Lcd.drawPixel(x, 240 - 240 * dac / 255, GREEN);  // DAC値をプロット
        M5.Lcd.drawPixel(x, 240 - 240 * adc /4095, WHITE);  // ADC値をプロット
        y = 120 - adc / 16 + dac;               // DAC出力とADC入力の誤差を計算
        M5.Lcd.drawPixel(x, 120, RED);          // 誤差0の直線をプロット
        M5.Lcd.drawPixel(x, y, WHITE);          // 誤差値をプロット
    }
    delay(1000);
}
