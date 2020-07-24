# m5adc: M5Stack / M5Stick C Examples by Wataru KUNINO
Let's learn IoT ready Analog Sensor device M5Stack and M5Stick C Programing with bokunimo.net.

## ADC ���� M5Stack / M5StickC �� IoT �Z���T�J������
M5Stack ���� IoT�f�o�C�X M5Stack �܂��� M5StickC �Ɠ����� A/D�R���o�[�^���g���āA IoT �Z���T�E�f�o�C�X�𐻍삵�Ă݂܂��傤�B  
�J�����ɂ͎��ۂ� IoT �Z���T�̊J���Ŏg�p����Ă��� C/C++ ����ɐG��邽�߂ɁA Arduino IDE ���g�p���܂��B
	- IoT �Z���T C/C++ ����
	- �iIoT �T�[�o Python ����j

## �Ή� IoT �Z���T�E�f�o�C�X  
�ȉ��̃f�o�C�X���g���� IoT �Z���T �𐻍삵�܂��B  
	- M5Stack
	- M5StickC

## �Ή� IoT �T�[�o�E�f�o�C�X  
�ȉ��̃f�o�C�X���g���� IoT �T�[�o �𐻍삵�܂��B  
	- Raspberry Pi
	- Windows ��œ��삷�� Cygwin + Python3 (Power Shell + Python 3�ł�����m�F�ς�)

## IoT �Z���T Arduino IDE �p M5Stack �J����
Arduino IDE�p��M5Stack�̊J�������Z�b�g�A�b�v������@�́A���L��M5Stack�����T�C�g���������������B  

- �C���g�[�����@�F  
	�i�ŐV�ŁE�p��jhttps://docs.m5stack.com/#/en/arduino/arduino_development  
	1. Arduino IDE  
		���L����_�E�����[�h���ăC���X�g�[�����Ă�������  
		- https://www.arduino.cc/en/Main/Software  
	2. ESP32�{�[�h�}�l�[�W��  
		Arduino IDE�́u�t�@�C���v���j���[���́u���ݒ�v���J���A�u�ǉ��̃{�[�h�}�l�[�W����URL�v�ɉ��L��ǉ����Ă��������B
		- https://dl.espressif.com/dl/package_esp32_index.json
	3. ESP32���C�u����  
		Arduino IDE�́u�c�[���v���j���[���́u�{�[�h�v��I�����A�u�{�[�h�}�l�[�W���v���J���A�������Ɂuesp32�v����͂��A���L���C���X�g�[�����Ă��������B
		- esp32 by Espressid Systems
	4. M5Stack���C�u����(M5Core and M5Stick)  
		Arduino IDE�́u�X�P�b�`�v���j���[���́u���C�u�������C���N���[�h�v��I�����A�u���C�u�������Ǘ��v���J���A�������ɉ��L����͂��A�C���X�g�[�����Ă��������B
		- M5Stack
		- M5StickC
	5. Arduino IDE��[�c�[���n���j���[���́m�{�[�h�n�ŁA�g�p����f�o�C�X��I�����Ă��������B
		- M5Srack-Core-ESP32
		- M5Stick-C

## IoT �T�[�o Raspberry Pi �p �J����

- �Z�b�g�A�b�v���@
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

### MIT License
- Free to use, but keep some conditions: [LICENSE](https://github.com/bokunimowakaru/m5Janken/blob/master/LICENSE)

## Copyright (c) 2020 [Wataru KUNINO @ bokunimo.net](https://bokunimo.net)
