#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

Adafruit_SH1106G display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

const int TOUCH_PIN_1 = 4;  // GPIO4  = T0 -> Botón 1
const int TOUCH_PIN_2 = 15; // GPIO15 = T3 -> Botón 2

// Ajusta este valor con el umbral que obtuviste en la Parte 1
// (idealmente mide el umbral por separado para cada pin)
const int UMBRAL_TOUCH = 600;

void setup() {

  Serial.begin(115200);

  Wire.begin(21, 22);

  if (!display.begin(OLED_ADDRESS, true)) {
    Serial.println("Error al iniciar la pantalla OLED");
    while (1);
  }

  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
}

void loop() {

  int valorTouch1 = touchRead(TOUCH_PIN_1);
  int valorTouch2 = touchRead(TOUCH_PIN_2);

  bool tocado1 = valorTouch1 < UMBRAL_TOUCH;
  bool tocado2 = valorTouch2 < UMBRAL_TOUCH;

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(26, 5);
  display.println("ESP32 TOUCH");

  if (tocado1) {

    display.setCursor(8, 28);
    display.println("Boton 1 activado");

    display.fillCircle(20, 52, 5, SH110X_WHITE);
  }

  else if (tocado2) {

    display.setCursor(8, 28);
    display.println("Boton 2 activado");

    display.fillCircle(105, 52, 5, SH110X_WHITE);
  }

  else {

    display.setCursor(32, 28);
    display.println("Esperando...");
  }

  display.display();

  delay(100);
}
