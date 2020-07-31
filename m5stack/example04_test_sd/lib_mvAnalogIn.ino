/*******************************************************************************
ESP32用 ADC mVAnalogIn

float mvAnalogIn(uint8_t PIN)
float mvAnalogIn(uint8_t PIN, float offset)

                                          Copyright (c) 2017-2020 Wataru KUNINO
********************************************************************************
参考文献
https://github.com/bokunimowakaru/esp/blob/master/2_example/example36_le/
*******************************************************************************/

//#define DEBUG_ADC
#define DEBUG_ADC_CAL

#ifndef FLT_MAX
    #define FLT_MAX 3.4028235E+38
#endif
#ifndef INT32_MAX
    #define INT32_MAX 4096*256
#endif

float mvAnalogIn_GAIN = 1.09;
float mvAnalogIn_OFFSET = -0.02;

float get_mvAnalogIn_GAIN(){
	return mvAnalogIn_GAIN;
}

float get_mvAnalogIn_OFFSET(){
	return mvAnalogIn_OFFSET;
}

float mvAnalogIn_correction(int adc, float offset, float gain){
    float ad3;
    
    if( adc > 2599 ){
        ad3 = -1.457583e-7 * (float)adc * (float)adc
            + 1.510116e-3 * (float)adc
            - 0.680858 + offset;
    }else{
        ad3 = 8.378998e-4 * (float)adc + 8.158714e-2 + offset;
    }
    ad3 *= gain;
    if( ad3 < 0.0 ) ad3 = 0.0;
    #ifdef DEBUG_ADC
        Serial.print("ADC (ATT=3;11dB) = ");
        Serial.print(ad3,3);
        Serial.print(" [V], ");
        Serial.println(adc);
    #endif
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
        #ifdef DEBUG_ADC
            Serial.print("ADC (ATT=0; 0dB) = ");
            Serial.print(ad0,3);
            Serial.print(" [V], "); 
            Serial.println(in0);
        #endif
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

float mvAnalogInCalibration(uint8_t PIN, uint8_t POUT){
    int err, err_min = INT32_MAX;
    #ifdef m5
        M5.Lcd.printf("mvAnalogInCalibration GPIO%d -> %d\n", PIN, POUT);
    #endif
    dacWrite(POUT, 0);
    delay(100);
    err = adc_adapted_att(PIN);
    dacWrite(POUT, 255);
    delay(100);
    err += abs(adc_adapted_att(PIN) - 4095);
    if( err > 200){
        Serial.println("Cal. ERROR");
        #ifdef m5
            M5.Lcd.println("Cal. ERROR");
            delay(3000);
        #endif
        return -999;
    }
    for(float gain = .81; gain < 1.20; gain += 0.01){
        for(float offset = -0.10; offset < 0.10; offset += 0.005){
            err = 0;
            for(int dac = 0; dac < 256; dac++){
                dacWrite(POUT, dac);
                int adc = (int)(mvAnalogIn(PIN, offset, gain) / 3300. * 4095. + 0.5);
                err += abs(dac * 16 - adc);
                #ifdef DEBUG_ADC_CAL
                //  if(dac % 32 == 0) Serial.printf("{'d':%d}, {'a':%d}, {'e':%d}\n",dac * 16, adc, err);
                #endif
            }
            if(err < err_min){
                mvAnalogIn_GAIN = gain;
                mvAnalogIn_OFFSET = offset;
                err_min = err;
                #ifdef DEBUG_ADC_CAL
                    Serial.printf("Cal. gain=%.3f, offset=%.3f, err=%.1f\n",gain, offset, (float)err/256.);
                #endif
                #ifdef m5
                    M5.Lcd.printf("%.0f ",(float)err/256.);
                #endif
            }
        }
    }
    #ifdef DEBUG_ADC_CAL
        Serial.printf("     gain  = %.3f\n", mvAnalogIn_GAIN);
        Serial.printf("     offset= %.3f\n", mvAnalogIn_OFFSET);
        Serial.printf("     error = %.3f\n", (float)err_min / 256.);
    #endif
    #ifdef m5
        M5.Lcd.printf("\ngain = %.3f", mvAnalogIn_GAIN);
        M5.Lcd.printf(", offset = %.3f", mvAnalogIn_OFFSET);
        M5.Lcd.printf(", error = %.3f\n", (float)err_min / 256.);
        delay(3000);
    #endif
    return err_min;
}

////////////////////////////////////////////////////////////////////////

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
    float ad = mvAnalogIn(PIN,offset, gain) / 3300. * 4095.;
    int adc = int( ad + 0.5 );
    if( adc < 0 ) adc = 0;
    if( adc > 4095 ) adc = 4095;
    return adc;
}

float adc_adapted_att(uint8_t PIN, float offset){
    return adc_adapted_att(PIN, offset, mvAnalogIn_GAIN);
}

float adc_adapted_att(uint8_t PIN){
    return adc_adapted_att(PIN, mvAnalogIn_OFFSET, mvAnalogIn_GAIN);
}

