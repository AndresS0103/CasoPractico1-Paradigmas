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
#define BTN_ALARMA A0      
#define BTN_editarAR A1
#define BTN_SUBIR   A2
#define BTN_BAJAR A3
#define BTN_SELECCIONAR A7
#define BTN_TONO A6

int leds[] = {22, 23, 24, 25, 26, 27}; 
int tonoSeleccionado = 0;   
bool modoSeleccionTono = false;
bool alarmaActiva = false;  
long tAlarmaInicio = 0;

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
//variables relog
int alarmaHora = 0, alarmaMinuto = 0;  
bool editar = 0, selMin = 0, parpadeo = 1;
//variables alarma
bool modoAlarma = 0;                   
bool editandoAlarma = 0, selMinAlarma = 0, parpadeoAlarma = 1;
long t0, tParpadeo, tIndicador;

/*****************************************************
*Definicion de funciones y metodos del sistema        *
*****************************************************/

void displayBCD(int digito, int pin[4]) { 
  for (int i = 0; i < 4; i++) 
    digitalWrite(pin[i], (digito >> i) & 1); 
}

void refrescar() {
   // Modo alarma
  if (modoAlarma) {
    displayBCD(15, SEC_U);                   
    displayBCD(15, SEC_D);                   
    
    int mostrarMin = !(editandoAlarma &&  selMinAlarma && !parpadeoAlarma);
    int mostrarHrs = !(editandoAlarma && !selMinAlarma && !parpadeoAlarma);
    
    int mU = mostrarMin ? alarmaMinuto % 10 : 15;
    int mD = mostrarMin ? alarmaMinuto / 10 : 15;
    int hU = mostrarHrs ? alarmaHora % 10 : 15;
    int hD = mostrarHrs ? alarmaHora / 10 : 15;
    
    displayBCD(mU, MIN_U);
    displayBCD(mD, MIN_D);
    displayBCD(hU, HRS_U);
    displayBCD(hD, HRS_D);
  } else {
    // Modo reloj 
    displayBCD(segundo % 10, SEC_U);
    displayBCD(segundo / 10, SEC_D);
    
    int mostrarMin = !(editar &&  selMin && !parpadeo);
    int mostrarHrs = !(editar && !selMin && !parpadeo);
    
    int mU = mostrarMin ? minuto % 10 : 15;
    int mD = mostrarMin ? minuto / 10 : 15;
    int hU = mostrarHrs ? hora % 10 : 15;
    int hD = mostrarHrs ? hora / 10 : 15;
    
    displayBCD(mU, MIN_U);
    displayBCD(mD, MIN_D);
    displayBCD(hU, HRS_U);
    displayBCD(hD, HRS_D);
  }
}

bool presionado(int pin) { return !digitalRead(pin); }

void setup() {
  int* grupoPines[] = {SEC_U, SEC_D, MIN_U, MIN_D, HRS_U, HRS_D};
  for (int k = 0; k < 6; k++) 
    for (int i = 0; i < 4; i++) 
      pinMode(grupoPines[k][i], OUTPUT);
      
  pinMode(BTN_ALARMA, INPUT_PULLUP);         
  pinMode(BTN_editarAR, INPUT_PULLUP);
  pinMode(BTN_SUBIR, INPUT_PULLUP);
  pinMode(BTN_BAJAR, INPUT_PULLUP);
  pinMode(BTN_SELECCIONAR, INPUT_PULLUP);
  pinMode(BTN_TONO, INPUT_PULLUP);
  for (int i = 0; i < 6; i++) pinMode(leds[i], OUTPUT);
  
  delay(1000);

  t0 = millis(); 
  tParpadeo = t0;
  tIndicador = t0;
  refrescar();
}

