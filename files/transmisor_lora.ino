#include <SPI.h>
#include <LoRa.h>

// ======================================================
// IDENTIFICACION
// ======================================================

const int MI_ID = 1;
const int DESTINO_ID = 2;

String mensajeOriginal = "SENSOR ACTIVO";

// ======================================================
// PINES
// ======================================================

#define LORA_SCK   18
#define LORA_MISO  19
#define LORA_MOSI  23
#define LORA_NSS   5

#define TOUCH_PIN      27
#define UMBRAL_TOUCH   30   // ajustar según el valor que lea tu ESP32

bool tocando = false;

// ======================================================
// ATBASH
// ======================================================

char convertirAtbash(char letra) {

  if (letra >= 'A' && letra <= 'Z') {
    return 'Z' - (letra - 'A');
  }

  if (letra >= 'a' && letra <= 'z') {
    return 'z' - (letra - 'a');
  }

  return letra;
}

String cifrarAtbash(String mensaje) {

  String mensajeCifrado = "";

  for (int i = 0; i < mensaje.length(); i++) {
    mensajeCifrado += convertirAtbash(mensaje.charAt(i));
  }

  return mensajeCifrado;
}

// ======================================================
// ENVIAR MENSAJE
// ======================================================

void enviarMensaje() {

  String mensajeCifrado = cifrarAtbash(mensajeOriginal);

  Serial.println();
  Serial.println("TOUCH DETECTADO");

  Serial.print("Mensaje original: ");
  Serial.println(mensajeOriginal);

  Serial.print("Mensaje cifrado: ");
  Serial.println(mensajeCifrado);

  // Formato del paquete: ORIGEN|DESTINO|MENSAJE
  LoRa.beginPacket();
  LoRa.print(MI_ID);
  LoRa.print("|");
  LoRa.print(DESTINO_ID);
  LoRa.print("|");
  LoRa.print(mensajeCifrado);
  LoRa.endPacket();

  Serial.print("Paquete enviado: ");
  Serial.print(MI_ID);
  Serial.print("|");
  Serial.print(DESTINO_ID);
  Serial.print("|");
  Serial.println(mensajeCifrado);

  Serial.println("----------------------------");
}

// ======================================================
// SETUP
// ======================================================

void setup() {

  Serial.begin(115200);
  delay(1000);

  Serial.println("TRANSMISOR LORA");

  Serial.print("Mi ID: ");
  Serial.println(MI_ID);

  Serial.print("Destino: ");
  Serial.println(DESTINO_ID);

  // Bus SPI con los pines definidos
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_NSS);

  // setPins(NSS, RESET, DIO0): RESET y DIO0 no se usan
  LoRa.setPins(LORA_NSS, -1, -1);

  if (!LoRa.begin(433E6)) {
    Serial.println("ERROR: No se pudo iniciar LoRa");
    while (true);
  }

  Serial.println("LoRa iniciado correctamente");
  Serial.println("Esperando toque...");
}

// ======================================================
// LOOP
// ======================================================

void loop() {

  int valorTouch = touchRead(TOUCH_PIN);

  // Serial.println(valorTouch);   // descomentar para calibrar el umbral

  if (valorTouch < UMBRAL_TOUCH && !tocando) {
    tocando = true;          // toque nuevo: se envía una sola vez
    enviarMensaje();
  }

  if (valorTouch >= UMBRAL_TOUCH) {
    tocando = false;         // dedo suelto: se permite un nuevo envío
  }

  delay(100);
}
