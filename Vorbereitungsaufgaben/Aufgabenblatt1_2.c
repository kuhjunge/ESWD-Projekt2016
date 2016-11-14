/*
 * Aufgabenblatt1_2.c
 *
 * Created: 25.10.2016 17:12:34
 * Version 11
 *  Author: Chris Deter, Alexandra Scheben, Dirk Teschner
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>		// Interrupts

// Definieren von Booleans
#define FALSE 0
#define TRUE !FALSE

// Definieren von einzelnen Bits
#define N0  0x01 
#define N1  0x02 
#define N2  0x04 
#define N3  0x08 
#define N4  0x10 
#define N5  0x20 
#define N6  0x40 
#define N7  0x80 

// Definieren von Pins
#define PINUP 0
#define PINDOWN 1
#define PINLEFT 2
#define PINRIGHT 3
#define PINENTER 4
#define PINCANCEL 5

#define MAXARRAY 42

/************************************************************************/
/* Variablen für das Muster                                             */
/************************************************************************/
typedef enum
{
	ende, up, down, left, right

} muster_t;

//spezielle Anzeigemuster
typedef enum
{
	ready, correct, wrong

} blinkseq_t;

muster_t eingMuster = ende;
muster_t muster[MAXARRAY];
muster_t checkMuster[MAXARRAY];

// 
unsigned char enterIsPressed = FALSE;
unsigned char cancelIsPressed = FALSE;
volatile unsigned char timerIsRinging = FALSE;

unsigned char state = 0; // Speichert den Status der aktuellen Knöpfe
unsigned int level = 0; 
unsigned int debug = 0;
unsigned int randCounter = 0;

/************************************************************************/
/* Wechselt die Wertigkeit eines bestimmten Bits im Char                */
/************************************************************************/
char toggleBit(char var, char n, char x)
{
	return var ^= (-x ^ var) & (1 << n);
}

/************************************************************************/
/* Gibt das Bit einer bestimmten Stelle im Char zurück                  */
/************************************************************************/
char getBit(char id, int position)
{
	// return id & (1 << position); // NULL or NON ZERO
   return (id >> position) & 1;
}

/************************************************************************/
/* Setzt die Variablen nach einer Eingabe							    */
/************************************************************************/
void doChange(char pin, int number) {
		if (pin != 0){
			if (number == PINCANCEL) {cancelIsPressed = TRUE;}
		} else {
			if (number == PINLEFT) {eingMuster = left;}	
			else if (number == PINRIGHT) {eingMuster = right;}	
			else if (number == PINUP) {eingMuster = up;}
			else if (number == PINDOWN) {eingMuster = down;}
			else if (number == PINENTER) {enterIsPressed = TRUE;}
//				else if (number == PINCANCEL) {}
		}
}

/************************************************************************/
/* Schaltet eine LED an oder aus										*/
/* SetLED(left,TRUE) schaltet die linke LED an 							*/
/************************************************************************/
void setLED(muster_t m, int on){
	if (on){
		if (m ==  left) {PORTA |= N6; }
		else if (m == right) {PORTC |= N6; }
		else if (m == up) {PORTB |= N5; }
		else if (m == down) {PORTB |= N7; }
	}
	else {
		if (m == left) {PORTA &= ~N6;}
		else if (m == right) {PORTC &= ~N6;}
		else if (m == up) {PORTB &= ~N5;}
		else if (m == down) {PORTB &= ~N7;}
	}
}

/************************************************************************/
/* Initialisieren von Timer1.                                           */
/************************************************************************/
void timer1Init (float sec) {
	// Taktquelle ist per Default der Systemtakt, Asynchrones Verhalten
	// wird in separatem ASSR ? Asynchronous Status Register konfiguriert

	// Timer-Modus: Clear Timer on Compare match (CTC)
	// d.h Zaehlen bis Wert in OCR1A, dann Reset
	// Konfigbits dafuer in TCCR1A und TCCR1B verteilt
	// Prescaler steht in Bit 2..0 von TCCR1B
	TCCR1A = 0x00;		//CTC ON
	TCCR1B = 0x0D;		//Prescaler 1024; Grundfreq = 1 MHz / 1024 = 977Hz
	// mit Fuse CLKDIV8 gesetzt sind es effektiv 1MHz
	
	// Konfigurieren der "Output Compare Units"
	// (Vergleich des Zaehlerwertes auf Schwellwert --> Interrupt)
	//	OCR1BH = 0x00;		// 0,2*1.000.000 / 1024 = 195 fuer 0,2s (overflow compare)
	//	OCR1BL = 0xC3;		// 194 -> 0x00C3
	OCR1A = 977 * sec;		// 1.000.000 / 1024 = 977 fuer eine Sekunde
	// Nun die relevanten Interrupts aktivieren: Timer Interrupt Mask
	TIMSK1 = (1<<1);	// Bit 1 ? OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
	//	TIMSK1 |= (1<<2);	// Bit 2 ? OCIE1B: Timer/Counter1, Output Compare B Match Interrupt Enable
	
	TCNT1 = 0x00;		// Zaehlregister des Timers noch auf Null stellen
}

