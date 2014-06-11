#include <Wire.h>  
#include <DS1307.h>       
#include <LiquidCrystal_I2C.h>
#include <Adafruit_PWMServoDriver.h>
#include <EEPROMex.h>
#include <Flash.h>
#include <OneWire.h>
#include <DallasTemperature.h>
//#include <AFMotor.h>

// ********************************************* //
//                 VARIABLES                     //
// ********************************************* //
typedef struct {
  boolean Active;
  long Sunrise;  // 34200;    // 9:30
  long Sunset;   // 77400;    //21:30
  int Min;       // 0 = off
  int Max;       // 100% = 4095
  int Dim_in;    // Wie schnell soll hochgedimmt werden
  int Dim_out;   // Wie schnell wird heruntergedimmt
  boolean Invert;
}LIGHT;
//boolean moonSim=false,cloudSim=false;

Time LED1E, LED1A, 
     LED2E, LED2A,
     LED3E, LED3A,
     LED4E, LED4A,
     LED5E, LED5A,
     LED6E, LED6A,
     LED7E, LED7A,
     LED8E, LED8A,
     LED9E, LED9A,
     LED10E, LED10A,
     LED11E, LED11A,
     LED12E, LED12A,
     LED13E, LED13A,
     LED14E, LED14A,
     LED15E, LED15A;
     //LED16E, LED16A;

int LED1Min, LED1Max, LED1Dim_in, LED1Dim_out,
    LED2Min, LED2Max, LED2Dim_in, LED2Dim_out,
    LED3Min, LED3Max, LED3Dim_in, LED3Dim_out,
    LED4Min, LED4Max, LED4Dim_in, LED4Dim_out,
    LED5Min, LED5Max, LED5Dim_in, LED5Dim_out,
    LED6Min, LED6Max, LED6Dim_in, LED6Dim_out,
    LED7Min, LED7Max, LED7Dim_in, LED7Dim_out,
    LED8Min, LED8Max, LED8Dim_in, LED8Dim_out,
    LED9Min, LED9Max, LED9Dim_in, LED9Dim_out,
    LED10Min, LED10Max, LED10Dim_in, LED10Dim_out,
    LED11Min, LED11Max, LED11Dim_in, LED11Dim_out,
    LED12Min, LED12Max, LED12Dim_in, LED12Dim_out,
    LED13Min, LED13Max, LED13Dim_in, LED13Dim_out,
    LED14Min, LED14Max, LED14Dim_in, LED14Dim_out,
    LED15Min, LED15Max, LED15Dim_in, LED15Dim_out;
    //LED16Min, LED16Max, LED16Dim_in, LED16Dim_out;

boolean Active1, Active2, Active3, Active4, Active5, Active6, Active7, Active8, Active9, Active10, Active11, Active12, Active13, Active14, Active15, //Active16;
        Invert1, Invert2, Invert3, Invert4, Invert5, Invert6, Invert7, Invert8, Invert9, Invert10, Invert11, Invert12, Invert13, Invert14, Invert15; //Invert16;
        
//  Erstellung eines Fish-Symbols das nach rechts schaut
byte newChar[8] = {
	B00000,
	B00000,
	B10001,
	B11011,
	B11111,
	B11111,
	B11001,
	B10000
};

byte newChar1[8] = {
	B00001,
	B00010,
	B11101,
	B11010,
	B11111,
	B11110,
	B11100,
	B00000
};
//  Erstellung eines Fish-Symbols das nach links schaut
byte newChar2[8] = {
	B00000,
	B00000,
	B10001,
	B11011,
	B11111,
	B11111,
	B10011,
	B00001
};
byte newChar3[8] = {
	B10000,
	B01000,
	B10111,
	B01011,
	B11111,
	B01111,
	B00111,
	B00000
};
//  Erstellen eines Sonnensymbols
byte newChar4[8] = {
	B00111,
	B01110,
	B11100,
	B11000,
	B11000,
	B11100,
	B01110,
	B00111
};
byte newChar5[8] = {
	B01010,
	B10000,
	B00010,
	B01001,
	B00100,
	B10000,
	B00100,
	B10001
};
//  Erstellen eines Mondsymbols
byte newChar6[8] = {
	B01010,
	B00100,
	B00001,
	B11011,
	B00011,
	B00001,
	B01000,
	B10010
};
byte newChar7[8] = {
	B10010,
	B00100,
	B10000,
	B11011,
	B11000,
	B10000,
	B00010,
	B01001
};    

DS1307 rtc(56, 57);
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 20 chars and 4 line display, Backlight PWM PIN 10
#define ONE_WIRE_BUS 13       // Data wire is plugged into port 13 on the Arduino

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x41);

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 

// Pass our oneWire reference to Dallas Temperature.  
DallasTemperature sensors(&oneWire); 

// arrays to hold device address
DeviceAddress lampeThermometer1, lampeThermometer2, wasserThermometer, raumThermometer;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ K O N S T A N T E N +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const int DISPLAY_LETZTE_SEITE = 3;
const int DISPLAY_WARTEZEIT = 3;
const long HOUR = 60 * 60;
const long MINUTE = 60;

Time rtcClock;
long lTime;
long Sekunde;
long Minute;
long HourTime;

const int memBase          = 0;

//Hier werden die Tasten des Keyboards festgelegt und müssen je nach Keypad angepasst werden
#define RIGHTKEY 3
#define UPKEY 1
#define DOWNKEY 2
#define LEFTKEY 0
#define SELECTKEY 4

// Tasten
//int adc_key_val[5] ={50, 250, 460, 650, 800 };
//int adc_key_val[5] ={50, 240, 430, 630, 990 };
//int adc_key_val[5] ={ 50, 200, 400, 600, 800 };
int adc_key_val[5] ={50, 300, 550, 700, 900 };
int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
int oldkey=-1;

//Deklaration des Lichtkanal-Arrays
LIGHT light_channels[15];

//Variablen Display
int intSekunden;
int intDisplayAktuelleSeite = 0;
int intDisplayLetzteSeite = 0;
int intDisplayWartezeit = 0;

//Menühelper
int SetDisplayHelper = 0;
int SetDisplayHelperDate = 0;

//Menü
int MenuTiefe = 0;
int MenuEbene0 = 0;
int MenuEbene1 = 0;
int MenuEbene2 = 0;
int MenuEbene3 = 0;
boolean FirstSelect = false;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++ H I N T E R G R U N D B E L E U C H T U N G ++++++++++++++++++++++++++++++++++++++++++++++++++
long HintergrundbeleuchtungsAnZeit;
long HintergrundbeleuchtungsAusZeit;
Time HintergrundbeleuchtungAn;
Time HintergrundbeleuchtungAus;
boolean prevBacklightSwitch = 0;
boolean backlightSwitch = 0; 



//-------------------------------- B U Z Z E R / A L A R M P O R T - P O R T  ---------------------------
const int BUZZERPORT = 9;   // Alarm on Digital Pin 1
//-------------------------------------------------------------------------------------------------------
//-------------------------------- L I Q U I D - L E V E L S E N S O R - P O R T  ---------------------------
const int S_Sensor = 24;         // Fill Level Sensors are connected to digital port 24 (Schwimmerschalter 1) Technikbecken Füllstand High
int liquidSensorValue = 0;
//-------------------------------------------------------------------------------------------------------


//HD44780U (LCD-II)Oktal-Codes für Umlaute (-see page 17 on Datasheet for ROM Code: A00-):
//ö =  \357
//ä =  \341
//ü =  \365

//Sonderzeichen:
// ° = \337 oder char(223)

//Hauptmenü
const int MENUHAUPT = 4;
char sMenuHaupt[MENUHAUPT + 1][21] = {"Starten", "Grundeinstellung", "Lampe           ", "Temp            ","Dosierpumpe     "};

//Grundeinstellung-Menue
const int MENUGRUNDEINSTELLUNG = 5;
char sMenuGrundeinstellung[MENUGRUNDEINSTELLUNG + 1][21] = {"Hauptmen\365", "Datum", "Uhrzeit", "Tag", "LCD An       Uhr", "LCD Aus      Uhr"};

//Lampeneinstellung
const int MENULAMPENEINSTELLUNG = 16;
char sMenuLampeneinstellung[MENULAMPENEINSTELLUNG + 1][21] = {"Hauptmen\365", "LED1", "LED2", "LED3", "LED4", "LED5", "LED6", "LED7", "LED8", "LED9", "LED10", "LED11", "LED12", "LED13", "LED14", "LED15", "Mond"};

//LED
const int MENULED = 6;
char sMenuLED[MENULED + 1][21] = {"zur\365ck", "Min                %", "Max                %", "Start", "Ende", "Dauer Start      min", "Dauer Ende      min" };
String DP;

//Mond //Temp
const int MENUMINMAX = 2;
char sMenuMinMax[MENUMINMAX + 1][21] = {"zur\365ck", "Min", "Max" };
double Temp1Min;
double Temp1Max;
double tempLampe1 = 0;
double tempLampe2 = 0;
double tempWasser = 0;
double tempRaum   = 0;

//Dosierpumpe-Menü
char sMenuDosierpumpeneinstellung[5][17] = {  
  "Hauptmen\365", "Dosierer 1", "Dosierer 2", "Dosierer 3", "Dosierer 4" }; 
const int MENUDOSIERPUMPENEINSTELLUNG = 4;

//Dosierpumpe-Untermenü
char sMenuDosierpumpeneinstellung2[6][17] = {  
  "Hauptmen\365", "Manuell Betrieb ", "Kalibrieren     ", "Nachf\365llautomat ", "Speed           ", "Dosieren        " };
const int MENUDOSIERPUMPENEINSTELLUNG2 = 5;

//--------------------------------D O S I E R P U M P E N - M O D U L E-----------------------------------------------------------------------------
//--------------------------------------------Dosierpumpe 1-----------------------------------------------------------------------------------------
const int M1_MENUDOSIERPORT_1 = 2;
//AF_DCMotor motor1(1, MOTOR12_64KHZ); // create motor #1, 1KHz pwm; MOTOR12_64KHZ, MOTOR12_8KHZ, MOTOR12_2KHZ, orMOTOR12_1KHZ is possible

const int MENUDOSIERER1MANUELL = 1;
char sMenuDosierer1Manuell[MENUDOSIERER1MANUELL + 1][17] = {
  "zur\365ck          ", "Manuell         "  }; //ml in 1 Minute gemessen; wieviel ml soll dosiert werden; wann soll dosiert werden
boolean Dosiermanuell_1;

const int MENUDOSIERER1KALIB = 2;
char sMenuDosierer1Kalib[MENUDOSIERER1KALIB + 1][17] = {
  "zur\365ck          ", "Kalib        ml", "Set Time Loop   " }; //ml in 1 Minute gemessen; wieviel ml soll dosiert werden; wann soll dosiert werden
int Kalibrierung1;
long Endtime1;

const int MENUDOSIERER1NACHFUELL = 1;
char sMenuDosierer1Nachfuell[MENUDOSIERER1NACHFUELL + 1][17] = {
  "zur\365ck          ", "Nachf\365llaut.    "  }; //ml in 1 Minute gemessen; wieviel ml soll dosiert werden; wann soll dosiert werden
boolean Dosiernachfuell_1;

const int MENUDOSIERER1SPEED = 1;
char sMenuDosierer1Speed[MENUDOSIERER1SPEED + 1][17] = {
  "zur\365ck          ", "Speed           "  }; //mit welcher Geschwindigkeit soll sich die Pumpe drehen; Werte von 0-255 sind moeglich
int Dosierspeed_1;

