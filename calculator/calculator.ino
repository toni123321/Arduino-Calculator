
// -----------------------  Calculator   -----------------------

#include "Display.h"

// variables for Led pins
int redLedPin = 4;
int greenLedPin = 5;
int blueLedPin = 6;
int yellowLedPin = 7;

// variables for button pins
int btnLeftPin = 9;
int btnRightPin = 8;
int potPin = A0;

int mode = 0; // variable for indicating in which state the program is
int first_time = 1; // variable for checking if it's the first time the program is runned

// variables for button states
int btnLeftState = 0;
int btnRightState = 0;

int potValue = 0; // variable needed for getting the result from the potentiometer

int isBtnPushed = LOW;
int previousBtnState = HIGH;

int input1 = 0; // variable for first inputed value by the user
int input2 = 0; // variable for second inputed value by the user


char operatorName; // variable for naming all 4 operators, used in this program (+, -, /, *) in order to present every operator on the Display
float result = 0; // variable needed to store the result of making some maths' operations with input1 and input2

// Function purpose: to switch on for 2 sec all the leds and then to switch them off in order to notify the user that the calculator has just started
void startLeds()
{
  digitalWrite(redLedPin, HIGH);
  digitalWrite(greenLedPin, HIGH);
  digitalWrite(blueLedPin, HIGH);
  digitalWrite(yellowLedPin, HIGH);
  delay(2000);
  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, LOW);
  digitalWrite(blueLedPin, LOW);
  digitalWrite(yellowLedPin, LOW);
}

// Function purpose: Made to get the number which the knob returns and to translate it to 
// character which reffered to one of the four operators used in this program (+,-,/,*)
char getFunction(int n)
{
  if (n == 0)
  {
    return 'a';
  }
  else if (n == 1)
  {
    return 's';
  }
  else if (n == 2)
  {
    return 't';
  }
  else if (n == 3)
  {
    return 'd';
  }
}

// Function purpose: Get the operator name and with it makes the current math operation - addition, substraction, division, multiplication(times)
void calculate(char operatorName)
{
  if (operatorName == 'a')
  {
    result = input1 + input2;
  }
  else if (operatorName == 's')
  {
    result = input1 - input2;
  }
  else if (operatorName == 't')
  {
    result = input1 * input2;
  }

  else if (operatorName == 'd')
  {
    if (input2 != 0)
    {
      // cast input2 in order to receive precise result, but not only zeros after the point -> 4.55, but not 4.00
      result = input1 / float(input2); 
    }
  }
}

void setup() {
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);

  pinMode(btnLeftPin, INPUT_PULLUP);
  pinMode(btnRightPin, INPUT_PULLUP);
  pinMode(potPin, INPUT);
  Serial.begin(9600);

  Serial.println("----"); // notify the user that the device is on
}

void loop() {
  btnLeftState =  digitalRead(btnLeftPin); //read the button state

  if (previousBtnState != btnLeftState)
  {
    if (btnLeftState == isBtnPushed)
    {
      mode++;
      if (mode > 4)
      {
        mode = 1;
      }

    }
    previousBtnState = btnLeftState;
  }

  if (mode == 1)
  {
    if (first_time == 1)
    {
      startLeds();
      first_time = 0;
    }
    
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, LOW);
    digitalWrite(blueLedPin, HIGH);
    Serial.println("Please input the first input value: ");
    potValue = analogRead(potPin);
    input1 = map(potValue, 0, 1023, -10, 10);
    Display.show(input1);
  }

  else if (mode == 2)
  {
    Serial.println("Please input the input operator: ");
    potValue = analogRead(potPin);
    int operatorValue = map(potValue, 0, 1023, 0, 4);
    operatorName = getFunction(operatorValue);
    if (operatorName == 'a')
    {
      Display.show("-a--"); // addition
    }
    else if (operatorName == 's')
    {
      Display.show("-s--"); //substraction
    }
    else if (operatorName == 'd')
    {
      Display.show("-d--"); // division
    }
    else if (operatorName == 't')
    {
      Display.show("-t--"); //multiplication
    }
  }
  else if (mode == 3)
  {
    Serial.println("Please input the second input value: ");
    potValue = analogRead(potPin);
    input2 = map(potValue, 0, 1023, -10, 10);
    Display.show(input2);
  }
  else if (mode == 4)
  {
    Serial.print("First input: "); // print the first input value
    Serial.println(input1);
    
    Serial.print("Second input: "); // print the second input value
    Serial.println(input2);
    
    digitalWrite(blueLedPin, LOW);
    Serial.println("This is your result: "); // print the result
    
    calculate(operatorName); // calculate the result having the name of the operator the user has inputed by the knob
    
    if(result < -99.9 or result > 999.9) // make sure numbers which exceeded four digit limit not to be shown on the display
    {
        Serial.println("Result: ");
        Serial.println("--");
        Display.show("--");
        digitalWrite(yellowLedPin, HIGH);
    }
    else
    {
      if (input2 == 0 && operatorName == 'd') // error handling for division by zero which is not allowed
      {
        Display.show("Err");
        digitalWrite(redLedPin, HIGH);
      }
      else{
        Serial.print("Result: ");
        Serial.println(result);  
        Display.show(result);

        digitalWrite(greenLedPin, HIGH);
      }
    }
  }
}