/************************************************************************/
/* Compare Interrupt A													*/
/* schaltet die PortA-LED ein und die Port B - LED aus					*/
/************************************************************************/
ISR (TIMER1_COMPA_vect) {
	timerIsRinging = TRUE;
}

/************************************************************************/
/* Setzt den Timercounter zurück                                        */
/************************************************************************/
void resetWait(){
	TCNT1 = 0x00;		// Zaehlregister des Timers auf Null setzen
}

/************************************************************************/
/* Stopt den Timer                                                      */
/************************************************************************/
void cleanWait(){
	TCCR1B = 0x08;
}
/************************************************************************/
/* Wartet eine festgelegte Anzahl von Sekunden,							*/
/* zweites Argument bestimmt, ob der Aufruf blockierend ist oder nicht	*/
/************************************************************************/
void wait(float sec, unsigned char block){
	timerIsRinging	= FALSE;
	
	cli();			// vor dem Initialisieren Interrupts aus!
	timer1Init(sec);
	sei();			// Sicherstellen, dass Interrupts wieder an sind
	
	while (!timerIsRinging && block);
	if (block){
		cleanWait();
	}
}

/************************************************************************/
/* gibt ein Muster aus mit einer Sek verzögerung zwischen den Zeichen	*/
/************************************************************************/
void ShowMuster(muster_t m[]){
	int zaehler = 0;
	while (m[zaehler] != ende)
	{
		setLED(m[zaehler],TRUE);
		wait(1, TRUE);
		cleanWait();
		setLED(m[zaehler],FALSE);
		zaehler++;
	}
}

/************************************************************************/
/* Prüft, ob sich die Eingabe geändert hat und veranlasst doChange()    */
/* Via Polling															*/
/************************************************************************/
void checkAndDoChange(char pin, int number) {
	if(getBit(state, number) != pin){
		state = toggleBit(state, number, pin);
		doChange(pin, number);
		// setzte den Timer zurück, da Eingabe
		resetWait(); // ein wenig unsauber, aber durchs polling nicht schöner lösbar
	}
}

/************************************************************************/
/* Zeigt ein vordefiniertes Muster                                      */
/************************************************************************/
void showDefineMuster(blinkseq_t b){
	int i;
	// Muster Bereitschaftsmodus 
	if(b == ready) {
		
		for(i = 0; i < 3; i++) {
			setLED(up, TRUE);
			setLED(right, TRUE);
			setLED(down, TRUE);
			setLED(left, TRUE);
			
			wait(1, TRUE);
			
			setLED(up, FALSE);
			setLED(right, FALSE);
			setLED(down, FALSE);
			setLED(left, FALSE);			
		}
		
	}
	// Muster richtige Eingabe
	else if(b == correct) {
		
		setLED(up, TRUE);
		setLED(right, TRUE);
		setLED(down, TRUE);
		setLED(left, TRUE);
					
		wait(1.5, TRUE);
					
		setLED(up, FALSE);
		setLED(right, FALSE);
		setLED(down, FALSE);
		setLED(left, FALSE);
		
	}
	// Muster falsche Eingabe
	else if(b == wrong) {
		
		setLED(up, TRUE);
		setLED(right, FALSE);
		setLED(down, FALSE);
		setLED(left, FALSE);
		
		wait(0.25, TRUE);
		
		setLED(up, FALSE);
		setLED(right, TRUE);
		setLED(down, FALSE);
		setLED(left, FALSE);
		
		wait(0.25, TRUE);
		
		setLED(up, FALSE);
		setLED(right, FALSE);
		setLED(down, TRUE);
		setLED(left, FALSE);
		
		wait(0.25, TRUE);
		
		setLED(up, FALSE);
		setLED(right, FALSE);
		setLED(down, FALSE);
		setLED(left, TRUE);
		
		wait(0.25, TRUE);
		
		setLED(up, FALSE);
		setLED(right, FALSE);
		setLED(down, FALSE);
		setLED(left, FALSE);	
		
	}
}