const int MENUDOSIERER1DOSIEREN = 13;
char sMenuDosierer1Dosieren[MENUDOSIERER1DOSIEREN + 1][17] = {
  "zur\365ck          ", "Dosiermenge   ml", "Dos-Zeit1       ", "Dos-Zeit2       ", "Dos-Zeit3       ", "Dos-Zeit4       ", "Dos-Zeit5       ", "Dos-Zeit6       ", "Dos-Zeit7       ", "Dos-Zeit8       ", "Dos-Zeit9       ", "Dos-Zeit10      ", "Dos-Zeit11      ", "Dos-Zeit12      " }; //ml in 1 Minute gemessen; wieviel ml soll dosiert werden; wann soll dosiert werden
int Dosiermenge1;
Time Dosierung1_1;
Time Dosierung1_2;
Time Dosierung1_3;
Time Dosierung1_4;
Time Dosierung1_5;
Time Dosierung1_6;
Time Dosierung1_7;
Time Dosierung1_8;
Time Dosierung1_9;
Time Dosierung1_10;
Time Dosierung1_11;
Time Dosierung1_12;
long Dosierdauer1;
long Dosierautomatzeit1_1;
long Dosierautomatzeit1_2;
long Dosierautomatzeit1_3;
long Dosierautomatzeit1_4;
long Dosierautomatzeit1_5;
long Dosierautomatzeit1_6;
long Dosierautomatzeit1_7;
long Dosierautomatzeit1_8;
long Dosierautomatzeit1_9;
long Dosierautomatzeit1_10;
long Dosierautomatzeit1_11;
long Dosierautomatzeit1_12;
//-----------------------------------------bis hier hin:  Dosierpumpe 1-------------------------------------------------------------------------------

//--------------------------------------------Dosierpumpe 2-----------------------------------------------------------------------------------------
const int M1_MENUDOSIERPORT_2 = 44;
//AF_DCMotor motor2(2, MOTOR12_64KHZ); // create motor #2, 1KHz pwm; MOTOR12_64KHZ, MOTOR12_8KHZ, MOTOR12_2KHZ, orMOTOR12_1KHZ is possible

const int MENUDOSIERER2MANUELL = 1;
char sMenuDosierer2Manuell[MENUDOSIERER2MANUELL + 1][17] = {
  "zur\365ck          ", "Manuell         "  }; //ml in 1 Minute gemessen; wieviel ml soll dosiert werden; wann soll dosiert werden
boolean Dosiermanuell_2;

const int MENUDOSIERER2KALIB = 2;
char sMenuDosierer2Kalib[MENUDOSIERER2KALIB + 1][17] = {
  "zur\365ck          ", "Kalib        ml", "Set Time Loop   " }; //ml in 1 Minute gemessen; wieviel ml soll dosiert werden; wann soll dosiert werden
int Kalibrierung2;
long Endtime2;

const int MENUDOSIERER2NACHFUELL = 1;
char sMenuDosierer2Nachfuell[MENUDOSIERER2NACHFUELL + 1][17] = {
  "zur\365ck          ", "Nachf\365llaut.    "  }; //ml in 1 Minute gemessen; wieviel ml soll dosiert werden; wann soll dosiert werden
boolean Dosiernachfuell_2;

const int MENUDOSIERER2SPEED = 1;
char sMenuDosierer2Speed[MENUDOSIERER2SPEED + 1][17] = {
  "zur\365ck          ", "Speed           "  }; //mit welcher Geschwindigkeit soll sich die Pumpe drehen; Werte von 0-255 sind moeglich
int Dosierspeed_2;

const int MENUDOSIERER2DOSIEREN = 13;
char sMenuDosierer2Dosieren[MENUDOSIERER2DOSIEREN + 1][17] = {
  "zur\365ck          ", "Dosiermenge   ml", "Dos-Zeit1       ", "Dos-Zeit2       ", "Dos-Zeit3       ", "Dos-Zeit4       ", "Dos-Zeit5       ", "Dos-Zeit6       ", "Dos-Zeit7       ", "Dos-Zeit8       ", "Dos-Zeit9       ", "Dos-Zeit10      ", "Dos-Zeit11      ", "Dos-Zeit12      " }; //ml in 1 Minute gemessen; wieviel ml soll dosiert werden; wann soll dosiert werden
int Dosiermenge2;
Time Dosierung2_1;
Time Dosierung2_2;
Time Dosierung2_3;
Time Dosierung2_4;
Time Dosierung2_5;
Time Dosierung2_6;
Time Dosierung2_7;
Time Dosierung2_8;
Time Dosierung2_9;
Time Dosierung2_10;
Time Dosierung2_11;
Time Dosierung2_12;
long Dosierdauer2;
long Dosierautomatzeit2_1;
long Dosierautomatzeit2_2;
long Dosierautomatzeit2_3;
long Dosierautomatzeit2_4;
long Dosierautomatzeit2_5;
long Dosierautomatzeit2_6;
long Dosierautomatzeit2_7;
long Dosierautomatzeit2_8;
long Dosierautomatzeit2_9;
long Dosierautomatzeit2_10;
long Dosierautomatzeit2_11;
long Dosierautomatzeit2_12;
//-----------------------------------------bis hier hin:  Dosierpumpe 2-------------------------------------------------------------------------------

//--------------------------------------------Dosierpumpe 3-----------------------------------------------------------------------------------------
const int M1_MENUDOSIERPORT_3 = 45;
//AF_DCMotor motor3(3, MOTOR34_1KHZ); // create motor #3, 1KHz pwm; only 1KHZ is possible

const int MENUDOSIERER3MANUELL = 1;
char sMenuDosierer3Manuell[MENUDOSIERER3MANUELL + 1][17] = {
  "zur\365ck          ", "Manuell         "  }; //ml in 1 Minute gemessen; wieviel ml soll dosiert werden; wann soll dosiert werden
boolean Dosiermanuell_3;

const int MENUDOSIERER3KALIB = 2;
char sMenuDosierer3Kalib[MENUDOSIERER3KALIB + 1][17] = {
  "zur\365ck          ", "Kalib        ml", "Set Time Loop   " }; //ml in 1 Minute gemessen; wieviel ml soll dosiert werden; wann soll dosiert werden
int Kalibrierung3;
long Endtime3;

const int MENUDOSIERER3NACHFUELL = 1;
char sMenuDosierer3Nachfuell[MENUDOSIERER3NACHFUELL + 1][17] = {
  "zur\365ck          ", "Nachf\365llaut.    "  }; //ml in 1 Minute gemessen; wieviel ml soll dosiert werden; wann soll dosiert werden
boolean Dosiernachfuell_3;

const int MENUDOSIERER3SPEED = 1;
char sMenuDosierer3Speed[MENUDOSIERER3SPEED + 1][17] = {
  "zur\365ck          ", "Speed           "  }; //mit welcher Geschwindigkeit soll sich die Pumpe drehen; Werte von 0-255 sind moeglich
int Dosierspeed_3;

const int MENUDOSIERER3DOSIEREN = 13;
char sMenuDosierer3Dosieren[MENUDOSIERER3DOSIEREN + 1][17] = {
  "zur\365ck          ", "Dosiermenge   ml", "Dos-Zeit1       ", "Dos-Zeit2       ", "Dos-Zeit3       ", "Dos-Zeit4       ", "Dos-Zeit5       ", "Dos-Zeit6       ", "Dos-Zeit7       ", "Dos-Zeit8       ", "Dos-Zeit9       ", "Dos-Zeit10      ", "Dos-Zeit11      ", "Dos-Zeit12      " }; //ml in 1 Minute gemessen; wieviel ml soll dosiert werden; wann soll dosiert werden
int Dosiermenge3;
Time Dosierung3_1;
Time Dosierung3_2;
Time Dosierung3_3;
Time Dosierung3_4;
Time Dosierung3_5;
Time Dosierung3_6;
Time Dosierung3_7;
Time Dosierung3_8;
Time Dosierung3_9;
Time Dosierung3_10;
Time Dosierung3_11;
Time Dosierung3_12;
long Dosierdauer3;
long Dosierautomatzeit3_1;
long Dosierautomatzeit3_2;
long Dosierautomatzeit3_3;
long Dosierautomatzeit3_4;
long Dosierautomatzeit3_5;
long Dosierautomatzeit3_6;
long Dosierautomatzeit3_7;
long Dosierautomatzeit3_8;
long Dosierautomatzeit3_9;
long Dosierautomatzeit3_10;
long Dosierautomatzeit3_11;
long Dosierautomatzeit3_12;
//-----------------------------------------bis hier hin:  Dosierpumpe 3-------------------------------------------------------------------------------

//--------------------------------------------Dosierpumpe 4-----------------------------------------------------------------------------------------
const int M1_MENUDOSIERPORT_4 = 46;
//AF_DCMotor motor4(4, MOTOR34_1KHZ); // create motor #4, 1KHz pwm; only 1KHZ is possible

const int MENUDOSIERER4MANUELL = 1;
char sMenuDosierer4Manuell[MENUDOSIERER4MANUELL + 1][17] = {
  "zur\365ck          ", "Manuell         "  }; //ml in 1 Minute gemessen; wieviel ml soll dosiert werden; wann soll dosiert werden
boolean Dosiermanuell_4;

const int MENUDOSIERER4KALIB = 2;
char sMenuDosierer4Kalib[MENUDOSIERER4KALIB + 1][17] = {
  "zur\365ck          ", "Kalib        ml", "Set Time Loop   " }; //ml in 1 Minute gemessen; wieviel ml soll dosiert werden; wann soll dosiert werden
int Kalibrierung4;
long Endtime4;

const int MENUDOSIERER4NACHFUELL = 1;
char sMenuDosierer4Nachfuell[MENUDOSIERER4NACHFUELL + 1][17] = {
  "zur\365ck          ", "Nachf\365llaut.    "  }; //ml in 1 Minute gemessen; wieviel ml soll dosiert werden; wann soll dosiert werden
boolean Dosiernachfuell_4;

const int MENUDOSIERER4SPEED = 1;
char sMenuDosierer4Speed[MENUDOSIERER4SPEED + 1][17] = {
  "zur\365ck          ", "Speed           "  }; //mit welcher Geschwindigkeit soll sich die Pumpe drehen; Werte von 0-255 sind moeglich
int Dosierspeed_4;

const int MENUDOSIERER4DOSIEREN = 13;
char sMenuDosierer4Dosieren[MENUDOSIERER4DOSIEREN + 1][17] = {
  "zur\365ck          ", "Dosiermenge   ml", "Dos-Zeit1       ", "Dos-Zeit2       ", "Dos-Zeit3       ", "Dos-Zeit4       ", "Dos-Zeit5       ", "Dos-Zeit6       ", "Dos-Zeit7       ", "Dos-Zeit8       ", "Dos-Zeit9       ", "Dos-Zeit10      ", "Dos-Zeit11      ", "Dos-Zeit12      " }; //ml in 1 Minute gemessen; wieviel ml soll dosiert werden; wann soll dosiert werden
int Dosiermenge4;
Time Dosierung4_1;
Time Dosierung4_2;
Time Dosierung4_3;
Time Dosierung4_4;
Time Dosierung4_5;
Time Dosierung4_6;
Time Dosierung4_7;
Time Dosierung4_8;
Time Dosierung4_9;
Time Dosierung4_10;
Time Dosierung4_11;
Time Dosierung4_12;
long Dosierdauer4;
long Dosierautomatzeit4_1;
long Dosierautomatzeit4_2;
long Dosierautomatzeit4_3;
long Dosierautomatzeit4_4;
long Dosierautomatzeit4_5;
long Dosierautomatzeit4_6;
long Dosierautomatzeit4_7;
long Dosierautomatzeit4_8;
long Dosierautomatzeit4_9;
long Dosierautomatzeit4_10;
long Dosierautomatzeit4_11;
long Dosierautomatzeit4_12;
//-----------------------------------------bis hier hin:  Dosierpumpe 4-------------------------------------------------------------------------------


