/*******************************************************************************
ESP32用 ADC mVAnalogIn

float mvAnalogIn(uint8_t PIN)
float mvAnalogIn(uint8_t PIN, float offset)

                                          Copyright (c) 2017-2020 Wataru KUNINO
********************************************************************************
参考文献
https://github.com/bokunimowakaru/esp/blob/master/2_example/example36_le/
*******************************************************************************/

float mvAnalogIn_correction(int adc, float offset = 0.0){
    float ad3;
    
    if( adc > 2599 ){
        ad3 = -1.457583e-7 * (float)adc * (float)adc
            + 1.510116e-3 * (float)adc
            - 0.680858 + offset;
    }else{
        ad3 = 8.378998e-4 * (float)adc + 8.158714e-2 + offset;
    }
    if( ad3 < 0.0 ) ad3 = 0.0;
    if( ad3 > 3.3 ) ad3 = 3.3;
    Serial.print("ADC (ATT=3;11dB) = ");
    Serial.print(ad3,3);
    Serial.print(" [V], ");
    Serial.println(adc);
    return ad3 * 1000.;
}

float mvAnalogIn(uint8_t PIN){
    return mvAnalogIn(PIN, 0.0);            // 動作最小電圧 0.0 ～ 0.1(V)程度
//  return mvAnalogIn(PIN, 1.075584e-1);
}

float mvAnalogIn(uint8_t PIN, float offset){
    int in0,in3;
    float ad0,ad3;
    
    analogSetPinAttenuation(PIN,ADC_11db);
    in3=analogRead(PIN);
    ad3 = mvAnalogIn_correction(in3, offset) / 1000.; 
    if( in3 < 200 ){
        analogSetPinAttenuation(PIN,ADC_0db);
        in0=analogRead(PIN);
        ad0 = 2.442116e-4 * (float)in0 + offset;
        Serial.print("ADC (ATT=0; 0dB) = ");
        Serial.print(ad0,3);
        Serial.print(" [V], "); 
        Serial.println(in0);
        if( in3 >= 100 ){
            ad3 = ad3 * ((float)in3 - 100.) / 100.
                + ad0 * (200. - (float)in3) / 100.;
        }else{
            ad3 = ad0;
        }
    }
    analogSetPinAttenuation(PIN,ADC_11db);
    if( ad3 < 0.0 ) ad3 = 0.0;
    if( ad3 > 3.3 ) ad3 = 3.3;
    return ad3 * 1000.;
}

float ad_correction(int adc, float offset){
    return mvAnalogIn_correction(adc,offset) / 3300. * 4095.;
}

float ad_correction(int adc){
    return ad_correction(adc, 0.0);
}

float adc_adapted_att(uint8_t PIN, float offset){
    return mvAnalogIn(PIN, offset) / 3300. * 4095.;
}

float adc_adapted_att(uint8_t PIN){
    return adc_adapted_att(PIN, 0.0);
}
