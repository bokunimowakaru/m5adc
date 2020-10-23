/*******************************************************************************
Example 05: ADS1100 Input Voltage Meter for M5StickC Plus [Auto Range Display]
・M5Stack製オプション ADC HAT (TI ADS1100)を使用
　分圧=100K/(300K+100K) 8SPS時 16bit (単入力・15bit)
・A/Dコンバータ ADS1100 の読み値をアナログ・メータ表示します【自動レンジ対応版】

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
#include <Wire.h>                               // I2C通信用ライブラリ

int range = 0;                                  // 自動レンジ用

void setup(){                                   // 起動時に一度だけ実行する関数
    M5.begin();                                 // M5StickC用ライブラリの起動
    Wire.begin(0,26);                           // I2Cを初期化(GPIO 0, GPIO 26)
    Wire.beginTransmission(0x48);               // ADS1100(0x48)との通信を開始
    Wire.write(0x0C);                           // 連続,16bit-8SPS,Gain=1を設定
    Wire.endTransmission();                     // ADS1100(0x48)との通信を終了
    M5.Axp.ScreenBreath(7 + 3);                 // LCDの輝度を3に設定
}

void loop(){                                    // 繰り返し実行する関数
    int16_t adc = 0x0000;                       // 変数adcを定義
    float mv;                                   // 浮動小数点数型変数mvを定義
    Wire.requestFrom(0x48, 2);                  // 2バイトのデータを要求
    if(Wire.available() >= 2){                  // 2バイト以上を受信
        adc = ((int16_t)Wire.read()) << 8;      // 1バイト目を変数adcの上位へ
        adc |= (int16_t)Wire.read();            // 2バイト目を変数adcの下位へ
    }
    mv = 4. * (float)adc * 3300. / 32767.;      // ADC値を電圧に変換してmvへ代入
    Serial.printf("adc=%d, mv=%f\n", adc, mv);  // ADC値と電圧値mvをシリアル出力
    
    /* 自動レンジ切替機能 */
    if((mv <= 3500. && range != 4) || range == 0){  // 電圧値が3500mV以下時
        range = 4;                                  // レンジを4(V)に
        analogMeterInit("mV", "ADS 4V", 0, 4000);   // メータのレンジ設定
    }
    if(mv >= 4000. && mv <= 5500. && range != 6){   // 電圧値3500～5500時
        range = 6;                                  // レンジを6000mVに
        analogMeterInit("mV", "ADS 6V", 0, 6000);   // メータのレンジ設定
    }
    if(mv >= 6000. && mv <= 8500. && range != 9){   // 電圧値が5500～8500時
        range = 9;                                  // レンジを9000mVに
        analogMeterInit("mV", "ADS 9V", 0, 9000);   // メータのレンジ設定
    }
    if(mv >= 9000. && range != 12){                 // 電圧値が6000mV超過時
        range = 12;                                 // レンジを12000mVに
        analogMeterInit("mV", "ADS 12V", 0, 12);    // メータのレンジ設定
    }
    if(range == 12) mv /= 1000.;                // 12Vレンジのときmvを1000で除算
    
    /* 表示 */
    analogMeterNeedle(mv);                      // ADCの電圧値をメータ表示
    delay(100);                                 // 0.1秒（100ms）の待ち時間処理
}