//Luefter
#define FAN          10                              // D10 PWM pin
int     FanSpeed;                                    // Luefter Geschwindigkeit
int     fanSwitch = 0;
int     prevFanSwitch = 0;

//Mond
float LC = 29.53059;                 //1 Lunar Cycle = 29.53059 days
String LP;                           //LP = Lunar Phase - variable used to print out Moon Phase
double AG;
int MondMax;                         //Maximum Illumination of Moon (User Defined/Set in Prog. -- Default = 0)
//int MondMin = 43; 
int MondMin;                     //Highest Value (0-4095) at which Led is Still OFF, or the Value
                                     //you wish the New Moon to Shine (it will increase from here to MI)
int moonled_out;                    // current LED output value

///*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ D I M M U N G S T A B E L L E +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
FLASH_ARRAY(int, pwmtable,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
                          3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                          5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9,
                          9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12,
                          12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 16, 16, 16,
                          16, 16, 16, 16, 16, 16, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19, 19, 19, 19, 19, 19, 20, 20, 20, 20, 20,
                          20, 20, 20, 21, 21, 21, 21,21, 21, 21, 21, 22, 22, 22, 22, 22, 22, 22, 22, 23,23, 23, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24,24, 25, 25, 25, 25, 25,
                          25, 25, 25, 26, 26, 26, 26,26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28,28, 28, 28, 28, 29, 29, 29, 29, 29, 29, 30, 30, 30,30, 30, 30, 30, 31, 31,
                          31, 31, 31, 31, 31, 32, 32,32, 32, 32, 32, 33, 33, 33, 33, 33, 33, 34, 34, 34,34, 34, 34, 34, 35, 35, 35, 35, 35, 35, 36, 36, 36,36, 36, 36, 37, 37, 37,
                          37, 37, 37, 38, 38, 38, 38,38, 39, 39, 39, 39, 39, 39, 40, 40, 40, 40, 40, 40,41, 41, 41, 41, 41, 42, 42, 42, 42, 42, 42, 43, 43,43, 43, 43, 44, 44, 44,
                          44, 44, 44, 45, 45, 45, 45,45, 46, 46, 46, 46, 46, 47, 47, 47, 47, 47, 47, 48,48, 48, 48, 48, 49, 49, 49, 49, 49, 50, 50, 50, 50,50, 51, 51, 51, 51, 51,
                          52, 52, 52, 52, 52, 53, 53,53, 53, 53, 54, 54, 54, 54, 55, 55, 55, 55, 55, 56,56, 56, 56, 56, 57, 57, 57, 57, 57, 58, 58, 58, 58,59, 59, 59, 59, 59, 60,
                          60, 60, 60, 61, 61, 61, 61,61, 62, 62, 62, 62, 63, 63, 63, 63, 63, 64, 64, 64,64, 65, 65, 65, 65, 65, 66, 66, 66, 66, 67, 67, 67,67, 68, 68, 68, 68, 69,
                          69, 69, 69, 69, 70, 70, 70,70, 71, 71, 71, 71, 72, 72, 72, 72, 73, 73, 73, 73,74, 74, 74, 74, 75, 75, 75, 75, 76, 76, 76, 76, 77,77, 77, 77, 78, 78, 78,
                          78, 79, 79, 79, 79, 80, 80,80, 80, 81, 81, 81, 81, 82, 82, 82, 82, 83, 83, 83,84, 84, 84, 84, 85, 85, 85, 85, 86, 86, 86, 86, 87,87, 87, 88, 88, 88, 88,
                          89, 89, 89, 89, 90, 90, 90,91, 91, 91, 91, 92, 92, 92, 92, 93, 93, 93, 94, 94,94, 94, 95, 95, 95, 96, 96, 96, 96, 97, 97, 97, 98,98, 98, 98, 99, 99, 99,
                          100, 100, 100, 100, 101,101, 101, 102, 102, 102, 102, 103, 103, 103, 104,104, 104, 105, 105, 105, 105, 106, 106, 106, 107,107, 107, 108, 108, 108, 108,
                          109, 109, 109, 110,110, 110, 111, 111, 111, 112, 112, 112, 112, 113,113, 113, 114, 114, 114, 115, 115, 115, 116, 116,116, 116, 117, 117, 117, 118, 118,
                          118, 119, 119,119, 120, 120, 120, 121, 121, 121, 122, 122, 122,123, 123, 123, 124, 124, 124, 125, 125, 125, 126,126, 126, 127, 127, 127, 127, 128, 128,
                          128, 129,129, 129, 130, 130, 131, 131, 131, 132, 132, 132,133, 133, 133, 134, 134, 134, 135, 135, 135, 136,136, 136, 137, 137, 137, 138, 138, 138, 139,
                          139,139, 140, 140, 140, 141, 141, 141, 142, 142, 143,143, 143, 144, 144, 144, 145, 145, 145, 146, 146,146, 147, 147, 148, 148, 148, 149, 149, 149, 150,
                          150, 150, 151, 151, 152, 152, 152, 153, 153, 153,154, 154, 154, 155, 155, 156, 156, 156, 157, 157,157, 158, 158, 159, 159, 159, 160, 160, 160, 161,161,
                          162, 162, 162, 163, 163, 163, 164, 164, 165,165, 165, 166, 166, 166, 167, 167, 168, 168, 168,169, 169, 170, 170, 170, 171, 171, 171, 172, 172,173, 173,
                          173, 174, 174, 175, 175, 175, 176, 176,177, 177, 177, 178, 178, 179, 179, 179, 180, 180,181, 181, 181, 182, 182, 183, 183, 183, 184, 184,185, 185, 185,
                          186, 186, 187, 187, 187, 188, 188,189, 189, 190, 190, 190, 191, 191, 192, 192, 192,193, 193, 194, 194, 194, 195, 195, 196, 196, 197,197, 197, 198, 198,
                          199, 199, 200, 200, 200, 201,201, 202, 202, 203, 203, 203, 204, 204, 205, 205,206, 206, 206, 207, 207, 208, 208, 209, 209, 209,210, 210, 211, 211, 212,
                          212, 212, 213, 213, 214,214, 215, 215, 216, 216, 216, 217, 217, 218, 218,219, 219, 220, 220, 220, 221, 221, 222, 222, 223,223, 224, 224, 224, 225, 225,
                          226, 226, 227, 227,228, 228, 229, 229, 229, 230, 230, 231, 231, 232,232, 233, 233, 234, 234, 235, 235, 235, 236, 236,237, 237, 238, 238, 239, 239, 240,
                          240, 241, 241,241, 242, 242, 243, 243, 244, 244, 245, 245, 246,246, 247, 247, 248, 248, 249, 249, 250, 250, 251,251, 251, 252, 252, 253, 253, 254, 254,
                          255, 255,256, 256, 257, 257, 258, 258, 259, 259, 260, 260,261, 261, 262, 262, 263, 263, 264, 264, 265, 265,266, 266, 267, 267, 268, 268, 269, 269, 270,
                          270,271, 271, 272, 272, 273, 273, 274, 274, 275, 275,276, 276, 277, 277, 278, 278, 279, 279, 280, 280,281, 281, 282, 282, 283, 283, 284, 284, 285, 285,
                          286, 286, 287, 287, 288, 288, 289, 289, 290, 290,291, 292, 292, 293, 293, 294, 294, 295, 295, 296,296, 297, 297, 298, 298, 299, 299, 300, 300, 301,301,
                          302, 303, 303, 304, 304, 305, 305, 306, 306,307, 307, 308, 308, 309, 309, 310, 311, 311, 312,312, 313, 313, 314, 314, 315, 315, 316, 317, 317,318, 318,
                          319, 319, 320, 320, 321, 321, 322, 323,323, 324, 324, 325, 325, 326, 326, 327, 328, 328,329, 329, 330, 330, 331, 331, 332, 333, 333, 334,334, 335, 335,
                          336, 336, 337, 338, 338, 339, 339,340, 340, 341, 342, 342, 343, 343, 344, 344, 345,346, 346, 347, 347, 348, 348, 349, 350, 350, 351,351, 352, 352, 353,
                          354, 354, 355, 355, 356, 356,357, 358, 358, 359, 359, 360, 361, 361, 362, 362,363, 363, 364, 365, 365, 366, 366, 367, 368, 368,369, 369, 370, 371, 371,
                          372, 372, 373, 374, 374,375, 375, 376, 377, 377, 378, 378, 379, 380, 380,381, 381, 382, 383, 383, 384, 384, 385, 386, 386,387, 387, 388, 389, 389, 390,
                          390, 391, 392, 392,393, 393, 394, 395, 395, 396, 397, 397, 398, 398,399, 400, 400, 401, 402, 402, 403, 403, 404, 405,405, 406, 406, 407, 408, 408, 409,
                          410, 410, 411,411, 412, 413, 413, 414, 415, 415, 416, 417, 417,418, 418, 419, 420, 420, 421, 422, 422, 423, 424,424, 425, 425, 426, 427, 427, 428, 429,
                          429, 430,431, 431, 432, 433, 433, 434, 434, 435, 436, 436,437, 438, 438, 439, 440, 440, 441, 442, 442, 443,444, 444, 445, 446, 446, 447, 447, 448, 449,
                          449,450, 451, 451, 452, 453, 453, 454, 455, 455, 456,457, 457, 458, 459, 459, 460, 461, 461, 462, 463,463, 464, 465, 465, 466, 467, 467, 468, 469, 469,
                          470, 471, 472, 472, 473, 474, 474, 475, 476, 476,477, 478, 478, 479, 480, 480, 481, 482, 482, 483,484, 484, 485, 486, 487, 487, 488, 489, 489, 490,491,
                          491, 492, 493, 493, 494, 495, 496, 496, 497,498, 498, 499, 500, 500, 501, 502, 502, 503, 504,505, 505, 506, 507, 507, 508, 509, 510, 510, 511,512, 512,
                          513, 514, 514, 515, 516, 517, 517, 518,519, 519, 520, 521, 522, 522, 523, 524, 524, 525,526, 527, 527, 528, 529, 530, 530, 531, 532, 532,533, 534, 535,
                          535, 536, 537, 537, 538, 539, 540,540, 541, 542, 543, 543, 544, 545, 545, 546, 547,548, 548, 549, 550, 551, 551, 552, 553, 554, 554,555, 556, 557, 557,
                          558, 559, 560, 560, 561, 562,562, 563, 564, 565, 565, 566, 567, 568, 568, 569,570, 571, 571, 572, 573, 574, 574, 575, 576, 577,577, 578, 579, 580, 581,
                          581, 582, 583, 584, 584,585, 586, 587, 587, 588, 589, 590, 590, 591, 592,593, 593, 594, 595, 596, 597, 597, 598, 599, 600,600, 601, 602, 603, 603, 604,
                          605, 606, 607, 607,608, 609, 610, 610, 611, 612, 613, 614, 614, 615,616, 617, 618, 618, 619, 620, 621, 621, 622, 623,624, 625, 625, 626, 627, 628, 629,
                          629, 630, 631,632, 633, 633, 634, 635, 636, 636, 637, 638, 639,640, 640, 641, 642, 643, 644, 644, 645, 646, 647,648, 649, 649, 650, 651, 652, 653, 653,
                          654, 655,656, 657, 657, 658, 659, 660, 661, 661, 662, 663,664, 665, 666, 666, 667, 668, 669, 670, 670, 671,672, 673, 674, 675, 675, 676, 677, 678, 679,
                          679,680, 681, 682, 683, 684, 684, 685, 686, 687, 688,89, 689, 690, 691, 692, 693, 694, 694, 695, 696,697, 698, 699, 699, 700, 701, 702, 703, 704, 705,
                          705, 706, 707, 708, 709, 710, 710, 711, 712, 713,714, 715, 716, 716, 717, 718, 719, 720, 721, 721,722, 723, 724, 725, 726, 727, 727, 728, 729, 730,731,
                          732, 733, 733, 734, 735, 736, 737, 738, 739,740, 740, 741, 742, 743, 744, 745, 746, 746, 747,748, 749, 750, 751, 752, 753, 753, 754, 755, 756,757, 758,
                          759, 760, 760, 761, 762, 763, 764, 765,766, 767, 767, 768, 769, 770, 771, 772, 773, 774,775, 775, 776, 777, 778, 779, 780, 781, 782, 783,783, 784, 785,
                          786, 787, 788, 789, 790, 791, 792,792, 793, 794, 795, 796, 797, 798, 799, 800, 801,801, 802, 803, 804, 805, 806, 807, 808, 809, 810,810, 811, 812, 813,
                          814, 815, 816, 817, 818, 819,820, 821, 821, 822, 823, 824, 825, 826, 827, 828,829, 830, 831, 832, 832, 833, 834, 835, 836, 837,838, 839, 840, 841, 842,
                          843, 844, 845, 845, 846,847, 848, 849, 850, 851, 852, 853, 854, 855, 856,857, 858, 859, 859, 860, 861, 862, 863, 864, 865,866, 867, 868, 869, 870, 871,
                          872, 873, 874, 875,876, 876, 877, 878, 879, 880, 881, 882, 883, 884,885, 886, 887, 888, 889, 890, 891, 892, 893, 894,895, 896, 897, 897, 898, 899, 900,
                          901, 902, 903,904, 905, 906, 907, 908, 909, 910, 911, 912, 913,914, 915, 916, 917, 918, 919, 920, 921, 922, 923,924, 925, 926, 927, 928, 929, 929, 930,
                          931, 932,933, 934, 935, 936, 937, 938, 939, 940, 941, 942,943, 944, 945, 946, 947, 948, 949, 950, 951, 952,953, 954, 955, 956, 957, 958, 959, 960, 961,
                          962,963, 964, 965, 966, 967, 968, 969, 970, 971, 972,973, 974, 975, 976, 977, 978, 979, 980, 981, 982,983, 984, 985, 986, 987, 988, 989, 990, 991, 992,
                          993, 994, 995, 996, 997, 998, 999, 1000, 1002,1003, 1004, 1005, 1006, 1007, 1008, 1009, 1010,1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018,1019, 1020,
                          1021, 1022, 1023, 1024, 1025, 1026,1027, 1028, 1029, 1030, 1031, 1032, 1033, 1034,1035, 1037, 1038, 1039, 1040, 1041, 1042, 1043,1044, 1045, 1046, 1047,
                          1048, 1049, 1050, 1051,1052, 1053, 1054, 1055, 1056, 1057, 1058, 1060,1061, 1062, 1063, 1064, 1065, 1066, 1067, 1068,1069, 1070, 1071, 1072, 1073, 1074,
                          1075, 1076,1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085,1086, 1087, 1088, 1089, 1090, 1091, 1092, 1094,1095, 1096, 1097, 1098, 1099, 1100, 1101, 1102,
                          1103, 1104, 1105, 1106, 1107, 1109, 1110, 1111,1112, 1113, 1114, 1115, 1116, 1117, 1118, 1119,1120, 1122, 1123, 1124, 1125, 1126, 1127, 1128,1129, 1130,
                          1131, 1132, 1134, 1135, 1136, 1137,1138, 1139, 1140, 1141, 1142, 1143, 1144, 1146,1147, 1148, 1149, 1150, 1151, 1152, 1153, 1154,1155, 1157, 1158, 1159,
                          1160, 1161, 1162, 1163,1164, 1165, 1167, 1168, 1169, 1170, 1171, 1172,1173, 1174, 1175, 1177, 1178, 1179, 1180, 1181,1182, 1183, 1184, 1185, 1187, 1188,
                          1189, 1190,1191, 1192, 1193, 1194, 1196, 1197, 1198, 1199,1200, 1201, 1202, 1203, 1205, 1206, 1207, 1208,1209, 1210, 1211, 1212, 1214, 1215, 1216, 1217,
                          1218, 1219, 1220, 1222, 1223, 1224, 1225, 1226,1227, 1228, 1230, 1231, 1232, 1233, 1234, 1235,1236, 1238, 1239, 1240, 1241, 1242, 1243, 1244,1246, 1247,
                          1248, 1249, 1250, 1251, 1252, 1254,1255, 1256, 1257, 1258, 1259, 1261, 1262, 1263,1264, 1265, 1266, 1268, 1269, 1270, 1271, 1272,1273, 1274, 1276, 1277,
                          1278, 1279, 1280, 1281,1283, 1284, 1285, 1286, 1287, 1288, 1290, 1291,1292, 1293, 1294, 1296, 1297, 1298, 1299, 1300,1301, 1303, 1304, 1305, 1306, 1307,
                          1308, 1310,1311, 1312, 1313, 1314, 1316, 1317, 1318, 1319,1320, 1322, 1323, 1324, 1325, 1326, 1327, 1329,1330, 1331, 1332, 1333, 1335, 1336, 1337, 1338,
                          1339, 1341, 1342, 1343, 1344, 1345, 1347, 1348,1349, 1350, 1351, 1353, 1354, 1355, 1356, 1357,1359, 1360, 1361, 1362, 1363, 1365, 1366, 1367,1368, 1369,
                          1371, 1372, 1373, 1374, 1376, 1377,1378, 1379, 1380, 1382, 1383, 1384, 1385, 1386,1388, 1389, 1390, 1391, 1393, 1394, 1395, 1396,1397, 1399, 1400, 1401,
                          1402, 1404, 1405, 1406,1407, 1408, 1410, 1411, 1412, 1413, 1415, 1416,1417, 1418, 1420, 1421, 1422, 1423, 1425, 1426,1427, 1428, 1429, 1431, 1432, 1433,
                          1434, 1436,1437, 1438, 1439, 1441, 1442, 1443, 1444, 1446,447, 1448, 1449, 1451, 1452, 1453, 1454, 1456,1457, 1458, 1459, 1461, 1462, 1463, 1464, 1466,
                          1467, 1468, 1469, 1471, 1472, 1473, 1474, 1476,1477, 1478, 1480, 1481, 1482, 1483, 1485, 1486,1487, 1488, 1490, 1491, 1492, 1493, 1495, 1496,1497, 1499,
                          1500, 1501, 1502, 1504, 1505, 1506,1507, 1509, 1510, 1511, 1513, 1514, 1515, 1516,1518, 1519, 1520, 1521, 1523, 1524, 1525, 1527,1528, 1529, 1530, 1532,
                          1533, 1534, 1536, 1537,1538, 1540, 1541, 1542, 1543, 1545, 1546, 1547,1549, 1550, 1551, 1552, 1554, 1555, 1556, 1558,1559, 1560, 1562, 1563, 1564, 1565,
                          1567, 1568,1569, 1571, 1572, 1573, 1575, 1576, 1577, 1578,1580, 1581, 1582, 1584, 1585, 1586, 1588, 1589,1590, 1592, 1593, 1594, 1596, 1597, 1598, 1599,
                          1601, 1602, 1603, 1605, 1606, 1607, 1609, 1610,1611, 1613, 1614, 1615, 1617, 1618, 1619, 1621,1622, 1623, 1625, 1626, 1627, 1629, 1630, 1631,1633, 1634,
                          1635, 1637, 1638, 1639, 1641, 1642,1643, 1645, 1646, 1647, 1649, 1650, 1651, 1653,1654, 1655, 1657, 1658, 1659, 1661, 1662, 1663,1665, 1666, 1667, 1669,
                          1670, 1671, 1673, 1674,1676, 1677, 1678, 1680, 1681, 1682, 1684, 1685,1686, 1688, 1689, 1690, 1692, 1693, 1695, 1696,1697, 1699, 1700, 1701, 1703, 1704,
                          1705, 1707,1708, 1709, 1711, 1712, 1714, 1715, 1716, 1718,1719, 1720, 1722, 1723, 1725, 1726, 1727, 1729,1730, 1731, 1733, 1734, 1736, 1737, 1738, 1740,
                          1741, 1742, 1744, 1745, 1747, 1748, 1749, 1751,1752, 1754, 1755, 1756, 1758, 1759, 1760, 1762,1763, 1765, 1766, 1767, 1769, 1770, 1772, 1773,1774, 1776,
                          1777, 1779, 1780, 1781, 1783, 1784,1786, 1787, 1788, 1790, 1791, 1793, 1794, 1795,1797, 1798, 1800, 1801, 1802, 1804, 1805, 1807,1808, 1809, 1811, 1812,
                          1814, 1815, 1816, 1818,1819, 1821, 1822, 1824, 1825, 1826, 1828, 1829,1831, 1832, 1833, 1835, 1836, 1838, 1839, 1841,1842, 1843, 1845, 1846, 1848, 1849,
                          1851, 1852,1853, 1855, 1856, 1858, 1859, 1861, 1862, 1863,1865, 1866, 1868, 1869, 1871, 1872, 1873, 1875,1876, 1878, 1879, 1881, 1882, 1883, 1885, 1886,
                          1888, 1889, 1891, 1892, 1894, 1895, 1896, 1898,1899, 1901, 1902, 1904, 1905, 1907, 1908, 1910,1911, 1912, 1914, 1915, 1917, 1918, 1920, 1921,1923, 1924,
                          1926, 1927, 1928, 1930, 1931, 1933,1934, 1936, 1937, 1939, 1940, 1942, 1943, 1945,1946, 1947, 1949, 1950, 1952, 1953, 1955, 1956,1958, 1959, 1961, 1962,
                          1964, 1965, 1967, 1968,1970, 1971, 1972, 1974, 1975, 1977, 1978, 1980,1981, 1983, 1984, 1986, 1987, 1989, 1990, 1992,1993, 1995, 1996, 1998, 1999, 2001,
                          2002, 2004,2005, 2007, 2008, 2010, 2011, 2013, 2014, 2016,2017, 2019, 2020, 2022, 2023, 2025, 2026, 2028,2029, 2031, 2032, 2034, 2035, 2037, 2038, 2040,
                          2041, 2043, 2044, 2046, 2047, 2049, 2050, 2052,2053, 2055, 2056, 2058, 2059, 2061, 2062, 2064,2065, 2067, 2068, 2070, 2071, 2073, 2074, 2076,2077, 2079,
                          2080, 2082, 2083, 2085, 2086, 2088,2090, 2091, 2093, 2094, 2096, 2097, 2099, 2100,2102, 2103, 2105, 2106, 2108, 2109, 2111, 2112,2114, 2116, 2117, 2119,
                          2120, 2122, 2123, 2125,2126, 2128, 2129, 2131, 2132, 2134, 2136, 2137,2139, 2140, 2142, 2143, 2145, 2146, 2148, 2149,2151, 2153, 2154, 2156, 2157, 2159,
                          2160, 2162,2163, 2165, 2166, 2168, 2170, 2171, 2173, 2174,2176, 2177, 2179, 2180, 2182, 2184, 2185, 2187,2188, 2190, 2191, 2193, 2195, 2196, 2198, 2199,
                          2201, 2202, 2204, 2206, 2207, 2209, 2210, 2212,2213, 2215, 2217, 2218, 2220, 2221, 2223, 2224,2226, 2228, 2229, 2231, 2232, 2234, 2235, 2237,2239, 2240,
                          2242, 2243, 2245, 2247, 2248, 2250,2251, 2253, 2254, 2256, 2258, 2259, 2261, 2262,2264, 2266, 2267, 2269, 2270, 2272, 2274, 2275,2277, 2278, 2280, 2282,
                          2283, 2285, 2286, 2288,2290, 2291, 2293, 2294, 2296, 2298, 2299, 2301,2302, 2304, 2306, 2307, 2309, 2310, 2312, 2314,2315, 2317, 2319, 2320, 2322, 2323,
                          2325, 2327,2328, 2330, 2331, 2333, 2335, 2336, 2338, 2340,2341, 2343, 2344, 2346, 2348, 2349, 2351, 2353,2354, 2356, 2357, 2359, 2361, 2362, 2364, 2366,
                          2367, 2369, 2370, 2372, 2374, 2375, 2377, 2379,2380, 2382, 2384, 2385, 2387, 2388, 2390, 2392,2393, 2395, 2397, 2398, 2400, 2402, 2403, 2405,2407, 2408,
                          2410, 2411, 2413, 2415, 2416, 2418,2420, 2421, 2423, 2425, 2426, 2428, 2430, 2431,2433, 2435, 2436, 2438, 2440, 2441, 2443, 2445,2446, 2448, 2450, 2451,
                          2453, 2455, 2456, 2458,2460, 2461, 2463, 2465, 2466, 2468, 2470, 2471,2473, 2475, 2476, 2478, 2480, 2481, 2483, 2485,2486, 2488, 2490, 2491, 2493, 2495,
                          2496, 2498,2500, 2501, 2503, 2505, 2506, 2508, 2510, 2511,2513, 2515, 2517, 2518, 2520, 2522, 2523, 2525,2527, 2528, 2530, 2532, 2533, 2535, 2537, 2539,
                          2540, 2542, 2544, 2545, 2547, 2549, 2550, 2552,2554, 2555, 2557, 2559, 2561, 2562, 2564, 2566,2567, 2569, 2571, 2573, 2574, 2576, 2578, 2579,2581, 2583,
                          2584, 2586, 2588, 2590, 2591, 2593,2595, 2596, 2598, 2600, 2602, 2603, 2605, 2607,2609, 2610, 2612, 2614, 2615, 2617, 2619, 2621,2622, 2624, 2626, 2627,
                          2629, 2631, 2633, 2634,2636, 2638, 2640, 2641, 2643, 2645, 2647, 2648,2650, 2652, 2653, 2655, 2657, 2659, 2660, 2662,2664, 2666, 2667, 2669, 2671, 2673,
                          2674, 2676,2678, 2680, 2681, 2683, 2685, 2687, 2688, 2690,2692, 2694, 2695, 2697, 2699, 2701, 2702, 2704,2706, 2708, 2709, 2711, 2713, 2715, 2716, 2718,
                          2720, 2722, 2723, 2725, 2727, 2729, 2730, 2732,2734, 2736, 2738, 2739, 2741, 2743, 2745, 2746,2748, 2750, 2752, 2753, 2755, 2757, 2759, 2761,2762, 2764,
                          2766, 2768, 2769, 2771, 2773, 2775,2777, 2778, 2780, 2782, 2784, 2785, 2787, 2789,2791, 2793, 2794, 2796, 2798, 2800, 2801, 2803,2805, 2807, 2809, 2810,
                          2812, 2814, 2816, 2818,2819, 2821, 2823, 2825, 2827, 2828, 2830, 2832,2834, 2836, 2837, 2839, 2841, 2843, 2845, 2846,2848, 2850, 2852, 2854, 2855, 2857,
                          2859, 2861,2863, 2864, 2866, 2868, 2870, 2872, 2874, 2875,2877, 2879, 2881, 2883, 2884, 2886, 2888, 2890,2892, 2894, 2895, 2897, 2899, 2901, 2903, 2904,
                          2906, 2908, 2910, 2912, 2914, 2915, 2917, 2919,2921, 2923, 2925, 2926, 2928, 2930, 2932, 2934,2936, 2937, 2939, 2941, 2943, 2945, 2947, 2948,2950, 2952,
                          2954, 2956, 2958, 2959, 2961, 2963,2965, 2967, 2969, 2971, 2972, 2974, 2976, 2978,2980, 2982, 2983, 2985, 2987, 2989, 2991, 2993,2995, 2996, 2998, 3000,
                          3002, 3004, 3006, 3008,3009, 3011, 3013, 3015, 3017, 3019, 3021, 3022,3024, 3026, 3028, 3030, 3032, 3034, 3036, 3037,3039, 3041, 3043, 3045, 3047, 3049,
                          3051, 3052,3054, 3056, 3058, 3060, 3062, 3064, 3066, 3067,3069, 3071, 3073, 3075, 3077, 3079, 3081, 3082,3084, 3086, 3088, 3090, 3092, 3094, 3096, 3098,
                          3099, 3101, 3103, 3105, 3107, 3109, 3111, 3113,3115, 3116, 3118, 3120, 3122, 3124, 3126, 3128,3130, 3132, 3134, 3135, 3137, 3139, 3141, 3143,3145, 3147,
                          3149, 3151, 3153, 3155, 3156, 3158,3160, 3162, 3164, 3166, 3168, 3170, 3172, 3174,3176, 3177, 3179, 3181, 3183, 3185, 3187, 3189,3191, 3193, 3195, 3197,
                          3199, 3200, 3202, 3204,3206, 3208, 3210, 3212, 3214, 3216, 3218, 3220,3222, 3224, 3226, 3227, 3229, 3231, 3233, 3235,3237, 3239, 3241, 3243, 3245, 3247,
                          3249, 3251,3253, 3255, 3257, 3258, 3260, 3262, 3264, 3266,3268, 3270, 3272, 3274, 3276, 3278, 3280, 3282,3284, 3286, 3288, 3290, 3292, 3294, 3295, 3297,
                          3299, 3301, 3303, 3305, 3307, 3309, 3311, 3313,3315, 3317, 3319, 3321, 3323, 3325, 3327, 3329,3331, 3333, 3335, 3337, 3339, 3341, 3343, 3345,3346, 3348,
                          3350, 3352, 3354, 3356, 3358, 3360,3362, 3364, 3366, 3368, 3370, 3372, 3374, 3376,3378, 3380, 3382, 3384, 3386, 3388, 3390, 3392,3394, 3396, 3398, 3400,
                          3402, 3404, 3406, 3408,3410, 3412, 3414, 3416, 3418, 3420, 3422, 3424,3426, 3428, 3430, 3432, 3434, 3436, 3438, 3440,3442, 3444, 3446, 3448, 3450, 3452,
                          3454, 3456,3458, 3460, 3462, 3464, 3466, 3468, 3470, 3472,3474, 3476, 3478, 3480, 3482, 3484, 3486, 3488,3490, 3492, 3494, 3496, 3498, 3500, 3502, 3504,
                          3506, 3508, 3510, 3512, 3514, 3516, 3518, 3520,3522, 3524, 3526, 3528, 3530, 3533, 3535, 3537,3539, 3541, 3543, 3545, 3547, 3549, 3551, 3553,3555, 3557,
                          3559, 3561, 3563, 3565, 3567, 3569,3571, 3573, 3575, 3577, 3579, 3581, 3583, 3585,3588, 3590, 3592, 3594, 3596, 3598, 3600, 3602,3604, 3606, 3608, 3610,
                          3612, 3614, 3616, 3618,3620, 3622, 3624, 3627, 3629, 3631, 3633, 3635,3637, 3639, 3641, 3643, 3645, 3647, 3649, 3651,3653, 3655, 3658, 3660, 3662, 3664,
                          3666, 3668,3670, 3672, 3674, 3676, 3678, 3680, 3682, 3684,3687, 3689, 3691, 3693, 3695, 3697, 3699, 3701,3703, 3705, 3707, 3709, 3711, 3714, 3716, 3718,
                          3720, 3722, 3724, 3726, 3728, 3730, 3732, 3734,3737, 3739, 3741, 3743, 3745, 3747, 3749, 3751,3753, 3755, 3758, 3760, 3762, 3764, 3766, 3768,3770, 3772,
                          3774, 3776, 3779, 3781, 3783, 3785,3787, 3789, 3791, 3793, 3795, 3798, 3800, 3802,3804, 3806, 3808, 3810, 3812, 3814, 3817, 3819,3821, 3823, 3825, 3827,
                          3829, 3831, 3834, 3836,3838, 3840, 3842, 3844, 3846, 3848, 3851, 3853,3855, 3857, 3859, 3861, 3863, 3865, 3868, 3870,3872, 3874, 3876, 3878, 3880, 3882,
                          3885, 3887,3889, 3891, 3893, 3895, 3897, 3900, 3902, 3904,3906, 3908, 3910, 3912, 3915, 3917, 3919, 3921,3923, 3925, 3928, 3930, 3932, 3934, 3936, 3938,
                          3940, 3943, 3945, 3947, 3949, 3951, 3953, 3956,3958, 3960, 3962, 3964, 3966, 3968, 3971, 3973,3975, 3977, 3979, 3981, 3984, 3986, 3988, 3990,3992, 3994,
                          3997, 3999, 4001, 4003, 4005, 4008,4010, 4012, 4014, 4016, 4018, 4021, 4023, 4025,4027, 4029, 4031, 4034, 4036, 4038, 4040, 4042,4045, 4047, 4049, 4051,
                          4053, 4056, 4058, 4060,4062, 4064, 4066, 4069, 4071, 4073, 4075, 4077,4080, 4082, 4084, 4086, 4088, 4091, 4093, 4095);








