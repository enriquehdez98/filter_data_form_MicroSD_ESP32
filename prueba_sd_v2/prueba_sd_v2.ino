#include "SD.h"   //Librería de SD
File myFile;
File myFileF;
int flagColumna  = 0;
String cadena;
float window_x[8]={0,0,0,0,0,0,0,0};
float cop_x[8]={0,0,0,0,0,0,0,0};
float window_y[8]={0,0,0,0,0,0,0,0};
float cop_y[8]={0,0,0,0,0,0,0,0};
int cont_8=0;
int flag_filtro=0;
float  a[8]={1, -3.06190312916490, 4.61551695155407,  -4.15585580305572, 2.37815653704039,  -0.853179924410146,  0.176453731249952, -0.0161309543925350};
float b[8]={0.000648886006415002,  0.00454220204490501, 0.0136266061347150,  0.0227110102245251,  0.0227110102245251,  0.0136266061347150,  0.00454220204490501, 0.000648886006415002};
float cop[2]={0,0};

void setup(){
  Serial.begin(115200);  
  Serial.print("Iniciando SD ...");
  if (!SD.begin(5)) {
    Serial.println("No se pudo inicializar");
    return;
  }
  Serial.println("inicializacion exitosa");
  myFile = SD.open("/prueba.txt");//abrimos  el archivo
  myFileF = SD.open ("/pruebaF.txt",FILE_WRITE);
}

void loop(){
  while(myFile.available()){ //Permite recorrer todos los caracteres del archivo
   char caracter = myFile.read(); //Lee caracter por caracter
        if(flagColumna==5){ // Si se detecta dato en la columna 5 
          if (caracter!=44){
            cadena=cadena+caracter; // Si es un signo - + ó un numero se almacena en una cadena
            }else{   
            cop[0]=cadena.toFloat(); // Se convierte la cadena de texto en flotante
            cadena.clear();
            if (cont_8<7){
              cont_8=cont_8+1;
              window_x[cont_8-1]=cop[0];
              }else{
                window_x[cont_8]=cop[0];
                for(int i=0;i<=7;i++){
                  cop_x[cont_8]=(cop_x[cont_8]+(b[i]*window_x[7-i])-(a[i]*cop_x[7-i]));
                  }
                 Serial.print(cop[0]); 
                 Serial.print("  ");
                 Serial.print(cop_x[cont_8]);
                 Serial.print("  ");
                 myFileF.print(cop_x[cont_8]);
                 myFileF.print(",");
                for (int i=0;i<=6;i++){
                  cop_x[i]=cop_x[i+1];
                  window_x[i]=window_x[i+1];
                  }
                 cont_8=7;  
                }
              }
          }

         if (flagColumna==6){
          if (caracter!=44){ 
           cadena=cadena+caracter;
            if (myFile.position()==myFile.size()){
                cop[1]=cadena.toFloat();
                cadena.clear();
                /////////////////////////
                for(int i=0;i<=7;i++){
                  cop_y[cont_8]=(cop_y[cont_8]+(b[i]*window_y[7-i])- (a[i]*cop_y[7-i]));
                  }
                 myFileF.print(cop_y[cont_8]); 
                //////////////////////////////

              }
          }else{
            cop[1]=cadena.toFloat();
            cadena.clear();
            
           ////////////////////////
              if (cont_8<7){
              cont_8=cont_8+1;  
              window_y[cont_8-1]=cop[1];
              }else{
                window_y[cont_8]=cop[1];
                for(int i=0;i<=7;i++){
                  cop_y[cont_8]=(cop_y[cont_8]+(b[i]*window_y[7-i])- (a[i]*cop_y[7-i]));
                  }
                 Serial.print(cop[1]);
                 Serial.print(" ");  
                 Serial.println(cop_y[cont_8]);
                 myFileF.println(cop_y[cont_8]);
                for (int i=0;i<=6;i++){
                  cop_y[i]=cop_y[i+1];
                  window_y[i]=window_y[i+1];
                  }  
                }

           ///////////////////////////



            
            }  
        }

        if (flagColumna>6) { 
          flagColumna=1;
        }

   if (caracter==44){
      flagColumna=flagColumna+1;
   }     
  }
   myFileF.close(); 
   myFile.close();

}
