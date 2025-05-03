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



DSCB GlobalBoardHandle = NULL;  // Dies ist ein Zeiger (siehe typedef BHandle* DSCB in io_treinber.h),
                                // der noch auf einen gueltigen Speicherbereich "gebogen" werden muss.
                                // Dieser Speicherbereich muss dann in Init() mittels calloc() allokiert
                                // werden.



BYTE Init(DSCB* Zeiger_auf_BoardHandle, unsigned long int Steuerwort) {

    DSCB Lokales_BoardHandle;   // Es wird empfohlen innerhalb dieser Funktion mit diesem
                                // lokalen BoardHandle zu arbeiten. Ein Zugriff auf die
                                // Stukturkomponenten ist dann ganz normal, z.B. mittels
                                // Lokales_BoardHandle->Borad_allocated, moeglich.


    if (*Zeiger_auf_BoardHandle == NULL){	
        // Allokieren des Speichersbereiches und "biegen"  des lokalen Boadhandle auf diesen
        // Speicherbereich.
        Lokales_BoardHandle = (DSCB) calloc(1, sizeof(BHandle));

        // Auch den per Call-By-Reference uebergebenen Zeiger auf den reservierten Speicherbereich "biegen"
        *Zeiger_auf_BoardHandle = Lokales_BoardHandle;
    } else {	
        Lokales_BoardHandle = *Zeiger_auf_BoardHandle; // Damit bei Mehrfachaufrufen von Init() das "Lokales_BoardHande" richtig initialisiert wird
    }

    // Ab hier nur noch mit dem lokalen BoardHande arbeiten
    
	// Mit Leben fuellen
	
	return 0;
}

BYTE InputByte(DSCB BoardHandle, BYTE Port, BYTE *DigitalValue) {
    
	// Mit Leben fuellen

	return 0;
}

BYTE OutputByte(DSCB BoardHandle, BYTE Port, BYTE DigitalValue) {
    
	// Mit Leben fuellen

	return 0;
}

BYTE Free(DSCB BoardHandle) {
    
	// Mit Leben fuellen
	
	return 0;
}

