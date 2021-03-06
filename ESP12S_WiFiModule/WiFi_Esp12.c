#include<stdarg.h>
int8 buffercounter=0;
int32 lenghtofMem=0;
int32 lenghtofMemImage=0;

char transienBuffer[50];
int1 waitAnswer()
{
   
   timeOutWiFi=0;
   while(timeOutWiFi<2)
   {
      if(wifiDataCame)
      {
      
        for(int i=0;i<100;i++)
        {
            if(wifiData[i][0]=='O' && wifiData[i+1][0]=='K')
             {     
                
                for(int i=0;i<100;i++)
                   wifiData[i][0]='\0';
                wifiDataCame=0;
                return 1;
             }
        }
      
      }
   }
   wifiDataCame=0;
   
   return 0;
}

void writeATCommand(char *command)
{

     fprintf(WiFi,"%s",command);
     fprintf(WiFi,"%s","\r\n");

}

int1 resetESP()
{       
       sprintf(transienBuffer,"AT+RST");
       writeATCommand(transienBuffer);
       delay_ms(1500);
       return waitAnswer();
       
}

int1 connectModule()
{
   transienBuffer="AT";
   writeATCommand(transienBuffer);
  
   return waitAnswer();
   
   
}


int1 selectMode(int selection)
{
   
   sprintf(transienBuffer,"AT+CWMODE=%d",selection);
   writeATCommand(transienBuffer);
   return waitAnswer();
       
}
int1 selectUsage(int selection)
{
      sprintf(transienBuffer,"AT+CIPMODE=%d",selection);
      writeATCommand(transienBuffer);
        
      return waitAnswer();

}

int1 selectMultipleOrSingle(int selection)
{
      sprintf(transienBuffer,"AT+CIPMUX=%d",selection);
      writeATCommand(transienBuffer);
      
      return waitAnswer();
}

int1 connectWifi(char *username, char *password)
{   
   char connectionBuffer[80];   

   sprintf(connectionBuffer,"AT+CWJAP=\"%s\",\"%s\"\r\n",username,password);  //To connect Wi-Fi, write Wi-Fi name and password.
   fprintf(WiFi,"%s",connectionBuffer);

   return waitAnswer();

}


int1 accessPoint()
{
   fprintf(WIFI,"%s","AT+CWSAP=\"");
   fprintf(WIFI,"%s","TALHA");       //Access Point name. Can changable.
   fprintf(WIFI,"%s","\",\"");   
   fprintf(WIFI,"%s","12345678");    //Passrowd for access point
   fprintf(WIFI,"%s","\",6,4\r\n");
    
   return waitAnswer();
      
}

int1 enableDHCP(int select1,int select2)
{
      sprintf(transienBuffer,"AT+CWDHCP_CUR=%d,%d",select1,select2);
      writeATCommand(transienBuffer);
     
      return waitAnswer();

}

int1 setSTA()
{

     transienBuffer="AT+CIPSTA=\"192.168.0.80\""; // Write IP Address whatever you want
     writeATCommand(transienBuffer);
     return waitAnswer();

}
int1 setTimeout(int timeout)
{

      sprintf(transienBuffer,"AT+CIPSTO=%d",timeout);
      writeATCommand(transienBuffer);
      
      return waitAnswer();
      
}

int1 queryTimeout()
{

      sprintf(transienBuffer,"AT+CIPSTO?");
      writeATCommand(transienBuffer);
       
      return waitAnswer();
      
}


int1 serverMode(int selection,int port)
{
       
      if(!selectMode(3)) return 0;  //1->Station 2->Access Point 3-> Both
      
      
      if(!selectMultipleOrSingle(1)) return 0;  //0->Single 1->Multiple
      
      sprintf(transienBuffer,"AT+CIPSERVER=%d,%d",1,1234);
      writeATCommand(transienBuffer);
      if(!waitAnswer()) return 0;
     
      
      if(!setTimeout(180)) return 0;
         
      return 1;

}


int1 sendDataviaESP(int8 *message,int ID,int length)
{       
      sprintf(transienBuffer,"AT+CIPSEND=%d,%d",ID,length);
      writeATCommand(transienBuffer);
      delay_ms(1);
      if(length==2048)
      {
         for(int i=0;i<4;i++)
         {
           for(int j=0;j<512;j++)
           {
              fprintf(WIFI,"%c",message[512*i+j]);
           
           }
           delay_ms(2);
   
         }
      }
      
      else
      {
           for(int j=0;j<length;j++)
           {
              fprintf(WIFI,"%c",message[j]);
           
           }     
      
      }
              
      return 1;
}


void nullSend()
{
      sprintf(transienBuffer,"AT+CIPSEND=%d,%d",0,64);
      writeATCommand(transienBuffer);
      delay_ms(1);
      
      for(int i=0;i<64;i++)
      {
         fprintf(WiFi,"%c",131);
      }

}


int1 connectESP()
{

   
   connectModule();
   if(!waitAnswer()) return 0;
   
   wifiID="Talha";
   wifiPassword="123456";
   connectWifi(wifiID,wifiPassword);
              
   if(!selectMode(3)) return 0;   //1->Station 2->Access Point 3-> Both
     
   if(!selectMultipleOrSingle(1)) return 0;      //0->Single 1->Multiple
   
   if(!accessPoint()) return 0;
   
   if(!setSTA()) return 0;
   
   queryTimeout();

   if(!serverMode(1,1234)) return 0;

   return 1;
}

void listAll()
{
      transienBuffer="AT+CWLAP";
      writeATCommand(transienBuffer);

}


int1 closeWiFiConnection()
{
    transienBuffer="AT+CWQAP";
    writeATCommand(transienBuffer);
    if(!waitAnswer()) return False;
    return True;
    
}
