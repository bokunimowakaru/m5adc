/*******************************************************************************
Example 02: ADC Input Voltage Checker for M5StickC
・A/Dコンバータの読み値をLCDに数値で表示します

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
#define ADC_PIN 36                              // HAT部の GPIO 36 ピン(ADC1_0)

void setup(){                                   // 起動時に一度だけ実行する関数
    M5.begin();                                 // M5Stack用ライブラリの起動
    M5.Axp.ScreenBreath(7 + 3);                 // LCDの輝度を3に設定
}

void loop(){                                    // 繰り返し実行する関数
    int i, adc, mv;                             // 変数iとadc、mvを定義
    M5.Lcd.fillScreen(BLACK);                   // LCDを消去
    for(i = 0; i < 7; i++){                     // 変数iが6未満で以下を繰り返し
        adc = analogRead(ADC_PIN);              // ADC値をadcへ代入
        mv = adc * 3300 / 4095;                 // ADC値を電圧に変換してmvへ代入
        M5.Lcd.setCursor(12, 22 * i + 2, 4);    // 文字座標と文字大(4倍)を設定
        M5.Lcd.println(mv);                     // 電圧値を表示
        delay(500);                             // 0.5秒（500ms）の待ち時間処理
    }
}
