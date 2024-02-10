const int tempPin = A0; 
const int SoilMoisturePin = A1;
const int Blueled = 2;    
const int Yellowled = 3;
const int Greenled = 4;
const int motionPin = 5;   

const int soilMoistureThreshold = 500;  // Adjust the threshold as needed
const int timerInterruptInterval = 1000;  // Timer interrupt interval in milliseconds (adjust as needed)
unsigned long previousMillis = 0;

int tempValue = 0;
int SoilMoistureValue = 0;

void setup() {
  Serial.begin(9600);
  pinMode(Blueled, OUTPUT);
  pinMode(Yellowled, OUTPUT);
  pinMode(motionPin, INPUT);
  pinMode(Greenled, OUTPUT);

attachInterrupt(tempPin, temperatureChangeISR, CHANGE);
attachInterrupt(motionPin, motionISR, CHANGE);
  
  TCCR1A = 0;
  TCCR1B = 0;
  OCR1A = 15624;  
  TCCR1B |= (1 << WGM12);  
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  TIMSK1 |= (1 << OCIE1A);  
}

void loop()
{
  temperatureChangeISR();
  motionISR();
}

void temperatureChangeISR() 
{
  tempValue = analogRead(tempPin);
  float adjustedTemp = tempValue / 6.12;
  Serial.print("Temperature: ");
  Serial.print(adjustedTemp);
  delay(500);

  if (adjustedTemp > 25) 
  {
    digitalWrite(Blueled, HIGH);
    Serial.println(" LED ON (Interrupted)");
  } 
  else 
  {
    digitalWrite(Blueled, LOW);
    Serial.println(" Not Interrupted");
  }
}

void motionISR() 
{
  int motionDetected = digitalRead(motionPin);

  if (motionDetected == HIGH) 
  {
    digitalWrite(Yellowled, HIGH);
    Serial.println("Motion detected! (Interrupted)");
  } 
  else 
  {
    digitalWrite(Yellowled, LOW);
    delay(500);
  }
}

ISR(TIMER1_COMPA_vect) 
{
  // Reading soil moisture sensor
  SoilMoistureValue = analogRead(SoilMoisturePin);
  Serial.print("Soil Moisture: ");
  Serial.print(SoilMoistureValue);
  if (SoilMoistureValue < soilMoistureThreshold) 
  {
    digitalWrite(Greenled, HIGH);
    Serial.println("");
    Serial.println("Warning: Soil is too dry! (Interrupted)");
  } 
  else 
  {
    digitalWrite(Greenled, LOW);
  }
}