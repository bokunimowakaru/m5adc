/*******************************************************************************
Example 04: ADC for Loopback Tester for M5StickC Plus
・A/Dコンバータの読み値の誤差をグラフ表示します
・起動時にD/Aコンバータの出力を使って校正値を保持し、補正します
　（DAC_PINとADC_PINをジャンパ線で接続していてください。）

                                          Copyright (c) 2019-2021 Wataru KUNINO
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
uint16_t corv[256];                             // 校正値の保存用

int correct(int adc){                           // 補正用の関数
    int i;
    for(i = 0; i < 255; i++){
        if(corv[i] > adc) break;
    }
    if(i == 0) return 0;
    float a = 4095. / 255. / ((float)corv[i] - (float)corv[i-1]);
    float b = (float)i * 4095. / 255. - a * (float)corv[i];
    return (int)(a * adc + b + 0.5);
}

void setup(){                                   // 起動時に一度だけ実行する関数
    M5.begin();                                 // M5StickC用ライブラリの起動
    pinMode(DAC_PIN, OUTPUT);                   // HAT部のGPIO26を出力に
    pinMode(ADC_PIN, ANALOG);                   // HAT部のGPIO36をアナログ入力に
    gpio_pulldown_dis(GPIO_NUM_25);             // M5StickC PlusでG36を使用する
    gpio_pullup_dis(GPIO_NUM_25);                  // ときはG25をオープンにする
    for(int i=0; i < 256; i++){                 // 256回、繰り返す
        dacWrite(DAC_PIN, i);                   // DAC出力
        corv[i] = analogRead(ADC_PIN);          // 補正用AD変換値を保存
        Serial.printf("corv[%d]=%d\n",i,corv[i]); // 補正値をシリアル出力
    }
}

void loop(){                                    // 繰り返し実行する関数
    int dac, adc, cor, x, y;                    // 変数dacとadc,cor,x,yを定義

    /* 罫線描画部 */
    M5.Lcd.fillScreen(BLACK);                   // LCDを消去
    M5.Lcd.drawRect(7, 0, 120, 240, BLUE);      // 座標0,0から135x240の箱を描画
    for(y = 0; y < 239; y += 15) M5.Lcd.drawLine(7,y,127,y, BLUE); // 罫線Y描画
    for(x = 7; x < 134; x += 15) M5.Lcd.drawLine(x,0,x,239, BLUE); // 罫線X描画

    /* グラフ作成部 */
    for(dac = 0; dac < 256; dac++){             // 変数dac=0～255まで繰り返し
        dacWrite(DAC_PIN, dac);                 // 変数dacの値をDAC出力
        adc = analogRead(ADC_PIN);              // ADC値をadcへ代入
        cor = correct(adc);                     // 補正後のADC値をcorへ代入
        x = dac * 120 / 255 + 7;                // X軸の位置を計算
        M5.Lcd.drawPixel(x, 240 - 240 * dac / 255, GREEN);  // DAC値をプロット
        M5.Lcd.drawPixel(x, 240 - 240 * adc /4095, LIGHTGREY);  // ADC値をプロット
        M5.Lcd.drawPixel(x, 240 - 240 * cor /4095, WHITE);  // ADC値をプロット

        M5.Lcd.drawPixel(x, 120, RED);          // 誤差0の直線をプロット
        y = 120 - adc / 16 + dac;               // DAC出力とADC入力の誤差を計算
        M5.Lcd.drawPixel(x, y, LIGHTGREY);      // 誤差値をプロット
        y = 120 - cor / 16 + dac;               // DAC出力とCOR値の誤差を計算
        M5.Lcd.drawPixel(x, y, WHITE);          // 誤差値をプロット
    }
    delay(1000);
}