void setup() {
  Serial.begin(9600);    // Ausgabe am PC

  EEPROM.setMemPool(memBase, EEPROMSizeMega);
  
  lcd.init();
  lcd.backlight();
  
  lcd.createChar(0, newChar);
  lcd.createChar(1, newChar1);
  lcd.createChar(2, newChar2);
  lcd.createChar(3, newChar3);
  
  lcd.clear();
  
  lcd.setCursor(0, 0);
  //lcd.print("Init...");
  
  lcd.write(0);
  lcd.write(1);
  lcd.setCursor(2, 0);
  lcd.print("Univers V1.1");
  lcd.setCursor(15, 0);
  lcd.write(3);
  lcd.write(2);
  lcd.setCursor(2, 1);
  lcd.print("by Aquafish007");
  
  pwm.begin();
  pwm.setPWMFreq(1000);  // This is the maximum PWM frequency
//  pwm1.begin();
//  pwm1.setPWMFreq(1000);  // This is the maximum PWM frequency
    
  // save I2C bitrate
  uint8_t twbrbackup = TWBR;
  // must be changed after calling Wire.begin() (inside pwm.begin())
  TWBR = 12; // upgrade to 400KHz!
  
  //Unsere Uhr
  rtc.halt(false);
  
  //Thermometer
  sensors.begin();             // Start up the DS18B20 Temp library
  if (!sensors.getAddress(lampeThermometer1, 0)) Serial.println("Unable to find address for Device 0");
  if (!sensors.getAddress(lampeThermometer2, 1)) Serial.println("Unable to find address for Device 1");
  if (!sensors.getAddress(wasserThermometer, 2)) Serial.println("Unable to find address for Device 2"); 
  if (!sensors.getAddress(raumThermometer, 3)) Serial.println("Unable to find address for Device 3");
  
  //Luefter Test
  pinMode(FAN, OUTPUT);  
  digitalWrite(FAN, HIGH);                    // Volle Lueftergeschwindigkeit setzen
  delay(2000);
  digitalWrite(FAN, LOW);                     // Luefter auschalten
  
  Serial.print("EEPROM Wert:   ");
  Serial.println(EEPROM.readInt(0));
  requestEEPROM();
  
  //Alle PinModes
  pinMode(BUZZERPORT, OUTPUT); // set a pin for buzzer output
  pinMode(S_Sensor, INPUT); // set a pin for liquidsensor output
  
  //Initial Buzzing 
  //Untere 2 Zeilen Auskommentieren, wenn man beim Einschalten des Gerätes einen Ton hören möchte
  buzz(BUZZERPORT, 2500, 200); // buzz the buzzer on pin 48 at 2500Hz for 200 milliseconds
  buzz(BUZZERPORT, 3500, 200); // buzz the buzzer on pin 48 at 3500Hz for 200 milliseconds
  
  
//  //Temp1 (Lampe)
//  Temp1Min = 35.5;
//  Temp1Max = 40.5;
//  Temp1Alarm = true;
  
}

