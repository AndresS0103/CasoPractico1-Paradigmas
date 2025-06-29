/********************************************
* Universidad Fidélitas                     *
* Curso: Paradigmas de la programación      *
* Autor: GRUPO 3                            *
* Archivo: casoPractico.ino              *
* Fecha: 27-6-2025                          *               
********************************************/

/*******************************************
*Carga de las bibliotecas del sistema      *
********************************************/

/********************************************
*Definicion de las constantes del sistema   *
********************************************/
#define BTN_editarAR A1
#define BTN_SUBIR   A2
#define BTN_BAJAR A3
#define BTN_SELECCIONAR A7

/*****************************************************
*Definicion de las variables y objetos del sistema   *
*****************************************************/
int SEC_U[4] = {53, 52, 51, 50};
int SEC_D[4] = {49, 48, 47, 46};
int MIN_U[4] = {45, 44, 43, 42};
int MIN_D[4] = {41, 40, 39, 38};
int HRS_U[4] = {37, 36, 35, 34};
int HRS_D[4] = {33, 32, 31, 30};

int hora = 12, minuto = 34, segundo = 50;
bool editar = 0, selMin = 0, parpadeo = 1;
long t0, tParpadeo;

void displayBCD(int digito, int pin[4]) { for (int i = 0; i < 4; i++) digitalWrite(pin[i], (digito >> i) & 1); }

void refrescar() {
  displayBCD(segundo % 10, SEC_U); displayBCD(segundo / 10, SEC_D);
  int mostrarMin = !(editar &&  selMin && !parpadeo);
  int mostrarHrs = !(editar && !selMin && !parpadeo);
  int mU = mostrarMin ? minuto % 10 : 15, mD = mostrarMin ? minuto / 10 : 15;
  int hU = mostrarHrs ? hora % 10 : 15, hD = mostrarHrs ? hora / 10 : 15;
  displayBCD(mU, MIN_U); displayBCD(mD, MIN_D);
  displayBCD(hU, HRS_U); displayBCD(hD, HRS_D);
}//fin refrescar

bool presionado(int pin) { return !digitalRead(pin); }

void setup() {
  int* grupoPines[] = {SEC_U, SEC_D, MIN_U, MIN_D, HRS_U, HRS_D};
  for (int k = 0; k < 6; k++) for (int i = 0; i < 4; i++) pinMode(grupoPines[k][i], OUTPUT);
  pinMode(BTN_editarAR, INPUT_PULLUP); pinMode(BTN_SUBIR, INPUT_PULLUP);
  pinMode(BTN_BAJAR, INPUT_PULLUP); pinMode(BTN_SELECCIONAR, INPUT_PULLUP);
  t0 = millis(); tParpadeo = t0; refrescar();
}//fin del setup

void loop() {
  static bool presionadoEditar = 0, presionadoSubir = 0, presionadoBajar = 0, presionadoSeleccionar = 0;

  if (presionado(BTN_editarAR) && !presionadoEditar) { editar = !editar; selMin = 0; segundo = 0; parpadeo = 1; delay(180); }
  if (presionado(BTN_SELECCIONAR) && !presionadoSeleccionar && editar) { selMin = !selMin; delay(180); }
  if (presionado(BTN_SUBIR) && !presionadoSubir && editar) {
    if (selMin) minuto = (minuto + 1) % 60; else hora = (hora + 1) % 24; delay(180);
  }
  if (presionado(BTN_BAJAR) && !presionadoBajar && editar) {
    if (selMin) minuto = (minuto + 59) % 60; else hora = (hora + 23) % 24; delay(180);
  }

  presionadoEditar = presionado(BTN_editarAR); presionadoSubir = presionado(BTN_SUBIR);
  presionadoBajar = presionado(BTN_BAJAR); presionadoSeleccionar = presionado(BTN_SELECCIONAR);

  long ahora = millis();

  if (!editar && ahora - t0 >= 500) {
    t0 += 500;
    if (++segundo == 60) { segundo = 0; if (++minuto == 60) { minuto = 0; if (++hora == 24) hora = 0; } }
  }

  if (editar && ahora - tParpadeo >= 500) { tParpadeo += 500; parpadeo = !parpadeo; }

  refrescar();
}//fin del loop


