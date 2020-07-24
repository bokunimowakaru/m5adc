/*******************************************************************************
Example 05: ADS1100 Input Voltage Meter for M5StickC
・M5Stack製オプション ADC HAT (ADS1100)を使用
・A/Dコンバータ ADS1100 の読み値をアナログ・メータ表示します

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
#include "lib_ADS1100.h"                        // ADS1100用ライブラリ

ADS1100 ads1100;                                // 変数(オブジェクト)ads1100生成
int range = 0;

void setup(){                                   // 起動時に一度だけ実行する関数
    M5.begin();                                 // M5Stack用ライブラリの起動
    ads1100.getAddr_ADS1100(0x48);              // I2Cアドレスを0x48に設定
    ads1100.begin();
    M5.Axp.ScreenBreath(7 + 3);                 // LCDの輝度を3に設定
}

void loop(){                                    // 繰り返し実行する関数
    int adc;                                    // 変数adcを定義
    float mv;                                   // 浮動小数点数型変数mvを定義
    adc = ads1100.Measure_Differential();       // ADC値をadcへ代入
    mv = 4. * adc * 3300. / 32768.;             // ADC値を電圧に変換してmvへ代入
    Serial.printf("adc=%d, mv=%f\n", adc, mv);  // ADC値と電圧値mvをシリアル出力
    if(mv <= 4000. && range != 4000){               // 電圧値が4000mV以下のとき
        range = 4000;                               // レンジを4000mVに
        analogMeterInit("mV", "ADS1100", 0, 4000);  // メータのレンジ設定
    }else if(mv > 4000. && mv <= 6000. && range != 6000){   // 電圧値4000～6000
        range = 6000;                               // レンジを6000mVに
        analogMeterInit("mV", "ADS1100", 0, 6000);  // メータのレンジ設定
    }else if(mv > 6000. && range != 12000){         // 電圧値が6000mV超のとき
        range = 12000;                              // レンジを12000mVに
        analogMeterInit("mV", "ADS1100", 0, 12);    // メータのレンジ設定
    }
    if(range == 12000) mv /= 1000.;             // 12Vレンジのときmvを1000で除算
    analogMeterNeedle(mv);                      // ADCの電圧値をメータ表示
    delay(500);                                 // 0.5秒（500ms）の待ち時間処理
}
