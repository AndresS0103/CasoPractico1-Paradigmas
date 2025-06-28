/********************************************
* Universidad Fidélitas                     *
* Curso: Paradigmas de la programación      *
* Autor: GRUPO 3                            *
* Archivo: casoPractico.ino                 *
* Fecha: 27-6-2025                          *               
********************************************/

/*******************************************
*Carga de las bibliotecas del sistema      *
********************************************/

/********************************************  
*Definicion de las constantes del sistema   *
********************************************/

/*****************************************************
*Definicion de las variables y objetos del sistema   *
*****************************************************/
int SEC_U[4] = {53, 52, 51, 50};
int SEC_D[4] = {49, 48, 47, 46};

int MIN_U[4] = {45, 44, 43, 42};
int MIN_D[4] = {41, 40, 39, 38};

int HRS_U[4] = {37, 36, 35, 34};
int HRS_D[4] = {33, 32, 31, 30};

int hours   = 12;
int minutes = 34;
int seconds = 50;

/*****************************************************
*Definicion de funciones y metodos del sistema        *
*****************************************************/
void displayBCD(int decena, int pin[4]) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(pin[i], (decena >> i) & 1);
  }
}

void refrescar() {
  displayBCD(seconds % 10, SEC_U);
  displayBCD(seconds / 10, SEC_D);
  displayBCD(minutes % 10, MIN_U);
  displayBCD(minutes / 10, MIN_D);
  displayBCD(hours   % 10, HRS_U);
  displayBCD(hours   / 10, HRS_D);
}

//equivalente al BIOS del pc
void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(SEC_U[i], OUTPUT);
    pinMode(SEC_D[i], OUTPUT);
    pinMode(MIN_U[i], OUTPUT);
    pinMode(MIN_D[i], OUTPUT);
    pinMode(HRS_U[i], OUTPUT);
    pinMode(HRS_D[i], OUTPUT);
  }
  refrescar();
} // Final del setup

//Equivalente al SO
void loop() {
  delay(250);
  seconds++;
  if (seconds == 60) { seconds = 0; minutes++; }
  if (minutes == 60) { minutes = 0; hours++; }
  if (hours == 24) hours = 0;
  refrescar();
}//Final del loop



