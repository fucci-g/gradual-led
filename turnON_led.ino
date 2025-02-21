const int buttonPin = 2;  // Pin del pulsante
const int led = 9;     // Pin dell'LED

const int debounceDelay = 20;  // Ritardo di debounce
unsigned long lastDebounceTime = 0;
int reading = 0;
int buttonState = 0;      // Stato attuale del pulsante
int lastButtonState = 0;  // Stato precedente del pulsante
int velocity = 5;        // Velocità di cambiamento della luminosità
int stato = 0;
int LED = 0;
int verso = 0;
int lum = 0;
int maxlum = 200;            // Stato dell'LED
unsigned long intervalTime = 0;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  // Configura il pin del pulsante come input con pull-up interno
  pinMode(led, OUTPUT);           // Configura il pin dell'LED come output
  analogWrite(led, 0); 
  Serial.begin(115200);           // Imposta inizialmente l'LED spento
}

void loop() {
  reading = !digitalRead(buttonPin);  // Leggi lo stato del pulsante
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();  // Se lo stato è cambiato, resetta il timer di debounce
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Se il tempo di debounce è passato, controlla se lo stato del pulsante è cambiato
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == 1) {                             // pullsante abbassato
          intervalTime = millis();
          verso = !verso;
      } else{                                //buttonstate == 0 pulsante alzato
        if (LED == 1){ //da acceso a spento
          if ((millis() - intervalTime) < 500){ 
          turnOFF_LedGrad(led, velocity);
          LED = 0;
          }
        } else {
          turnON_LedGrad(led, velocity);
          LED = 1;
        }
      }  
    }
    if ((millis() - intervalTime) > 500 && LED == 1){
      if (verso == 0){
        turnOFFGradual_LedGrad(led, velocity);  
      } else {
        turnONGradual_LedGrad(led, velocity);
      }
    }
    Serial.println(verso);
  }

  lastButtonState = reading;  // Aggiorna lo stato precedente del pulsante
}

// Funzione per accendere gradualmente l'LED fino alla luminosità desiderata
void turnON_LedGrad(int ledPin, int velocity) {
  lum = 0;
  unsigned long previousMillis = millis();
  
  while (lum < maxlum) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= velocity) {
      previousMillis = currentMillis;  // Aggiorna il tempo precedente
      lum++;
      analogWrite(ledPin, lum);
    }
  }
}

// Funzione per spegnere gradualmente l'LED
void turnOFF_LedGrad(int ledPin, int velocity) {
  unsigned long previousMillis = millis();
  
  while (lum > 0) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= velocity) {
      previousMillis = currentMillis;  // Aggiorna il tempo precedente
      lum--;
      analogWrite(ledPin, lum);
    }
  }
}

// Funzione per spegnere gradualmente l'LED
void turnOFFGradual_LedGrad(int ledPin, int velocity) {
  unsigned long previousMillis = millis();
  
  while (reading == 1) {
    if (lum > 20){
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= velocity) {
        previousMillis = currentMillis;  // Aggiorna il tempo precedente
        lum--;
        analogWrite(ledPin, lum);
      }
    }
    reading = !digitalRead(buttonPin);
  }
}

void turnONGradual_LedGrad(int ledPin, int velocity) {
  unsigned long previousMillis = millis();
  
  while (reading == 1) {
    if (lum < 200){
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= velocity) {
        previousMillis = currentMillis;  // Aggiorna il tempo precedente
        lum++;
        analogWrite(ledPin, lum);
      }
    }
    reading = !digitalRead(buttonPin);
  }
}