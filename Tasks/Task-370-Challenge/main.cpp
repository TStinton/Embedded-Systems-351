#include "uop_msb.h"
#include <chrono>
using namespace uop_msb;

//Used this to 
Ticker tick;
Thread t1;
osThreadId_t ReadADCThreadID;
BufferedSerial serial(USBTX, USBRX, 115200);

AnalogIn LDR(AN_LDR_PIN);
DigitalOut GreenLed(TRAF_GRN1_PIN);
unsigned short lightVal;
int avg;
int coun;

void ReadADC(){
    while(true){
    ReadADCThreadID = ThisThread::get_id();
    ThisThread::flags_wait_all(1);
    //GreenLed = !GreenLed;
    coun = coun+1;
    lightVal = LDR.read_u16();
    lightVal += lightVal;
    
    }
}
void Average();

void ISR(){
    osSignalSet(ReadADCThreadID,1);

    //osSignalClear(ReadADCThreadID,1);
}

int main(void)
{
    tick.attach(&ISR,1ms);
    t1.start(ReadADC);
    //t1.join();

/*
1. Create an ISR - called by a Ticker every 1ms. This ISR simply has the job of signalling a waiting thread to perform an ADC conversion
2. Create a thread that waits for a signal from the ISR. This thread should ideally have the highest priority
3. Each time it is unblocked by the signal, it should read the ADC (Use `AnalogIn`) for the LDR and add it to a running sum.
4. Every 1s, it should print out the average of the past 1000 samples to the terminal. 
*/
    while (true) {
        ThisThread::sleep_for(1000ms);
        avg = lightVal / coun;
        lightVal = 0;
        coun = 0;
        printf("\nLDR = %d", avg);
    }
    
}   