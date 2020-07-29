/*******************************************************************************
ESP32用 ADC mVAnalogIn

float mvAnalogIn(uint8_t PIN)
float mvAnalogIn(uint8_t PIN, float offset)

                                          Copyright (c) 2017-2020 Wataru KUNINO
********************************************************************************
参考文献
https://github.com/bokunimowakaru/esp/blob/master/2_example/example36_le/
*******************************************************************************/

#define mvAnalogIn_GAIN 1.09
#define mvAnalogIn_OFFSET -0.02           // 動作最小電圧 0.0 ～ 0.1(V)程度

float mvAnalogIn_correction(int adc, float offset, float gain){
    float ad3;
    
    if( adc > 2599 ){
        ad3 = -1.457583e-7 * (float)adc * (float)adc
            + 1.510116e-3 * (float)adc
            - 0.680858 + offset;
    }else{
        ad3 = 8.378998e-4 * (float)adc + 8.158714e-2 + offset;
    }
    ad3 *= mvAnalogIn_GAIN;
    if( ad3 < 0.0 ) ad3 = 0.0;
    if( ad3 > 3.3 ) ad3 = 3.3;
    Serial.print("ADC (ATT=3;11dB) = ");
    Serial.print(ad3,3);
    Serial.print(" [V], ");
    Serial.println(adc);
    return ad3 * 1000.;
}

float mvAnalogIn(uint8_t PIN, float offset, float gain){
    int in0,in3;
    float ad0,ad3;
    
    analogSetPinAttenuation(PIN,ADC_11db);
    in3=analogRead(PIN);
    ad3 = mvAnalogIn_correction(in3, offset, gain) / 1000.; 
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

float mvAnalogIn(uint8_t PIN, float offset){
    return mvAnalogIn(PIN, offset, mvAnalogIn_GAIN); 
}

float mvAnalogIn(uint8_t PIN){
    return mvAnalogIn(PIN, mvAnalogIn_OFFSET, mvAnalogIn_GAIN); 
}

float ad_correction(int adc, float offset, float gain){
    return mvAnalogIn_correction(adc, offset, gain) / 3300. * 4095.;
}

float ad_correction(int adc, float offset){
    return mvAnalogIn_correction(adc, offset, mvAnalogIn_GAIN) / 3300. * 4095.;
}

float ad_correction(int adc){
    return mvAnalogIn_correction(adc, mvAnalogIn_OFFSET, mvAnalogIn_GAIN) / 3300. * 4095.;
}

float adc_adapted_att(uint8_t PIN, float offset, float gain){
    return mvAnalogIn(PIN,offset, gain) / 3300. * 4095.;
}

float adc_adapted_att(uint8_t PIN, float offset){
    return mvAnalogIn(PIN, offset, mvAnalogIn_GAIN) / 3300. * 4095.;
}

float adc_adapted_att(uint8_t PIN){
    return mvAnalogIn(PIN, mvAnalogIn_OFFSET, mvAnalogIn_GAIN) / 3300. * 4095.;
}
