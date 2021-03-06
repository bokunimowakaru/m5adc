/*******************************************************************************
Example 04: ADC for Loopback Tester for M5Stack
・A/Dコンバータの読み値の誤差をグラフ表示します
・起動時にD/Aコンバータの出力を使って校正値を保持し、補正します
　（DAC_PINとADC_PINをジャンパ線で接続していてください。）

                                          Copyright (c) 2019-2021 Wataru KUNINO
********************************************************************************
【参考文献】
Arduino IDE 開発環境イントール方法：
https://github.com/m5stack/M5Stack/blob/master/docs/getting_started_ja.md
https://docs.m5stack.com/#/en/related_documents/Arduino_IDE

M5Stack Arduino Library API 情報：
https://docs.m5stack.com/#/ja/api
https://docs.m5stack.com/#/en/arduino/arduino_api
*******************************************************************************/

#include <M5Stack.h>                            // M5Stack用ライブラリ
#define DAC_PIN 26                              // GPIO 26 ピン(DAC2)
#define ADC_PIN 36                              // GPIO 36 ピン(ADC1_0)
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
    M5.begin();                                 // M5Stack用ライブラリの起動
    pinMode(DAC_PIN, OUTPUT);                   // GPIO26を出力に
    pinMode(ADC_PIN, ANALOG);                   // GPIO36をアナログ入力に
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
    M5.Lcd.drawRect(0, 0, 320, 232, BLUE);      // 座標0,0から320x234の箱を描画
    for(y = 0; y < 231; y += 29) M5.Lcd.drawLine(0,y,319,y, BLUE);  // 罫線Y描画
    for(x = 0; x < 319; x += 40) M5.Lcd.drawLine(x,0,x,231, BLUE);  // 罫線X描画

    /* グラフ作成部 */
    for(x = 0; x < 320; x++){                   // 変数x=0～319まで繰り返し
        dac = 255 * x / 319;                    // DAC出力値(0～255)を設定
        dacWrite(DAC_PIN, dac);                 // 変数dacの値をDAC出力
        adc = analogRead(ADC_PIN);              // ADC値をadcへ代入
        cor = correct(adc);                     // 補正後のADC値をcorへ代入
        M5.Lcd.drawPixel(x, 232 - 232 * dac / 255, GREEN);  // DAC値をプロット
        M5.Lcd.drawPixel(x, 232 - 232 * adc /4095, LIGHTGREY); // ADC値プロット
        M5.Lcd.drawPixel(x, 232 - 232 * cor /4095, WHITE);  // COR値をプロット
        
        M5.Lcd.drawPixel(x, 116, RED);          // 誤差0の直線をプロット
        y = 116 - adc / 16 + dac;               // DAC出力とADC入力の誤差を計算
        M5.Lcd.drawPixel(x, y, LIGHTGREY);      // 誤差値をプロット
        y = 116 - cor / 16 + dac;               // DAC出力とCOR値の誤差を計算
        M5.Lcd.drawPixel(x, y, WHITE);          // 誤差値をプロット
    }
    delay(1000);
}
