#pragma config(Sensor, S1,     LightLeft,      sensorEV3_Color)
#pragma config(Sensor, S2,     Touch,          sensorEV3_Touch)
#pragma config(Sensor, S3,     UltraSonic,     sensorEV3_Ultrasonic)
#pragma config(Sensor, S4,     LightRight,     sensorEV3_Color)
#pragma config(Motor,  motorA,          Left,          tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD,          Right,         tmotorEV3_Large, PIDControl, encoder)

bool chemicalSpillMode = false;				//Whether or not the robot is in ChemSpill mode

// Motor Speed Values
int motorSpeed = 30;									//Adjustable, preferably above 20 and below 60
int reverseSpeed = -5; 								//This is only used in turning.

// Colour Values
int colourBlackMin = 0;								//Lowest possible value, keep on zero (Darkest / Black)
int colourBlackMax = 15;							//This value changes in calibration
int colourGreenMin = 13;							//Minimum green colour. Changes in calibration
int colourGreenMax = 20;							//Maximum green colour. Changes in calibration
int colourWhiteMin = 40;							//Minimum white colour. Changes in calibration
int colourWhiteMax = 95;							//Maximum white colour. Changes in calibration
int colourFoilMin = 95;								//Doesn't need to be calibrated.
string color;													//For colour calibration

// Light sensor values
int colourSensorLeft;									//Constantly updates. Holds current S1 value (0-100)
int colourSensorRight;								//Constantly updates. Holds current S2 value (0-100)
int touchSensorValue;									//Is the touch sensor pressed?
int UltraSonicValue;

//	 		END SETUP			//
//	 BEGIN MAIN LOOP	//

int CalibrateLightSensor(string color)			// Function for calibrating light sensors
{
	eraseDisplay();														// Clear Display
	while(SensorValue(Touch) != 1) {					// While the touch sensor hasn't been pressed
		displayTextLine(1, "Color: %s", color);	// Print the colour being calibrated
		displayTextLine(2, "Reading: %4d", SensorValue(LightLeft));	// Print current value
		displayTextLine(3, "Press Enter...");   // Print confirmation dialog thing
		wait1Msec(100);													// Refresh every 100ms
	}
	wait1Msec(500);														// Wait for half a second
	playSound(soundBlip);											// Play blip sound

	return SensorValue(LightLeft);						// Resturn respective value
}


task TurnAround() {
	motor[Right] = motorSpeed;
	motor[Left] = -motorSpeed;
	delay(500);
}
void TurnLeftGreen() {
	motor[Right] = motorSpeed;
	motor[Left] = motorSpeed;
	delay(150);
	motor[Left] = motorSpeed;
	motor[Right] = reverseSpeed;
	delay(400);
	//Turn Left on Green
}

void TurnRightGreen() {
	motor[Right] = motorSpeed;
	motor[Left] = motorSpeed;
	delay(150);
	motor[Right] = motorSpeed;
	motor[Left] = reverseSpeed;
	delay(400);
	//Turn Right on Green. This will be pre-programmed.
}

void navigationMode() {
	displayTextLine(9, "Navigation Mode");
	colourSensorLeft = SensorValue(LightLeft);
	colourSensorRight = SensorValue(LightRight);
	UltraSonicValue = SensorValue(UltraSonic);
	
	if (UltraSonicValue < 20 && UltraSonicValue > 5) {
		motor[Left] = -motorSpeed;
		motor[Right] = -motorSpeed;
	}
	if (colourSensorRight > colourGreenMin && colourSensorRight < colourGreenMax) {
		//TurnRightGreen();
	}
	else if (colourSensorLeft > colourGreenMin && colourSensorLeft < colourGreenMax) {
		//TurnLeftGreen();
	}
	else if (colourSensorLeft > colourBlackMin && colourSensorLeft < colourBlackMax) {
		motor[Left] = motorSpeed;
		motor[Right] = reverseSpeed;
		//Turn Left on Black
	}
	else if (colourSensorRight > colourBlackMin && colourSensorRight < colourBlackMax) {
		motor[Left] = reverseSpeed;
		motor[Right] = motorSpeed;
		//Turn Right on Black
	}
	else if (colourSensorRight > colourFoilMin && colourSensorLeft > colourFoilMin) {
		chemicalSpillMode = true;
		//Enter chemical spill mode
	}
	else if (colourSensorRight> colourWhiteMin && colourSensorLeft > colourWhiteMin && colourSensorLeft < colourWhiteMax && colourSensorRight > colourWhiteMax) {
		motor[Left] = motorSpeed;
		motor[Right] = motorSpeed;
		//Go Forward on White
	}
}
void chemSpillMode() {
	displayTextLine(9, "Chemical Spill Mode");
	UltraSonicValue = SensorValue(UltraSonic);
	colourSensorLeft = SensorValue(LightLeft);
	colourSensorRight = SensorValue(LightRight);
	/*if (UltraSonicValue < 10) {
		motor[Left] = motorSpeed;
		motor[Right] = motorSpeed;
	}*/
	if (colourSensorLeft > colourWhiteMin && colourSensorLeft < colourWhiteMax) {
		motor[Left] = reverseSpeed;
		motor[Right] = reverseSpeed + 10;
		//Turn Left on White
	}
	if (colourSensorRight > colourWhiteMin && colourSensorRight < colourWhiteMax) {
		motor[Left] = reverseSpeed + 10;
		motor[Right] = reverseSpeed;
		//Turn Right on White
	}
	if (colourSensorRight> colourWhiteMin && colourSensorLeft > colourWhiteMin && colourSensorLeft < colourWhiteMax && colourSensorRight > colourWhiteMax) {
		//startTask(TurnAround);
		//Turn around if exited green thing
	}
	else if (colourSensorRight> colourGreenMin && colourSensorLeft > colourGreenMin && colourSensorLeft < colourGreenMax && colourSensorRight > colourGreenMax) {
	  motor[Left] = reverseSpeed;
		motor[Right] = motorSpeed;
		//Go Forward on Green
	}
}

/*int intToColour (int value) {
	if (value < colourBlackMax && value > colourBlackMin) {
		return Black;
		currColour = "BLACK";
		//black
	}
	else if (value < colourWhiteMax && value > colourWhiteMin) {
		currColour = "WHITE";
		//white
 	}
 	else if (value < colourGreenMax && value > colourGreenMin) {
 		currColour = "GREEN";
 		//green
	}
}*/

task main(){
	chemicalSpillMode = false;
	
	//Calibrate Black
	color = "BLACK";
	colourBlackMax = CalibrateLightSensor(color) + 3;
	colourBlackMin = CalibrateLightSensor(color) - 3;

	//Calibrate White
	color = "WHITE";
	colourWhiteMax = CalibrateLightSensor(color) + 3;
	colourWhiteMin = CalibrateLightSensor(color) - 3;
	
	//Calibrate Green
	color = "GREEN";
	colourGreenMax = CalibrateLightSensor(color) + 3;
	colourGreenMin = CalibrateLightSensor(color) - 3;
	
	eraseDisplay();	//Clear display for new text
	
	while (true) {
		displayTextLine(1, "BLACK: %4d", colourBlackMax);							//Print Black Max
		displayTextLine(5, "WHITE: %4d", colourWhiteMax);							//Print White Max
		displayTextLine(7, "Reading: %4d", SensorValue(LightLeft)); 	//Print Current Value
		
		if (chemicalSpillMode == true) {
			chemSpillMode();							//Enter Chemical Spill Mode if true
		}
		else {
			navigationMode();							//Enter Navigation Mode if not true (false)
		}
	}
}
