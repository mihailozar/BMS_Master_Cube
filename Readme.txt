Opis:
-------------------------------------------------------------------------------------------------------------------------

Implementirani softver je pisan za STM32L476RG plocu koja se zove BMS Master Board.
Funkcionalnosti ove ploce su sledece:
    1. Komunikacija sa BMS Slave plocama
    2. Citanje napona, temepratura i gresaka sa BMS Slave plocama
    3. Pojedinacno balansiranje svake celije
    4. Printanje Human Readable formata podataka sa BMS Slaveova
    5. Printanje PC GUI BMS Dijagnostike formata o relevantim podacima sa BMSa
    6. Slanje podataka o BMS Slaveovima (napon/temeprature i greske) putem CANa
    7. Slanje poruke GENERAL_BMS_MASTER_STATE putem CANa
    8. Aktivacija precharge sekvence na zahtev ECUa po CANu
    9. Provera statusa relea koji su deo PreCharge sekvence i dijagnostika gresaka po CANu
    10. Aktivacija ShutDown kola, tj. otvaranje SD kola na zahtev ECUa ili usled greske u radu BMS Mastera
    11. Ocitavnje duty cyclea IMD Status-a radi dijagnostikovanja greske i slanje po CANu
    12. Ocitavanje i slanje po CANu statusa TSAL logike
    
TO DO:
    13. Registracija punjenja i komunikacija sa Punjacem
    14. Detekcija Alive Poruka: Poruka od ECUa i LVPBa


Kako se konfigurise i upravlja delovima koda?
------------------------------------------------------------------------------------------------------------------------
*   U okviru slaveConfig.h fajla konfigurisu se svi relevantni detalji za BMS Master funkcionalnost.
    U komentarima svakog parametra oznacena je dinica u kojoj se on zadaje!

TO DO:
    Jos nije implementirana funkcija za setovanje Under i Over Voltage-a iz Config file-a.

*   U okviru fajla enabledSubsystems.h nalaze se makroi za pustanje i printanje odredjenih delova koda:
    1. GUI_COMMUNICATION - omoguci slanje na Serijski port u GUI Redable formatu za Custom Made PC dijagnostiku
    2. PRINT_RELAY_STATE - Serijski print stanja: SD, AirPlus, AirMinus i Precharge relay STATE
    3. SERIAL_HUMAN_READABLE_PRINT - printanje Napona/Temepratura i Gresaka u Human Readable formatu 
    4. CAN_TEMEPERATURE_SERIAL_PRINT - sluzi da u Human Radable formatu printuje temerature koje se salju po CANu


Napomene:

------------------------------------------------------------------------------------------------------------------------

SHD_Status:

1 - SHD open, otvoreno ShutDown kolo, detektovana greska
0 - SHD close, zatvoreno ShutDown kolo, sve OK

BMS_SHD_Control:

openSHD()  - posalje logicku 0
closeSHD() - posalje logicku 1

------------------------------------------------------------------------------------
Kako uspavati sve Slave ploce?

Potrebno je na Serijskom monitoru sa baudom 9600, poslati komadu: 1,0
Sacekjate dok se ploce ne uspavaju, pri poslatoj komadi videce se poruka na monitoru 
da su Ploce uspesno uspavane!
______________________________________________________________________________________


Djale pitanja:
1. Sta se desi ako imam lecovan zahtev za precharge? Tj stigao mi zahtev po CANu
a on ne moze to da ga obradi jer je SHD open. Zatvorim SHD i on krene u precharge
sekvencu? da li je ovo oke?


TO DO:
1. Ubaci PWM u CAN
2. Drugacije izracunaj totalVoltage