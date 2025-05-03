#include "simuc.h"
#include "io_treiber.h"

#include <stdio.h>

typedef struct BHandle_Data {
	BYTE Board_allocated;	// 1=allocated, 0=free
	BYTE Port_A_Direction;	// 1=Output, 0=Input
	BYTE Port_B_Direction;
	BYTE Port_C_Direction;
	BYTE Port_D_Direction;
} BHandle;

typedef BHandle* DSCB;

DSCB GlobalBoardHandle = NULL;  // Dies ist ein Zeiger (siehe typedef BHandle* DSCB in io_treinber.h),
                                // der noch auf einen gueltigen Speicherbereich "gebogen" werden muss.
                                // Dieser Speicherbereich muss dann in Init() mittels calloc() allokiert
                                // werden.



BYTE Init(DSCB* Zeiger_auf_BoardHandle, unsigned long int Steuerwort)
{
    DSCB Lokales_BoardHandle;

    if (*Zeiger_auf_BoardHandle == NULL) {
        // Allokieren des Speicherbereichs und "Biegen" des lokalen BoardHandles darauf
        Lokales_BoardHandle = (DSCB)calloc(1, sizeof(BHandle));
        if (Lokales_BoardHandle == NULL) {
            // Fehler beim Allokieren des Speicherbereichs
            return 1;
        }
        *Zeiger_auf_BoardHandle = Lokales_BoardHandle; // "Biegen" des übergebenen Zeigers
    } else {
        Lokales_BoardHandle = *Zeiger_auf_BoardHandle;
    }

    // Ab hier nur noch mit dem lokalen BoardHandle arbeiten

    // Verwaltungsdaten entsprechend dem Steuerwort setzen
    switch (Steuerwort) {
        case 0x92:
            Lokales_BoardHandle->Board_allocated = 1;
            Lokales_BoardHandle->Port_A_Direction = 0;
            Lokales_BoardHandle->Port_D_Direction = 1;
            Lokales_BoardHandle->Port_C_Direction = 1;
            Lokales_BoardHandle->Port_B_Direction = 0;
            break;
        case 0x93:
            Lokales_BoardHandle->Board_allocated = 1;
            Lokales_BoardHandle->Port_A_Direction = 0;
            Lokales_BoardHandle->Port_D_Direction = 0;
            Lokales_BoardHandle->Port_C_Direction = 1;
            Lokales_BoardHandle->Port_B_Direction = 0;
            break;
        case 0x9A:
            Lokales_BoardHandle->Board_allocated = 1;
            Lokales_BoardHandle->Port_A_Direction = 0;
            Lokales_BoardHandle->Port_D_Direction = 1;
            Lokales_BoardHandle->Port_C_Direction = 0;
            Lokales_BoardHandle->Port_B_Direction = 0;
            break;
        case 0x9B:
            Lokales_BoardHandle->Board_allocated = 1;
            Lokales_BoardHandle->Port_A_Direction = 0;
            Lokales_BoardHandle->Port_D_Direction = 0;
            Lokales_BoardHandle->Port_C_Direction = 0;
            Lokales_BoardHandle->Port_B_Direction = 0;
            break;
        default:
            // Ungültiges Steuerwort
            return 2;
    }

    return 0;
}

BYTE InputByte(DSCB BoardHandle, BYTE Port, BYTE *DigitalValue) {
    
	// Mit Leben fuellen

    if(BoardHandle->Board_allocated==0)
        return 1;
     switch(Port) {
        case 3 :
            if(BoardHandle->Port_D_Direction==0){
                *DigitalValue=io_in16(IN1)>>8;
                return 0;
            }
            return 3;
            break;

        case 1:

            if(BoardHandle->Port_B_Direction==0){
                *DigitalValue=io_in16(IN0)>>8;
            return 0;
            }
            else
                return 3;
            break;

        case 0   :
            if(BoardHandle->Port_A_Direction==0){
                *DigitalValue=io_in16(IN0)&0x00FF;
            return 0;
            }
            else
                return 3;
            break;
        case  2 :
            if(BoardHandle->Port_C_Direction==0){
                *DigitalValue=io_in16(IN1)&0x00FF;
            return 0;
            }
            else
                return 3;

            default :
                return 2;
        }
    }

BYTE OutputByte(DSCB BoardHandle, BYTE Port, BYTE DigitalValue) {
    
	// Mit Leben fuellen
    unsigned short out1;
       unsigned short out2;
       if(BoardHandle->Board_allocated==0)
            return 1;
       switch(Port){
           case 3:
               if(BoardHandle->Port_D_Direction==1){
                   out1=(io_in16(IN1));
                   out1=out1& 0x00FF;//die Bits des Ports C behalten und der Port D auf 0 setzen
                   out2=(DigitalValue<<8); // binary um 8 Stellen nach Links verschieben und mit den Output verodern ergibt in port C die binäre Wert der Digitalvalue und Port D bleibt unverändert
                   io_out16(OUT1,out2|out1);
                   return 0;
               }
               else {
                   return 3;
               }
           case 1:
               if(BoardHandle->Port_B_Direction==1){
                   out1=(io_in16(OUT0)|0x00FF);//die Bits des Ports A behalten und der Port B auf 0 setzen
                   out2=((DigitalValue<<8)|out1);// binary um 8 Stellen nach Links verschieben und mit den Output verodern ergibt in port A die binäre Wert der Digitalvalue und Port Bbleibt unverändert
                   io_out16(OUT0,out2);
                   return 0;
               }
               else {
                   return 3;
               }
           case 2:
               if(BoardHandle->Port_C_Direction==1){
                   out1=(io_in16(OUT1)&0xFF00); //die Bits des Ports D behalten und der Port C auf 0 setzen
                   out2=(DigitalValue | out1 );      // binary mit den Output verodern ergibt in port C die binäre Wert der Digitalvalue und Port D bleibt unverändert
                   io_out16(OUT1,out2);
                   return 0;
               }
               else{
                   return 3;
               }
           case 0:
               if(BoardHandle->Port_A_Direction==1){
                   out1=(io_in16(OUT0)& 0xFF00); //die Bits des Ports B behalten und der Port A auf 0 setzen
                   out2=(DigitalValue | out1 );       // binary mit den Output verodern ergibt in port A die binäre Wert der Digitalvalue und Port B bleibt unverändert
                   io_out16(OUT0,out2);
                   return 0;
               }
               else{
                   return 3;
               }

               default :
                   return 2;

       }
   }


BYTE Free(DSCB BoardHandle) {
    
	// Mit Leben fuellen

    if(BoardHandle->Board_allocated==1){
        BoardHandle->Board_allocated=0;
        return 0;}
    else
        return 1;
}
