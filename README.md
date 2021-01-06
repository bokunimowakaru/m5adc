# m5adc: M5Stack / M5Stick C Examples by Wataru KUNINO
Let's learn IoT ready Analog Sensor device M5Stack and M5Stick C Programing with bokunimo.net.

## ADC 搭載 M5Stack / M5StickC で IoT センサ開発入門
M5Stack 製の IoTデバイス M5Stack または M5StickC と内蔵の A/Dコンバータを使って、 IoT センサ・デバイスを製作してみましょう。  
開発環境には実際の IoT センサの開発で使用されている C/C++ 言語に触れるために、 Arduino IDE を使用します。  
- IoT センサ C/C++ 言語  
- （IoT サーバ Python 言語）  

## 対応 IoT センサ・デバイス  
以下のデバイスを使った IoT センサ を製作します（両方のデバイスのサンプル・プログラムを用意してあるので、実験に必要なのは、どちらか1台です）。  
- M5Stack  
	![M5Stack](https://github.com/bokunimowakaru/m5adc/blob/image/image/DSC_0415.jpg)

- M5StickC  
	![M5StickC](https://github.com/bokunimowakaru/m5adc/blob/image/image/DSC_0418.jpg)

--------------------------------------------------------------------------------

### サンプル集  
M5Stack 用のサンプル・プログラムは m5stack フォルダ、M5Stick C 用のサンプル・プログラムは m5stickc に収録しました。  

- example01_serial  
	Example 01: ADC Input Voltage Monitor  
	A/Dコンバータの読み値をシリアル・モニタへ表示します  

- example02_lcd  
	Example 02: ADC Input Voltage Checker  
	A/Dコンバータの読み値をLCDに数値で表示します  

- example03_meter  
	Example 03: ADC Input Voltage Meter  
	A/Dコンバータの読み値をアナログ・メータ表示します  
	
	![メータ表示](https://github.com/bokunimowakaru/m5adc/blob/image/image/DSC_0393.jpg)  
	M5Stick C でアナログ・メータ表示を行ったときの様子。

- example04_test  
	Example 04: ADC for Loopback Tester  
	A/Dコンバータの読み値の誤差をグラフ表示します  
	（DAC_PINとADC_PINをジャンパ線で接続していてください。）  
	
	<img src="https://github.com/bokunimowakaru/m5adc/blob/image/image/DSC_0389.jpg" height="300">
	<img src="https://github.com/bokunimowakaru/m5adc/blob/image/image/DSC_0440.jpg"><br>  
	左下から右上に向かう斜め曲線の横軸はDAC出力、縦軸はADC値。斜め直線との乖離が誤差。中央の横曲線は誤差。  

- example04_test_val  
	起動時にD/Aコンバータの出力を使って校正値を保持し、補正します  
	（DAC_PINとADC_PINをジャンパ線で接続していてください。）  

- example04_test_sd (M5Stackのみ)  
	A/Dコンバータの非線形歪を補正し、変換精度を高めます  
	（DAC_PINとADC_PINをジャンパ線で接続していてください。）  
	
	![非線形歪補正効果](https://github.com/bokunimowakaru/m5adc/blob/image/image/fig03.jpg)  
	左＝補正無し、中央＝歪補正有、右＝歪補正＋アッテネータ調節

- example05_ads1100
	Example 05: ADS1100 Input Voltage Meter【自動レンジ対応版】  
	M5Stack製オプション ADC HAT (TI ADS1100)を使用  
	A/Dコンバータ ADS1100 の読み値をアナログ・メータ表示します  
	  
	![ADS1100](https://github.com/bokunimowakaru/m5adc/blob/image/image/DSC_0400.jpg)  
	M5Stack製オプション ADC HAT (TI ADS1100)を装着したときの様子。  

- example06_udp  
	Example 06: ADC Input Voltage UDP Sender  
	A/Dコンバータの読み値を Wi-Fi UDP 送信します  

- example07_illum  
	Example 07: Wi-Fi Illumination Sensor  
	照度センサ NJL7502L (新日本無線) の読み値を Wi-Fi UDP送信します  
	
	![NJL7502L](https://github.com/bokunimowakaru/m5adc/blob/image/image/DSC_0432.jpg)  
	M5Stack C に 照度センサ NJL7502L を接続したときの様子。  

- example08_ambient  
	Example 08: Wi-Fi Illumination Sensor for Ambient  
	照度センサ NJL7502L (新日本無線) の読み値を Ambient へ送信します  
	
	![ambient](https://github.com/bokunimowakaru/m5adc/blob/image/image/ambient01.jpg)  
	クラウド・サービス Ambient で照度値を表示したときの様子。  

## IoT センサ Arduino IDE 用 M5Stack 開発環境
以下に、Arduino IDE 用の M5Stack の開発環境をセットアップする方法を示します。詳細については、[セットアップ方法・説明書](https://github.com/bokunimowakaru/m5adc/blob/master/セットアップ方法.pdf)をご覧ください。  

- イントール方法：  
	1. Arduino IDE  
		下記からダウンロードしてインストールしてください  
		- https://www.arduino.cc/en/Main/Software  
	2. ESP32ボードマネージャ  
		Arduino IDEの「ファイル」メニュー内の「環境設定」を開き、「追加のボードマネージャのURL」に下記を追加してください。  
		- https://dl.espressif.com/dl/package_esp32_index.json  
	3. ESP32ライブラリ  
		Arduino IDEの「ツール」メニュー内の「ボード」を選択し、「ボードマネージャ」を開き、検索欄に「esp32」を入力し、下記をインストールしてください。  
		- esp32 by Espressid Systems  
	4. M5Stackライブラリ(M5Core and M5Stick)  
		Arduino IDEの「スケッチ」メニュー内の「ライブラリをインクルード」を選択し、「ライブラリを管理」を開き、検索欄に下記を入力し、インストールしてください。  
		- M5Stack  
		- M5StickC  
	5. Arduino IDEの[ツール］メニュー内の［ボード］で、使用するデバイスを選択してください。  
		- M5Srack-Core-ESP32  
		- M5Stick-C  
	
- M5Stack公式サイトの情報：  
	（最新版・英語）https://docs.m5stack.com/#/en/arduino/arduino_development  

--------------------------------------------------------------------------------

## 対応 IoT サーバ・デバイス  
以下のデバイスを使った IoT サーバ を製作します。  
- Raspberry Pi  
- Windows 上で動作する Cygwin + Python3 (Power Shell + Python 3でも動作確認済み)  

## IoT サーバ Raspberry Pi 用 開発環境

- セットアップ方法  
	pi@raspberry:~ $ git clone https://github.com/bokunimowakaru/m5adc  
	pi@raspberry:~ $ cd m5adc/tools  
	pi@raspberry:~/m5adc/tools $ ./udp_logger.py  
	UDP Logger (usage: ./udp_logger.py port)  
	Listening UDP port 1024 ...  
	2019/11/24 19:37, 192.168.1.3, adcnv_5,1.929  
	2019/11/24 19:37, 192.168.1.3, adcnv_5,0.646  
	2019/11/24 19:37, 192.168.1.3, adcnv_5,0.653  
	2019/11/24 19:38, 192.168.1.3, adcnv_5,0.000  
	2019/11/24 19:38, 192.168.1.3, adcnv_5,1.571  

--------------------------------------------------------------------------------

### MIT License
- Free to use, but keep some conditions: [LICENSE](https://github.com/bokunimowakaru/m5Janken/blob/master/LICENSE)

## Copyright (c) 2020-2021 [Wataru KUNINO @ bokunimo.net](https://bokunimo.net)