void loop() {
  static bool presionadoEditar = true;
  static bool presionadoSubir = true;
  static bool presionadoBajar = true;
  static bool presionadoSeleccionar = true;
  static bool presionadoAlarma = true;
  static bool presionadoTono = true;
  static bool presionadoIzq = true;
  static bool presionadoDer = true;
  static bool presionadoCentro = true;

if (presionado(BTN_TONO) && !presionadoTono) {
  if (modoSeleccionTono) {
    // confirmar el led para el tono
    modoSeleccionTono = false;
    for (int i = 0; i < 6; i++) {
      digitalWrite(leds[i], LOW);
    }
  } else {
    // se entra en el modo para escoger el tono,si no se esta en modo de elegir
    modoSeleccionTono = true;
  }
  delay(180);
}
presionadoTono = presionado(BTN_TONO);

if (modoSeleccionTono) {
  // se apagan los leds y enciende el seleccionado
  for (int i = 0; i < 6; i++) {
    digitalWrite(leds[i], LOW);
  }
  digitalWrite(leds[tonoSeleccionado], HIGH);

  // mover a la derecha los leds
  if (presionado(BTN_SELECCIONAR) && !presionadoDer) {
    tonoSeleccionado = (tonoSeleccionado + 1) % 6; 
    delay(180);
  }
  presionadoDer = presionado(BTN_SELECCIONAR);
}
// Verifica coincidencia solo cuando no esté editando
if (!modoAlarma && !editar) {
  if (hora == alarmaHora && minuto == alarmaMinuto && segundo == 0) {
    alarmaActiva = true;
    tAlarmaInicio = millis();
  }
}

// Si la alarma está activa, parpadea el LED elegido
if (alarmaActiva) {
  long ahora = millis();
  if (ahora - tAlarmaInicio <= 60000) {
    // Parpadeo cada 500 ms
    if ((ahora / 500) % 2 == 0) {
      digitalWrite(leds[tonoSeleccionado], HIGH);
    } else {
      digitalWrite(leds[tonoSeleccionado], LOW);
    }
  } else {
    alarmaActiva = false;
    digitalWrite(leds[tonoSeleccionado], LOW);
  }
}

  
  // Manejo del botón de alarma
  if (presionado(BTN_ALARMA) && !presionadoAlarma) {
    modoAlarma = !modoAlarma;
    editandoAlarma = false;      
    delay(180);
  }
  presionadoAlarma = presionado(BTN_ALARMA);

  if (modoAlarma) {
    // Modo alarma edicion
    if (presionado(BTN_editarAR) && !presionadoEditar) {
      editandoAlarma = !editandoAlarma;
      parpadeoAlarma = 1;
      delay(180);
    }
    if (presionado(BTN_SELECCIONAR) && !presionadoSeleccionar && editandoAlarma) {
      selMinAlarma = !selMinAlarma;
      delay(180);
    }
    if (presionado(BTN_SUBIR) && !presionadoSubir && editandoAlarma) {
      if (selMinAlarma) alarmaMinuto = (alarmaMinuto + 1) % 60;
      else alarmaHora = (alarmaHora + 1) % 24;
      delay(180);
    }
    if (presionado(BTN_BAJAR) && !presionadoBajar && editandoAlarma) {
      if (selMinAlarma) alarmaMinuto = (alarmaMinuto + 59) % 60;
      else alarmaHora = (alarmaHora + 23) % 24;
      delay(180);
    }
  } else {
    // Modo reloj edicion
    if (presionado(BTN_editarAR) && !presionadoEditar) {
      editar = !editar;
      selMin = 0;
      segundo = 0;
      parpadeo = 1;
      delay(180);
    }
    if (presionado(BTN_SELECCIONAR) && !presionadoSeleccionar && editar) {
      selMin = !selMin;
      delay(180);
    }
    if (presionado(BTN_SUBIR) && !presionadoSubir && editar) {
      if (selMin) minuto = (minuto + 1) % 60;
      else hora = (hora + 1) % 24;
      delay(180);
    }
    if (presionado(BTN_BAJAR) && !presionadoBajar && editar) {
      if (selMin) minuto = (minuto + 59) % 60;
      else hora = (hora + 23) % 24;
      delay(180);
    }
  }

  // Actualizar estados de botones
  presionadoEditar = presionado(BTN_editarAR);
  presionadoSubir = presionado(BTN_SUBIR);
  presionadoBajar = presionado(BTN_BAJAR);
  presionadoSeleccionar = presionado(BTN_SELECCIONAR);

  long ahora = millis();

  // Manejo del tiempo en modo reloj
  if (!modoAlarma && !editar && ahora - t0 >= 1000) {
    t0 += 1000;
    if (++segundo == 60) {
      segundo = 0;
      if (++minuto == 60) {
        minuto = 0;
        if (++hora == 24) hora = 0;
      }
    }
  }

  // Parpadeo en edición
  if (modoAlarma) {
    if (editandoAlarma && ahora - tParpadeo >= 500) {
      tParpadeo += 500;
      parpadeoAlarma = !parpadeoAlarma;
    }
  } else {
    if (editar && ahora - tParpadeo >= 500) {
      tParpadeo += 500;
      parpadeo = !parpadeo;
    }
  }

  refrescar();
}