/************************************************************************/
/* Loop für den "Spiel ist Bereit" Modus                                */
/************************************************************************/
void isReadyForGame(void){
	cancelIsPressed = FALSE;
	level = 0;
	while(level < 1) // Springt weiter nachdem ENTER losgelassen (!) wird
    {
		debug = getBit(PINA,PINENTER);
		checkAndDoChange(debug, PINENTER);
		if (enterIsPressed){
			level = 1;
		}
		randCounter++;
    }
	srand(randCounter);
	showDefineMuster(ready);
}

/************************************************************************/
/* Prüft, ob das Eingabemuster mit dem Ausgabemuster übereinstimmt      */
/************************************************************************/
void auswertung(void){
	int zaehler = 0;
	int musterokay = TRUE;
	if (checkMuster[0] == ende){
		showDefineMuster(wrong); // Fehler
		wait(1, TRUE);
	} else {
		while (muster[zaehler] != ende && checkMuster[zaehler] != ende)
		{
			if (muster[zaehler] != checkMuster[zaehler])
			{
				musterokay = FALSE;
			} 
			zaehler++;
		}
		if (musterokay) {
			showDefineMuster(correct); // Korrekt
		} else {
			showDefineMuster(wrong); // Fehler
			wait(1, TRUE);
		}
	}
}

/************************************************************************/
/* Liest die Eingabe des Nutzers                                        */
/************************************************************************/
void getEingabe(void){
	int zeichen = 0; // anzahl Muster Zähler
	int nextLvl = FALSE;
	timerIsRinging = FALSE;
	wait(5, FALSE);
//	resetWait();
	while(!cancelIsPressed && !nextLvl)
	{
		enterIsPressed = FALSE;
		// Lasse Muster vom Spieler eingeben
		checkAndDoChange(getBit(PINA,PINUP), PINUP);
		checkAndDoChange(getBit(PINA,PINDOWN), PINDOWN);
		checkAndDoChange(getBit(PINA,PINLEFT), PINLEFT);
		checkAndDoChange(getBit(PINA,PINDOWN), PINDOWN);
		if (eingMuster != ende){
			checkMuster[zeichen] =  eingMuster; //checkMuster[0] =  'Benuztereingabe'
			eingMuster = ende;
			if (MAXARRAY > zeichen) {zeichen++; }
		}
		checkAndDoChange(getBit(PINA,PINCANCEL), PINCANCEL);
		checkAndDoChange(getBit(PINA,PINENTER), PINENTER);
		if (enterIsPressed && !timerIsRinging){
			checkMuster[zeichen] = ende;
			auswertung(); // TODO, das Auswertungsergebnis irgendwie verwenden
			nextLvl = TRUE;
			level++;
		} else if (timerIsRinging) { // länger als 5 sek keine Eingabe, Auswertung (falsch)
			checkMuster[0] = ende;
			auswertung();
			cancelIsPressed = TRUE;
		}
		// TODO Nach Auswertung ->  Ausgabe von level (anzahl der Muster) + nextLvl = TRUE;
	}
}

/************************************************************************/
/* Generiert ein zufälliges Muster                                      */
/************************************************************************/
void genMuster(int laenge){
	int i;
	muster_t musterTeil = ende;
	
	for (i = 0; i < laenge; i++) {
		int r = rand() % 4 + 1;
		switch (r)
		{
			case 1:
			musterTeil = up;
			break;
		
			case 2:
			musterTeil = down;
			break;
		
			case 3:
			musterTeil = left;
			break;
		
			case 4:
			musterTeil = right;
			break;
		
			default:
			break;
		}
		muster[i] = musterTeil;	
	}
	muster[i+1] = ende;
}

/************************************************************************/
/* Loop für den "Spiel läuft" Status                                    */
/************************************************************************/
void isinGame(void){
    while(!cancelIsPressed) // wartet nicht länger als 5 Sec 
    {	
		genMuster(level);
		ShowMuster(muster); // Zeige Muster mit Pause an (sichtbar)
		getEingabe(); // Eingabe vom Nutzer erhalten
		checkAndDoChange(getBit(PINA,PINCANCEL +1), PINCANCEL);
    }
}

// Hier beginnt die Magie
int main(void)
{
	// Bereit machen
	DDRA  = N6;			// set lower nibble of PORTA to output
	PORTA = 0x00;		// all LEDs off
	DDRB  = N5 | N7;	// set lower nibble of PORTB to output
	PORTB = 0x00;		// all LEDs off
	DDRC  = N6;			// set lower nibble of PORTB to output
	PORTC = 0x00;		// all LEDs off
	
	// Spiel starten
	while(1)
	{
		isReadyForGame();
		isinGame();		
	}
}