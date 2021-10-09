#include <main.h>

#INT_RDA
void  rda_isr(void) 
{
    int32 countCycle=0;
    int16 dataCount=0;
    disable_interrupts(INT_RDA);
    do
    {
       if(kbhit(WiFi))
       {
       
         wifiData[dataCount++][0]=getc(WiFi);
         countCycle++;
         
       }
     
    }while(countCycle<65000 && dataCount<2048);
  
    wifiDataCame=1;
    enable_interrupts(INT_RDA);
}



void main()
{


   enable_interrupts(INT_RDA);
   enable_interrupts(INTR_GLOBAL);

   connectESP();  //Configuration function. You have to change this settings according to your settings.
   messageSend="WiFi ESP12S Library...";
   while(TRUE)
   {
      sendDataviaESP(messageSend,0,50);
      if(wifiDataCame)
      {
         wifiDataCame=0;
         //Do what you want after data received via ESP.
      
      }
      delay_ms(1000);
   }

}