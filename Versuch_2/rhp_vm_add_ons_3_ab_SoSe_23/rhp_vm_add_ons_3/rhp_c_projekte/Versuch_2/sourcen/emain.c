#include "simuc.h"
#include "io_treiber.h"


// Die Verwendung der folgenden Zustandsnamen ist verbindlich
typedef enum {hoch, runter, steht} STATE;

// Die folgenden Defines muessen sinnvoll genutzt werden
// Einige Defines zum leichteren Wiederfinden
#define BIT_POS_IST_OBEN	0
#define BIT_POS_IST_UNTEN	1
#define BIT_POS_NACH_OBEN	4
#define BIT_POS_NACH_UNTEN	5

#define BIT_POS_MOTOR_AN	0
#define BIT_POS_FAHRE_NACH_OBEN	1
#define BIT_POS_FAHRE_NACH_UNTEN	2


// Hier die Steuerungsfunktion definieren
// ...

void emain(void* arg) 
{
	// Hier alle benoetigten Variablen deklarieren
	// ...


	INIT_BM_WITH_REGISTER_UI; // Hier unbedingt einen Break-Point setzen !!!

	
	// Hier die Treiberfunktionen aufrufen und testen (Aufgabe 1)
	// ...





	// Ab hier beginnt die Endlosschleife fuer den Automaten (Aufgabe 2)
	while(1) {

		SYNC_SIM; 

		// Hier die Eingabesignale einlesen
		// ...

		

		// Hier die Steuerungsfunktion aufrufen
		// ...




		// Hier die Ausgabesignale ausgeben
		// ...


	} // while(1)..

	
}

  