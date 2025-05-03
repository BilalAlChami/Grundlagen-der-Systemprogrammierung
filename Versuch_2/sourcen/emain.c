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

        static long int startpos;
        static long int endpos;
        static long int aktpos;

        void stgfunc (unsigned short int HPL, unsigned short int HPR, unsigned short int T1, unsigned short int T2,
        unsigned short int* M_Re,unsigned short int *M_Li,unsigned short int* M_An, STATE* pstate,long int aktpos ) {
        switch (*pstate) {

        case steht:
        *M_Li = 0;
        *M_An=0;
        *M_Re=0;
        if(T1==1)
        {
        *pstate=hoch;
        }
        else if(T2==1){
        *pstate=runter;
        }

        break;
        case hoch:
        *M_Li=1;*M_Re=0;*M_An=1;
        startpos=aktpos;
        endpos = (startpos - 60000) % 65535;
        if(endpos<0)
        {
        endpos=endpos+65535;
        }
        if(T1==1)
        {
        *pstate=hoch;
        }


        break;
        case runter:
        *M_Li=0;*M_Re=1;*M_An=1;
        startpos=aktpos;
        endpos = (startpos + 60000) % 65535;
        if(T1==1){
        *pstate=runter;
        }

        break;

        default: *pstate=steht;
        }
}

        void emain(void* arg)
        {
        // 2) Definition des Startzustandes
        STATE pstate;
        // Hier alle benoetigten Variablen deklarieren
        // ...
        unsigned short int HPL; unsigned short int HPR; unsigned short int T1; unsigned short int T2;

        //Variable für die Ausgabesignale (Aufgabe 2)
        unsigned short int M_Re;unsigned short int M_Li;unsigned short int M_An;
        BYTE output;
        BYTE test1,test2,test3,test4,test5,test6,x;
        test5=15;

        INIT_BM_WITH_REGISTER_UI; // Hier unbedingt einen Break-Point setzen !!!


        // Hier die Treiberfunktionen aufrufen und testen (Aufgabe 1)
        // ...
        test1=Init(&GlobalBoardHandle, 0x9A);
        test2=InputByte(GlobalBoardHandle,2,&test4);
        test3=OutputByte(GlobalBoardHandle,3,test5);
        test6=Free(GlobalBoardHandle);


        pstate=steht;
        Init(&GlobalBoardHandle,0x9A );// Port D als Ausgang und Port A,B,C als Eingänge

        // Ab hier beginnt die Endlosschleife fuer den Automaten (Aufgabe 2)
        while(1) {

        //    Hier alle benoetigten Variablen deklarieren
        // Variable fuer Aufgabe 1


        SYNC_SIM;


        // Hier die Treiberfunktionen aufrufen und testen (Aufgabe 1)
        // ...
        //test1=Init(GlobalBoardHandle, 0x80);
        // test2=InputByte(GlobalBoardHandle,1,33);
        //) test3=OutputByte(GlobalBoardHandle,1, 22);




        // 1) Hardware konfigurieren
        //Free(GlobalBoardHandle);





        //3)Unendliche Schleife
        // Ab hier beginnt die Endlosschleife fuer den Automaten (Aufgabe 2)


        InputByte(GlobalBoardHandle,PC,&x);
        // Hier die Eingabesignale einlesen


        HPL=(x>>BIT_POS_IST_OBEN)&0x01;
        HPR=(x>>BIT_POS_IST_UNTEN)&0x01;
        T1=(x>>BIT_POS_NACH_OBEN)&0x01;
        T2=(x>>BIT_POS_NACH_UNTEN)&0x01;

        InputByte(GlobalBoardHandle,PA,&x);
        BYTE a=x;
        InputByte(GlobalBoardHandle,PB,&x);
        BYTE b=x;
        aktpos=(b<<8)|a;


        // Hier die Steuerungsfunktion aufrufen
        stgfunc(HPL,HPR,T1,T2,&M_Re,&M_Li,&M_An,&pstate,aktpos );

        // Hier die Ausgabesignale ausgeben

        output=output&~((1<<BIT_POS_MOTOR_AN)|(1<<BIT_POS_FAHRE_NACH_OBEN)|(1<<BIT_POS_FAHRE_NACH_UNTEN));
        output=output|((M_Re)<<BIT_POS_FAHRE_NACH_UNTEN)|((M_Li)<<BIT_POS_FAHRE_NACH_OBEN)|((M_An)<<BIT_POS_MOTOR_AN);

        OutputByte(GlobalBoardHandle,PD,output);

        }


        }
