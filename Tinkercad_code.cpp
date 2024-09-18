#include <Adafruit_LiquidCrystal.h>
Adafruit_LiquidCrystal lcd1(0);
int prevVal = 0;
int seg = 0;
int seg1 = 0;
int diff = 0;
bool inicio1 = true;
bool inicio2 = false;
bool seno = false;
bool cuadrada = false;
bool triangulada = false;
const int analogPin = 0;
const int botonInicioPin = 2;
const int botonMostrarPin = 4;
float voltage = 0;
float maxVoltage = 0;
float minVoltage = 5.0;
float Amplitud = 0;
float Hertz = 0;

void setup()
{
  while (!Serial)
  { 
    ; // Espera a que se inicie el puerto serie
  }
  Serial.begin(14400);
  lcd1.begin(16, 2);
  lcd1.setCursor(0, 0);
  pinMode(botonInicioPin, INPUT_PULLUP);
  pinMode(botonMostrarPin, INPUT_PULLUP);
}

void loop()
{
  while (inicio1)
  {
  	lcd1.print("Esperando...");
    delay(10);
    lcd1.clear();
    if(digitalRead(botonInicioPin) == HIGH)
    {
      delay(10);
      inicio1 = false;
      lcd1.clear();
      lcd1.print("Leyendo onda...");
    }
  }
  while(!inicio2)
  {
    voltage = analogRead(analogPin) * (10.0 / 1023.0);
    Serial.println(voltage);
    diff = abs(voltage - prevVal);
    if (voltage > maxVoltage)
    {
      maxVoltage = voltage;
    }
    if (voltage == maxVoltage)
    {
      seg = millis();
      Hertz = 1000.0 / (seg - seg1);
      seg1 = seg;
    }
    if (voltage < minVoltage)
    {
      minVoltage = voltage;
    }
    Amplitud = (maxVoltage - minVoltage)/2.0;
    if (voltage == maxVoltage || voltage == minVoltage)
    {  // Siempre tiene dos unicos valores
      cuadrada = true;
    } else if (diff < 0.1)
    {  // Cambio suave
      seno = true;
    } else if(diff == 0)
    {  // Cambio más constante (ni brusco ni suave)
      triangulada = true;
    }

    prevVal = voltage;
    if(digitalRead(botonMostrarPin) == HIGH)
    {
      inicio2 = true;
    }
  }
  if (inicio2)
  {
    lcd1.clear();
    if (cuadrada && !seno && !triangulada)
    {
      lcd1.print("Onda cuadrada");
      lcd1.setCursor(0, 1);
      lcd1.print(Amplitud);
      lcd1.print("V, ");
      lcd1.print(Hertz);
      lcd1.print("Hz");
    }
    else if (!cuadrada && seno && !triangulada)
    {
      lcd1.print("Onda senoidal. ");
      lcd1.setCursor(0, 1);
      lcd1.print(Amplitud);
      lcd1.print("V, ");
      lcd1.print(Hertz);
      lcd1.print("Hz");
    }
    else if (!cuadrada && !seno && triangulada)
    {
      lcd1.print("Onda Triangulada. ");
      lcd1.setCursor(0, 1);
      lcd1.print(Amplitud);
      lcd1.print("V, ");
      lcd1.print(Hertz);
      lcd1.print("Hz");
    }
    if(!cuadrada && !seno && !triangulada)
    {
      lcd1.print("No se pudo");
      lcd1.print("determinar");
    }
    delay(1500); //aumentar el tiempo para mostrar todos los datos y dar tiempo a leer
    lcd1.clear();
    lcd1.print("Leyendo onda...");
    inicio2 = false;
    seno = false;
    cuadrada = false;
    triangulada = false;
    maxVoltage = 0;
    minVoltage = 5.0;
    Amplitud = 0;
    prevVal = 0;
  }
}
