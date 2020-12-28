/*******************************************************************************
Example 05: ADS1100 Input Voltage Meter for M5Stack [Auto Range Display]
・M5Stack製オプション Voltmeter UNIT (TI ADS1115)を使用
・A/Dコンバータ ADS1115 の読み値をアナログ・メータ表示します【自動レンジ対応版】
・Voltmeter UNIT内のEEPROMに保存されている工場出荷時の校正値の補正機能つき

                                          Copyright (c) 2019-2020 Wataru KUNINO
********************************************************************************
【参考文献】
Arduino IDE 開発環境イントール方法：
https://github.com/m5stack/M5Stack/blob/master/docs/getting_started_ja.md
https://docs.m5stack.com/#/en/related_documents/Arduino_IDE

M5Stack Arduino Library API 情報：
https://docs.m5stack.com/#/ja/api
https://docs.m5stack.com/#/en/arduino/arduino_api

Voltmeter UNIT（M5Stack製）：
https://docs.m5stack.com/#/en/unit/vmeter
*******************************************************************************/

#include <M5Stack.h>                            // M5Stack用ライブラリ
#include <Wire.h>                               // I2C通信用ライブラリ

int range = 0;                                  // 自動レンジ用
float cal = 1.0;                                // 校正値

void setup(){                                   // 起動時に一度だけ実行する関数
    M5.begin();                                 // M5Stack用ライブラリの起動
    Wire.begin();                               // I2Cを初期化
    
    /* 校正値の読み取り用 */
    Wire.beginTransmission(0x53);               // EEPROM(0x53)との通信を開始
    Wire.write(0xF1);                           // 校正データをリクエスト
    if(Wire.endTransmission(false) == 0){       // 送信を終了（接続は継続）
        int16_t target, raw;                    // EEPROMデータ読み取り値代入用
        Wire.requestFrom(0x53, 4);              // 4バイトのデータを要求
        target = (int16_t)(Wire.read() << 8);   // 1バイト目をtargetの上位へ
        target |= (int16_t)Wire.read();         // 2バイト目をtargetの下位へ
        raw = (int16_t)(Wire.read() << 8);      // 3バイト目をrawの上位へ
        raw |= (int16_t)Wire.read();            // 4バイト目をrawの下位へ
        cal = (float)target / (float)raw;       // EEPROMから校正値を読み取り
    }
    Wire.endTransmission();                     // I2C通信の切断
    Serial.printf("target=%d, raw=%d, rate=%f%%\n", target, raw, cal * 100);

    /* ADコンバータ設定用 */
    Wire.beginTransmission(0x49);               // ADS1115(0x49)との通信を開始
    Wire.write(0x01);                           // configコマンド
    Wire.write(0x08);                           // PGA=100b,FSR±0.512V
    Wire.write(0x03);                           // DR=000b,Rate=8SPS
    Wire.endTransmission();                     // ADS1115(0x49)との通信を終了
    M5.Lcd.setBrightness(100);                  // LCDの輝度を100に設定
}

void loop(){                                    // 繰り返し実行する関数
    int16_t adc = 0x0000;                       // 変数adcを定義
    float mv;                                   // 浮動小数点数型変数mvを定義
    Wire.beginTransmission(0x49);               // ADS1115(0x49)との通信を開始
    Wire.write(0x00);                           // readコマンド
    if(Wire.endTransmission(false) == 0){       // 送信を終了（接続は継続）
        Wire.requestFrom(0x49, 2);              // 2バイトのデータを要求
        adc = ((int16_t)Wire.read()) << 8;      // 1バイト目を変数adcの上位へ
        adc |= (int16_t)Wire.read();            // 2バイト目を変数adcの下位へ
    }
    Wire.endTransmission();                     // ADS1115(0x49)との通信を終了
    mv = - 691. / 11. * (float)adc * 512. / 32768. * cal;      // ADC値を電圧に変換してmvへ代入
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
