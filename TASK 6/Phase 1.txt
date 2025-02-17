#define LED_PIN PC0  // Change this to the correct pin for your board

void setup() {
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_PIN, HIGH);  // Turn LED on
    delay(1000);                   // Wait 500ms
    digitalWrite(LED_PIN, LOW);   // Turn LED off
    delay(1000);                   // Wait 500ms
}