void loop() {
  //--------------------------------L A M P E N E I N S T E L L U N G S - M O D U L E----------------------
  LIGHT light_channels[15] = { 
  { Active1, get_ts(LED1E.hour,LED1E.min,0), get_ts(LED1A.hour,LED1A.min,0),LED1Min, LED1Max, LED1Dim_in, LED1Dim_out,Invert1},
  { Active2, get_ts(LED2E.hour,LED2E.min,0), get_ts(LED2A.hour,LED2A.min,0),LED2Min, LED2Max, LED2Dim_in, LED2Dim_out,Invert2},
  { Active3, get_ts(LED3E.hour,LED3E.min,0), get_ts(LED3A.hour,LED3A.min,0),LED3Min, LED3Max, LED3Dim_in, LED3Dim_out,Invert3},
  { Active4, get_ts(LED4E.hour,LED4E.min,0), get_ts(LED4A.hour,LED4A.min,0),LED4Min, LED4Max, LED4Dim_in, LED4Dim_out,Invert4},
  { Active5, get_ts(LED5E.hour,LED5E.min,0), get_ts(LED5A.hour,LED5A.min,0),LED5Min, LED5Max, LED5Dim_in, LED5Dim_out,Invert5}, 
  { Active6, get_ts(LED6E.hour,LED6E.min,0), get_ts(LED6A.hour,LED6A.min,0),LED6Min, LED6Max, LED6Dim_in, LED6Dim_out,Invert6}, 
  { Active7, get_ts(LED7E.hour,LED7E.min,0), get_ts(LED7A.hour,LED7A.min,0),LED7Min, LED7Max, LED7Dim_in, LED7Dim_out,Invert7}, 
  { Active8, get_ts(LED8E.hour,LED8E.min,0), get_ts(LED8A.hour,LED8A.min,0),LED8Min, LED8Max, LED8Dim_in, LED8Dim_out,Invert8}, 
  { Active9, get_ts(LED9E.hour,LED9E.min,0), get_ts(LED9A.hour,LED9A.min,0),LED9Min, LED9Max, LED9Dim_in, LED9Dim_out,Invert9}, 
  { Active10, get_ts(LED10E.hour,LED10E.min,0), get_ts(LED10A.hour,LED10A.min,0),LED10Min, LED10Max, LED10Dim_in, LED10Dim_out,Invert10}, 
  { Active11, get_ts(LED11E.hour,LED11E.min,0), get_ts(LED11A.hour,LED11A.min,0),LED11Min, LED11Max, LED11Dim_in, LED11Dim_out,Invert11}, 
  { Active12, get_ts(LED12E.hour,LED12E.min,0), get_ts(LED12A.hour,LED12A.min,0),LED12Min, LED12Max, LED12Dim_in, LED12Dim_out,Invert12}, 
  { Active13, get_ts(LED13E.hour,LED13E.min,0), get_ts(LED13A.hour,LED13A.min,0),LED13Min, LED13Max, LED13Dim_in, LED13Dim_out,Invert13}, 
  { Active14, get_ts(LED14E.hour,LED14E.min,0), get_ts(LED14A.hour,LED14A.min,0),LED14Min, LED14Max, LED14Dim_in, LED14Dim_out,Invert14}, 
  { Active15, get_ts(LED15E.hour,LED15E.min,0), get_ts(LED15A.hour,LED15A.min,0),LED15Min, LED15Max, LED15Dim_in, LED15Dim_out,Invert15}
  //{ Active16, get_ts(LED16E.hour,LED16E.min,0), get_ts(LED16A.hour,LED16A.min,0),LED16Min, LED16Max, LED16Dim_in, LED16Dim_out,Invert16}
  };
  
  
  Time time = rtc.getTime();
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++ A K T U E L L E  Z E I T ++++++++++++++++++++++++++++++++++++++++++++++++++
  lTime = (time.hour * HOUR) + (time.min * MINUTE) + time.sec;
  Sekunde = time.sec;
  Minute = (time.min * MINUTE);
  HourTime = Sekunde + Minute;

  //++++++++++++++++++++++++++++++++++++++++++++++++++++++ H I N T E R G R U N D B E L E U C H T U N G ++++++++++++++++++++++++++++++++++++++++++++++++++
  // Hier wird eingestellt wann sich die Hintergrundbeleuchtung für das Display ausschalten soll
  HintergrundbeleuchtungsAnZeit = (HintergrundbeleuchtungAn.hour * HOUR);
  HintergrundbeleuchtungsAusZeit = (HintergrundbeleuchtungAus.hour * HOUR);
  
  if (MenuTiefe > 0)
      {
        lcd.backlight();
        backlightSwitch = 1;
      }
            
  else if (((lTime >= HintergrundbeleuchtungsAusZeit) || (lTime <= HintergrundbeleuchtungsAnZeit) ) && (MenuTiefe == 0) )
     { 
       lcd.noBacklight(); 
     }
  else
     { 
       lcd.backlight();
     }
    
    if (backlightSwitch != prevBacklightSwitch)
    {
        if (backlightSwitch == 1)
          {
              lcd.backlight();
          }
          
     prevBacklightSwitch = backlightSwitch;
    }
  
  //Display
//  if (intSekunden != time.sec && MenuTiefe == 0)
//  {
//    if (intDisplayWartezeit == DISPLAY_WARTEZEIT)
//    {
//      Display(intDisplayAktuelleSeite);
//      intDisplayAktuelleSeite++;
//
//      if (intDisplayAktuelleSeite == DISPLAY_LETZTE_SEITE)
//        intDisplayAktuelleSeite = 0;
//
//      intDisplayWartezeit = 0;  
//    }
//    intSekunden = time.sec ;
//    intDisplayWartezeit++;
//  }

    if (intSekunden != time.sec && MenuTiefe == 0)
     {
       if (intDisplayWartezeit == DISPLAY_WARTEZEIT)
        {
          Display1( );
          intDisplayWartezeit = 0;
        }
        intSekunden = time.sec ;
        intDisplayWartezeit++;
     }
  
  key = 0;
  adc_key_in = analogRead(0);
  key = get_key(adc_key_in);
  if (key != oldkey)
  {
    delay(50);
    adc_key_in = analogRead(0);
    key = get_key(adc_key_in);
    if (key != oldkey)
    {
      oldkey = key;

      if (key >=0)
      {
        MenuWerteAuswahl();
        MenuWerteSetzen();       
      }
    }
  } 
  
  //Temperaturabfrage
  requestTemperature(lampeThermometer1);
  requestTemperature(lampeThermometer2);
  requestTemperature(wasserThermometer);
  requestTemperature(raumThermometer);
  
  //Luefteraktivitaet
  if ( (tempLampe1) >= Temp1Max) //|| ((tempLampe2) >= Temp1Max) ) //42
        {
          digitalWrite(FAN, HIGH);
//          lcd.setCursor(17, 3);
//          lcd.print("On ");
        }
      if ( (tempLampe1) < Temp1Min) //|| ((tempLampe2) < Temp1Min) ) //36
        {
          digitalWrite(FAN, LOW);
//          lcd.setCursor(17, 3);
//          lcd.print("Off");
        }
  
  
//  FanSpeed = map(tempLampe1, Temp1Min, Temp1Max, 0, 255);    // Temp1Min->0% // Temp1Max->100%
//  FanSpeed = map(tempLampe2, Temp1Min, Temp1Max, 0, 255);    // Temp1Min->0% // Temp1Max->100%
//  
//  if (FanSpeed<200) FanSpeed = 0; //155
//  if (FanSpeed < 55) FanSpeed = 0; //25
//  
//  if (FanSpeed >= 55)
//               { fanSwitch = 1; }
//            else
//               { fanSwitch = 0; } 
//           
//                 if (fanSwitch != prevFanSwitch){
//                      if (fanSwitch == 1)
//                        {
//                          FanSpeed = 255; //Der Luefter soll mal kurz volle Pulle anlaufen damit er überhaupt anlaeuft!
//                          delay(1000);
//                        }      
//                   prevFanSwitch = fanSwitch;
//                 }
     
//  if (FanSpeed > 255) FanSpeed = 255;
//  //Serial.println(FanSpeed); 
//  analogWrite(FAN, FanSpeed);               // PWM Geschwindigkeit setzen
 
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ L A M P E ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  for( int i=0;i <= 14; i++){
      if(light_channels[i].Active==1){
        int c_PWMProzent = dimmung(lTime, light_channels[i].Sunrise, light_channels[i].Sunset, light_channels[i].Dim_in, light_channels[i].Dim_out, light_channels[i].Min, light_channels[i].Max, light_channels[i].Invert,4095);
        int c_PWM = dimmungPWM(lTime, light_channels[i].Sunrise, light_channels[i].Sunset, light_channels[i].Dim_in, light_channels[i].Dim_out, light_channels[i].Min, light_channels[i].Max, light_channels[i].Invert,4095); 
        
        if(c_PWM<0){
          c_PWM=0;
        }else if(c_PWM>4095){
          c_PWM=4095;
        }
        
//        //Determine the Day Phase
//        if (c_PWM == 0)             //DayPhase: Night
//          { DP = "    Night   "; }
//        if (c_PWM == 4095)             //DayPhase: High Noon
//          { DP = "    High Noon   "; }
//        if (c_PWM == 4095)             //DayPhase: Sunrise
//          { DP = "    Sunrise   "; }
//        if (c_PWM == 4095)             //DayPhase: Sunset
//          { DP = "    Sunset   "; }

       
        
        //int c_Mond = MondPhase(time.year, time.mon, time.date);
        
        //----------------------------------------Mond Neu zum Test---------------------------------------------------------------------------------
        float lunarCycle = moonPhase(time.year, time.mon, time.date); //get a value for the lunar cycle
        //moonled_out = MondMin *(1 - lunarCycle) + MondMax * lunarCycle + 0.5;                  //MaximumIllumination * % of Full Moon (0-100)
        moonled_out = MondMin *(1 - lunarCycle) + MondMax * lunarCycle + 0.5;                  //MaximumIllumination * % of Full Moon (0-100)
        
        //Serial.print("moonled_out: ");
        //Serial.println(moonled_out);
        //Serial.print("phase: ");
        //Serial.println(phase);
        //Serial.print("Lunar Phase: ");
        //Serial.println(LP);
        
        //----------------bis hierhin!---------------------------------------------------------------------------------------------------------------
   
        
        //pwm.setPWM(i,0, c_PWM);
        pwm.setPWM(i,0, pwmtable[c_PWM]);
        
        //pwm.setPWM(15,0, c_Mond);
        pwm.setPWM(15,0, moonled_out);
        
//        if(i <= 15){
//        Serial.print("i: ");
//        Serial.println(i);
//        pwm.setPWM(i,0, pwmtable[c_PWM]);
//        }
//        else{
//          int j = (i-16);
//          Serial.print("j: ");
//          Serial.println(j);
//          pwm1.setPWM(j,0, pwmtable[c_PWM]);
//          //pwm.setPWM(15,0, c_Mond);
//          pwm1.setPWM(15,0, moonled_out);
//        }
        
        
        
        
        
      }
    }
    
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++ S C H W I M M E R S E N S O R    A U S L E S E N / S T A T U S A B F R A G E ++++++++++++++++++++++++++++++++++++++++++++++++++
  liquidSensorValue = digitalRead(S_Sensor);
//  Serial.print("Sensor Value: ");
//  Serial.println(liquidSensorValue);

  //+++++++++++++++++++++++++++++++++++++ D O S I E R P U M P E N +++++++++++++++++++++++++++++++++++++++++++++++++++

  Dosierdauer1 = kalibrieren ( Kalibrierung1, Dosiermenge1, Dosierdauer1);
  Dosierdauer2 = kalibrieren ( Kalibrierung2, Dosiermenge2, Dosierdauer2);
  Dosierdauer3 = kalibrieren ( Kalibrierung3, Dosiermenge3, Dosierdauer3);
  Dosierdauer4 = kalibrieren ( Kalibrierung4, Dosiermenge4, Dosierdauer4);

  Dosierautomatzeit1_1 = (Dosierung1_1.hour * HOUR) + (Dosierung1_1.min * MINUTE) + Dosierung1_1.sec;
  Dosierautomatzeit1_2 = (Dosierung1_2.hour * HOUR) + (Dosierung1_2.min * MINUTE) + Dosierung1_2.sec;
  Dosierautomatzeit1_3 = (Dosierung1_3.hour * HOUR) + (Dosierung1_3.min * MINUTE) + Dosierung1_3.sec;
  Dosierautomatzeit1_4 = (Dosierung1_4.hour * HOUR) + (Dosierung1_4.min * MINUTE) + Dosierung1_4.sec;
  Dosierautomatzeit1_5 = (Dosierung1_5.hour * HOUR) + (Dosierung1_5.min * MINUTE) + Dosierung1_5.sec;
  Dosierautomatzeit1_6 = (Dosierung1_6.hour * HOUR) + (Dosierung1_6.min * MINUTE) + Dosierung1_6.sec;
  Dosierautomatzeit1_7 = (Dosierung1_7.hour * HOUR) + (Dosierung1_7.min * MINUTE) + Dosierung1_7.sec;
  Dosierautomatzeit1_8 = (Dosierung1_8.hour * HOUR) + (Dosierung1_8.min * MINUTE) + Dosierung1_8.sec;
  Dosierautomatzeit1_9 = (Dosierung1_9.hour * HOUR) + (Dosierung1_9.min * MINUTE) + Dosierung1_9.sec;
  Dosierautomatzeit1_10 = (Dosierung1_10.hour * HOUR) + (Dosierung1_10.min * MINUTE) + Dosierung1_10.sec;
  Dosierautomatzeit1_11 = (Dosierung1_11.hour * HOUR) + (Dosierung1_11.min * MINUTE) + Dosierung1_11.sec;
  Dosierautomatzeit1_12 = (Dosierung1_12.hour * HOUR) + (Dosierung1_12.min * MINUTE) + Dosierung1_12.sec;

  Dosierautomatzeit2_1 = (Dosierung2_1.hour * HOUR) + (Dosierung2_1.min * MINUTE) + Dosierung2_1.sec;
  Dosierautomatzeit2_2 = (Dosierung2_2.hour * HOUR) + (Dosierung2_2.min * MINUTE) + Dosierung2_2.sec;
  Dosierautomatzeit2_3 = (Dosierung2_3.hour * HOUR) + (Dosierung2_3.min * MINUTE) + Dosierung2_3.sec;
  Dosierautomatzeit2_4 = (Dosierung2_4.hour * HOUR) + (Dosierung2_4.min * MINUTE) + Dosierung2_4.sec;
  Dosierautomatzeit2_5 = (Dosierung2_5.hour * HOUR) + (Dosierung2_5.min * MINUTE) + Dosierung2_5.sec;
  Dosierautomatzeit2_6 = (Dosierung2_6.hour * HOUR) + (Dosierung2_6.min * MINUTE) + Dosierung2_6.sec;
  Dosierautomatzeit2_7 = (Dosierung2_7.hour * HOUR) + (Dosierung2_7.min * MINUTE) + Dosierung2_7.sec;
  Dosierautomatzeit2_8 = (Dosierung2_8.hour * HOUR) + (Dosierung2_8.min * MINUTE) + Dosierung2_8.sec;
  Dosierautomatzeit2_9 = (Dosierung2_9.hour * HOUR) + (Dosierung2_9.min * MINUTE) + Dosierung2_9.sec;
  Dosierautomatzeit2_10 = (Dosierung2_10.hour * HOUR) + (Dosierung2_10.min * MINUTE) + Dosierung2_10.sec;
  Dosierautomatzeit2_11 = (Dosierung2_11.hour * HOUR) + (Dosierung2_11.min * MINUTE) + Dosierung2_11.sec;
  Dosierautomatzeit2_12 = (Dosierung2_12.hour * HOUR) + (Dosierung2_12.min * MINUTE) + Dosierung2_12.sec;

  Dosierautomatzeit3_1 = (Dosierung3_1.hour * HOUR) + (Dosierung3_1.min * MINUTE) + Dosierung3_1.sec;
  Dosierautomatzeit3_2 = (Dosierung3_2.hour * HOUR) + (Dosierung3_2.min * MINUTE) + Dosierung3_2.sec;
  Dosierautomatzeit3_3 = (Dosierung3_3.hour * HOUR) + (Dosierung3_3.min * MINUTE) + Dosierung3_3.sec;
  Dosierautomatzeit3_4 = (Dosierung3_4.hour * HOUR) + (Dosierung3_4.min * MINUTE) + Dosierung3_4.sec;
  Dosierautomatzeit3_5 = (Dosierung3_5.hour * HOUR) + (Dosierung3_5.min * MINUTE) + Dosierung3_5.sec;
  Dosierautomatzeit3_6 = (Dosierung3_6.hour * HOUR) + (Dosierung3_6.min * MINUTE) + Dosierung3_6.sec;
  Dosierautomatzeit3_7 = (Dosierung3_7.hour * HOUR) + (Dosierung3_7.min * MINUTE) + Dosierung3_7.sec;
  Dosierautomatzeit3_8 = (Dosierung3_8.hour * HOUR) + (Dosierung3_8.min * MINUTE) + Dosierung3_8.sec;
  Dosierautomatzeit3_9 = (Dosierung3_9.hour * HOUR) + (Dosierung3_9.min * MINUTE) + Dosierung3_9.sec;
  Dosierautomatzeit3_10 = (Dosierung3_10.hour * HOUR) + (Dosierung3_10.min * MINUTE) + Dosierung3_10.sec;
  Dosierautomatzeit3_11 = (Dosierung3_11.hour * HOUR) + (Dosierung3_11.min * MINUTE) + Dosierung3_11.sec;
  Dosierautomatzeit3_12 = (Dosierung3_12.hour * HOUR) + (Dosierung3_12.min * MINUTE) + Dosierung3_12.sec;

  Dosierautomatzeit4_1 = (Dosierung4_1.hour * HOUR) + (Dosierung4_1.min * MINUTE) + Dosierung4_1.sec;
  Dosierautomatzeit4_2 = (Dosierung4_2.hour * HOUR) + (Dosierung4_2.min * MINUTE) + Dosierung4_2.sec;
  Dosierautomatzeit4_3 = (Dosierung4_3.hour * HOUR) + (Dosierung4_3.min * MINUTE) + Dosierung4_3.sec;
  Dosierautomatzeit4_4 = (Dosierung4_4.hour * HOUR) + (Dosierung4_4.min * MINUTE) + Dosierung4_4.sec;
  Dosierautomatzeit4_5 = (Dosierung4_5.hour * HOUR) + (Dosierung4_5.min * MINUTE) + Dosierung4_5.sec;
  Dosierautomatzeit4_6 = (Dosierung4_6.hour * HOUR) + (Dosierung4_6.min * MINUTE) + Dosierung4_6.sec;
  Dosierautomatzeit4_7 = (Dosierung4_7.hour * HOUR) + (Dosierung4_7.min * MINUTE) + Dosierung4_7.sec;
  Dosierautomatzeit4_8 = (Dosierung4_8.hour * HOUR) + (Dosierung4_8.min * MINUTE) + Dosierung4_8.sec;
  Dosierautomatzeit4_9 = (Dosierung4_9.hour * HOUR) + (Dosierung4_9.min * MINUTE) + Dosierung4_9.sec;
  Dosierautomatzeit4_10 = (Dosierung4_10.hour * HOUR) + (Dosierung4_10.min * MINUTE) + Dosierung4_10.sec;
  Dosierautomatzeit4_11 = (Dosierung4_11.hour * HOUR) + (Dosierung4_11.min * MINUTE) + Dosierung4_11.sec;
  Dosierautomatzeit4_12 = (Dosierung4_12.hour * HOUR) + (Dosierung4_12.min * MINUTE) + Dosierung4_12.sec;


  //1. Abfrage: Wann soll dosiert werden
  //2. Abfrage: manuelle Zeitschleifensteuerung zur Messung; hier default-Wert 60 Sekunden
  //3. Abfrage: manuelle Streuerung An/Aus
  //Dosierpumpe1
  if  (((lTime >= Dosierautomatzeit1_1) && (lTime <= Dosierautomatzeit1_1 + Dosierdauer1) && (Dosierautomatzeit1_1 != 0) && (Dosiernachfuell_1 == false)) ||
       ((lTime >= Dosierautomatzeit1_2) && (lTime <= Dosierautomatzeit1_2 + Dosierdauer1) && (Dosierautomatzeit1_2 != 0) && (Dosiernachfuell_1 == false)) ||
       ((lTime >= Dosierautomatzeit1_3) && (lTime <= Dosierautomatzeit1_3 + Dosierdauer1) && (Dosierautomatzeit1_3 != 0) && (Dosiernachfuell_1 == false)) ||
       ((lTime >= Dosierautomatzeit1_4) && (lTime <= Dosierautomatzeit1_4 + Dosierdauer1) && (Dosierautomatzeit1_4 != 0) && (Dosiernachfuell_1 == false)) ||
       ((lTime >= Dosierautomatzeit1_5) && (lTime <= Dosierautomatzeit1_5 + Dosierdauer1) && (Dosierautomatzeit1_5 != 0) && (Dosiernachfuell_1 == false)) ||
       ((lTime >= Dosierautomatzeit1_6) && (lTime <= Dosierautomatzeit1_6 + Dosierdauer1) && (Dosierautomatzeit1_6 != 0) && (Dosiernachfuell_1 == false)) ||
       ((lTime >= Dosierautomatzeit1_7) && (lTime <= Dosierautomatzeit1_7 + Dosierdauer1) && (Dosierautomatzeit1_7 != 0) && (Dosiernachfuell_1 == false)) ||
       ((lTime >= Dosierautomatzeit1_8) && (lTime <= Dosierautomatzeit1_8 + Dosierdauer1) && (Dosierautomatzeit1_8 != 0) && (Dosiernachfuell_1 == false)) ||
       ((lTime >= Dosierautomatzeit1_9) && (lTime <= Dosierautomatzeit1_9 + Dosierdauer1) && (Dosierautomatzeit1_9 != 0) && (Dosiernachfuell_1 == false)) ||
       ((lTime >= Dosierautomatzeit1_10) && (lTime <= Dosierautomatzeit1_10 + Dosierdauer1) && (Dosierautomatzeit1_10 != 0) && (Dosiernachfuell_1 == false)) || 
       ((lTime >= Dosierautomatzeit1_11) && (lTime <= Dosierautomatzeit1_11 + Dosierdauer1) && (Dosierautomatzeit1_11 != 0) && (Dosiernachfuell_1 == false)) ||
       ((lTime >= Dosierautomatzeit1_12) && (lTime <= Dosierautomatzeit1_12 + Dosierdauer1) && (Dosierautomatzeit1_12 != 0) && (Dosiernachfuell_1 == false)) ||
       ((Endtime1 > 0) && (lTime <= Endtime1)) || 
       (Dosiermanuell_1 == true) ||
       ((liquidSensorValue == 1) && (Dosiernachfuell_1 == true)))
        
  {        
    digitalWrite(M1_MENUDOSIERPORT_1, HIGH); //255; HIGH
    //motor1.setSpeed(Dosierspeed_1);
    //motor1.run(FORWARD);
    
  }
  else
  {
    digitalWrite(M1_MENUDOSIERPORT_1, LOW);
    //motor1.run(RELEASE);
    Endtime1 = 0;   
  }

  //Dosierpumpe2
  if  (((lTime >= Dosierautomatzeit2_1) && (lTime <= Dosierautomatzeit2_1 + Dosierdauer2) && (Dosierautomatzeit2_1 != 0) && (Dosiernachfuell_2 == false)) ||
       ((lTime >= Dosierautomatzeit2_2) && (lTime <= Dosierautomatzeit2_2 + Dosierdauer2) && (Dosierautomatzeit2_2 != 0) && (Dosiernachfuell_2 == false)) ||
       ((lTime >= Dosierautomatzeit2_3) && (lTime <= Dosierautomatzeit2_3 + Dosierdauer2) && (Dosierautomatzeit2_3 != 0) && (Dosiernachfuell_2 == false)) ||
       ((lTime >= Dosierautomatzeit2_4) && (lTime <= Dosierautomatzeit2_4 + Dosierdauer2) && (Dosierautomatzeit2_4 != 0) && (Dosiernachfuell_2 == false)) ||
       ((lTime >= Dosierautomatzeit2_5) && (lTime <= Dosierautomatzeit2_5 + Dosierdauer2) && (Dosierautomatzeit2_5 != 0) && (Dosiernachfuell_2 == false)) ||
       ((lTime >= Dosierautomatzeit2_6) && (lTime <= Dosierautomatzeit2_6 + Dosierdauer2) && (Dosierautomatzeit2_6 != 0) && (Dosiernachfuell_2 == false)) ||
       ((lTime >= Dosierautomatzeit2_7) && (lTime <= Dosierautomatzeit2_7 + Dosierdauer2) && (Dosierautomatzeit2_7 != 0) && (Dosiernachfuell_2 == false)) ||
       ((lTime >= Dosierautomatzeit2_8) && (lTime <= Dosierautomatzeit2_8 + Dosierdauer2) && (Dosierautomatzeit2_8 != 0) && (Dosiernachfuell_2 == false)) ||
       ((lTime >= Dosierautomatzeit2_9) && (lTime <= Dosierautomatzeit2_9 + Dosierdauer2) && (Dosierautomatzeit2_9 != 0) && (Dosiernachfuell_2 == false)) ||
       ((lTime >= Dosierautomatzeit2_10) && (lTime <= Dosierautomatzeit2_10 + Dosierdauer2) && (Dosierautomatzeit2_10 != 0) && (Dosiernachfuell_2 == false)) || 
       ((lTime >= Dosierautomatzeit2_11) && (lTime <= Dosierautomatzeit2_11 + Dosierdauer2) && (Dosierautomatzeit2_11 != 0) && (Dosiernachfuell_2 == false)) || 
       ((lTime >= Dosierautomatzeit2_12) && (lTime <= Dosierautomatzeit2_12 + Dosierdauer2) && (Dosierautomatzeit2_12 != 0) && (Dosiernachfuell_2 == false)) || 
       ((Endtime2 > 0) && (lTime <= Endtime2)) || 
       (Dosiermanuell_2 == true) ||
       ((liquidSensorValue == 1) && (Dosiernachfuell_2 == true)))
  {        
    digitalWrite(M1_MENUDOSIERPORT_2, HIGH); //255
    //motor2.setSpeed(Dosierspeed_2);
    //motor2.run(FORWARD);
  }
  else
  {
    digitalWrite(M1_MENUDOSIERPORT_2, LOW);
    //motor2.run(RELEASE);
    Endtime2 = 0;   
  }

  //Dosierpumpe3
  if  (((lTime >= Dosierautomatzeit3_1) && (lTime <= Dosierautomatzeit3_1 + Dosierdauer3) && (Dosierautomatzeit3_1 != 0) && (Dosiernachfuell_3 == false)) ||
       ((lTime >= Dosierautomatzeit3_2) && (lTime <= Dosierautomatzeit3_2 + Dosierdauer3) && (Dosierautomatzeit3_2 != 0) && (Dosiernachfuell_3 == false)) ||
       ((lTime >= Dosierautomatzeit3_3) && (lTime <= Dosierautomatzeit3_3 + Dosierdauer3) && (Dosierautomatzeit3_3 != 0) && (Dosiernachfuell_3 == false)) ||
       ((lTime >= Dosierautomatzeit3_4) && (lTime <= Dosierautomatzeit3_4 + Dosierdauer3) && (Dosierautomatzeit3_4 != 0) && (Dosiernachfuell_3 == false)) ||
       ((lTime >= Dosierautomatzeit3_5) && (lTime <= Dosierautomatzeit3_5 + Dosierdauer3) && (Dosierautomatzeit3_5 != 0) && (Dosiernachfuell_3 == false)) ||
       ((lTime >= Dosierautomatzeit3_6) && (lTime <= Dosierautomatzeit3_6 + Dosierdauer3) && (Dosierautomatzeit3_6 != 0) && (Dosiernachfuell_3 == false)) ||
       ((lTime >= Dosierautomatzeit3_7) && (lTime <= Dosierautomatzeit3_7 + Dosierdauer3) && (Dosierautomatzeit3_7 != 0) && (Dosiernachfuell_3 == false)) ||
       ((lTime >= Dosierautomatzeit3_8) && (lTime <= Dosierautomatzeit3_8 + Dosierdauer3) && (Dosierautomatzeit3_8 != 0) && (Dosiernachfuell_3 == false)) ||
       ((lTime >= Dosierautomatzeit3_9) && (lTime <= Dosierautomatzeit3_9 + Dosierdauer3) && (Dosierautomatzeit3_9 != 0) && (Dosiernachfuell_3 == false)) ||
       ((lTime >= Dosierautomatzeit3_10) && (lTime <= Dosierautomatzeit3_10 + Dosierdauer3) && (Dosierautomatzeit3_10 != 0) && (Dosiernachfuell_3 == false)) || 
       ((lTime >= Dosierautomatzeit3_11) && (lTime <= Dosierautomatzeit3_11 + Dosierdauer3) && (Dosierautomatzeit3_11 != 0) && (Dosiernachfuell_3 == false)) || 
       ((lTime >= Dosierautomatzeit3_12) && (lTime <= Dosierautomatzeit3_12 + Dosierdauer3) && (Dosierautomatzeit3_12 != 0) && (Dosiernachfuell_3 == false)) || 
       ((Endtime3 > 0) && (lTime <= Endtime3)) || 
       (Dosiermanuell_3 == true) ||
       ((liquidSensorValue == 1) && (Dosiernachfuell_3 == true)))
  {        
    digitalWrite(M1_MENUDOSIERPORT_3, HIGH); //255
    //motor3.setSpeed(Dosierspeed_3);
    //motor3.run(FORWARD);
  }
  else
  {
    digitalWrite(M1_MENUDOSIERPORT_3, LOW);
    //motor3.run(RELEASE);
    Endtime3 = 0;   
  }

  //Dosierpumpe4
  if  (((lTime >= Dosierautomatzeit4_1) && (lTime <= Dosierautomatzeit4_1 + Dosierdauer4) && (Dosierautomatzeit4_1 != 0) && (Dosiernachfuell_4 == false)) ||
       ((lTime >= Dosierautomatzeit4_2) && (lTime <= Dosierautomatzeit4_2 + Dosierdauer4) && (Dosierautomatzeit4_2 != 0) && (Dosiernachfuell_4 == false)) ||
       ((lTime >= Dosierautomatzeit4_3) && (lTime <= Dosierautomatzeit4_3 + Dosierdauer4) && (Dosierautomatzeit4_3 != 0) && (Dosiernachfuell_4 == false)) ||
       ((lTime >= Dosierautomatzeit4_4) && (lTime <= Dosierautomatzeit4_4 + Dosierdauer4) && (Dosierautomatzeit4_4 != 0) && (Dosiernachfuell_4 == false)) ||
       ((lTime >= Dosierautomatzeit4_5) && (lTime <= Dosierautomatzeit4_5 + Dosierdauer4) && (Dosierautomatzeit4_5 != 0) && (Dosiernachfuell_4 == false)) ||
       ((lTime >= Dosierautomatzeit4_6) && (lTime <= Dosierautomatzeit4_6 + Dosierdauer4) && (Dosierautomatzeit4_6 != 0) && (Dosiernachfuell_4 == false)) ||
       ((lTime >= Dosierautomatzeit4_7) && (lTime <= Dosierautomatzeit4_7 + Dosierdauer4) && (Dosierautomatzeit4_7 != 0) && (Dosiernachfuell_4 == false)) ||
       ((lTime >= Dosierautomatzeit4_8) && (lTime <= Dosierautomatzeit4_8 + Dosierdauer4) && (Dosierautomatzeit4_8 != 0) && (Dosiernachfuell_4 == false)) ||
       ((lTime >= Dosierautomatzeit4_9) && (lTime <= Dosierautomatzeit4_9 + Dosierdauer4) && (Dosierautomatzeit4_9 != 0) && (Dosiernachfuell_4 == false)) ||
       ((lTime >= Dosierautomatzeit4_10) && (lTime <= Dosierautomatzeit4_10 + Dosierdauer4) && (Dosierautomatzeit4_10 != 0) && (Dosiernachfuell_4 == false)) || 
       ((lTime >= Dosierautomatzeit4_11) && (lTime <= Dosierautomatzeit4_11 + Dosierdauer4) && (Dosierautomatzeit4_11 != 0) && (Dosiernachfuell_4 == false)) || 
       ((lTime >= Dosierautomatzeit4_12) && (lTime <= Dosierautomatzeit4_12 + Dosierdauer4) && (Dosierautomatzeit4_12 != 0) && (Dosiernachfuell_4 == false)) || 
       ((Endtime4 > 0) && (lTime <= Endtime4)) || 
       (Dosiermanuell_4 == true) ||
       ((liquidSensorValue == 1) && (Dosiernachfuell_4 == true)))
  {        
    digitalWrite(M1_MENUDOSIERPORT_4, HIGH); //255
    //motor4.setSpeed(Dosierspeed_4);
    //motor4.run(FORWARD);
  }
  else
  {
    digitalWrite(M1_MENUDOSIERPORT_4, LOW);
    //motor4.run(RELEASE);
    Endtime4 = 0;   
  }
  
  
  //delay(1000);
}
  
  
