/*******************************************************************************
Example 02: ADC Input Voltage Checker for M5StickC Plus
・A/Dコンバータの読み値をLCDに数値で表示します

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
#define ADC_PIN 36                              // HAT部の GPIO 36 ピン(ADC1_0)

void setup(){                                   // 起動時に一度だけ実行する関数
    M5.begin();                                 // M5StickC用ライブラリの起動
    pinMode(ADC_PIN, ANALOG);                   // HAT部のGPIO36をアナログ入力に
    gpio_pulldown_dis(GPIO_NUM_25);             // M5StickC PlusでG36を使用する
    gpio_pullup_dis(GPIO_NUM_25);                  // ときはG25をオープンにする
}

void loop(){                                    // 繰り返し実行する関数
    int i, adc, mv;                             // 変数iとadc、mvを定義
    M5.Lcd.fillScreen(BLACK);                   // LCDを消去
    for(i = 0; i < 11; i++){                    // 変数iが10未満で以下を繰り返し
        adc = analogRead(ADC_PIN);              // ADC値をadcへ代入
        mv = adc * 3300 / 4095;                 // ADC値を電圧に変換してmvへ代入
        M5.Lcd.setCursor(12, 21 * i + 4, 4);    // 文字座標と文字大(4倍)を設定
        M5.Lcd.println(mv);                     // 電圧値を表示
        delay(500);                             // 0.5秒（500ms）の待ち時間処理
    }
}
