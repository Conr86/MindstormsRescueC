#pragma config(Sensor, S1,     LightLeft,      sensorEV3_Color)
#pragma config(Sensor, S2,     LightRight,      sensorEV3_Color)
#pragma config(Sensor, S4,     UltraSonic,     sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          Left,          tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD,          Right,         tmotorEV3_Large, PIDControl, encoder)

//Setup Motor Speeds
int motorSpeed = 30;			//Adjustable, preferably above 20 and below 50
int reverseSpeed = -5; 			//This is only used in turning.

//Temporary values for colours.
int colourBlackMin = 0;			//Lowest possible value (Darkest / Black)
int colourBlackMax = 16;		//This value changes in calibration

int colourGreenMin = 16;		//Needs to be adjusted, it may conflict with colourBlackMax
int colourGreenMax = 20;		//This value changes in calibration

int colourWhiteMin = 20;		//Value needs to be adjusted, possibly higher value
int colourWhiteMax = 90;		//This value changes in calibration

int colourFoilMin = 90;			//Doesn't need to be calibrated. Could even be raised to '95'


//Setup variables for current light sensor values
int colourSensorLeft;			//Constantly updates. Holds current S1 value (0-100)
int colourSensorRight;			//Constantly updates. Holds current S2 value (0-100)
task setup() {
	//Set black variable by working out average of the two light sensor values plus 5
  colourSensorLeft = SensorValue(LightLeft);
	colourSensorRight = SensorValue(LightRight);
	waitUntil(getTouchValue(S4) == 1);	//Wait until Touch sensor is pressed
	playSound(soundBlip);			//Plays simple 'Beep' sound.
	colourBlackMax = colourSensorLeft + colourSensorRight / 2 + 5;
	delay(500);				//Wait 500 milliseconds

	//Calibrate white by working out average of two light sensor values minus 5.
	waitUntil(getTouchValue(S4) == 1);  	//Wait until touch sensor is pressed
	playSound(soundBlip);			//Plays simple 'Beep' sound.
	colourWhiteMin = SensorValue(LightLeft) + SensorValue(LightRight) / 2 - 5;
	delay(500);				//Wait 500 Milliseconds
}

task chemicalSpill() {				//Begin Chemical Spill mode
	playSound(soundLowBuzz);		//Play 'buzz' sound
	while (true){				//Create new loop, it will never exit this.
		//Get to the chopper!
	}
}

task main(){
	startTask(setup);			//Run setup
	while (true){				//Loop
		colourSensorLeft = SensorValue(LightLeft);
		colourSensorRight = SensorValue(LightRight);

		if (colourSensorLeft > colourBlackMin && colourSensorLeft < colourBlackMax) {
			motor[Left] = motorSpeed;				
			motor[Right] = reverseSpeed;
			//Turn Left on Black
		}
		if (colourSensorRight > colourBlackMin && colourSensorRight < colourBlackMax) {
			motor[Left] = reverseSpeed;
			motor[Right] = motorSpeed;
			//Turn Right on Black
		}
		if (colourSensorRight > colourGreenMin && colourSensorRight < colourGreenMax) {
			motor[Left] = reverseSpeed;
			motor[Right] = motorSpeed;
			//Turn Right on Green
		}
		if (colourSensorLeft > colourGreenMin && colourSensorLeft < colourGreenMax) {
			motor[Left] = motorSpeed;
			motor[Right] = reverseSpeed;
			//Turn Left on Green
		}
		if (colourSensorRight > colourFoilMin && colourSensorLeft > colourFoilMin) {
			startTask(chemicalSpill);
			//Activate Chemical Spill Mode
		}
		else if (colourSensorRight> colourWhiteMin && colourSensorLeft > colourWhiteMin && colourSensorLeft < colourWhiteMax && colourSensorRight > colourWhiteMax) {
			motor[Left] = motorSpeed;
			motor[Right] = motorSpeed;
			//Go Forward on White
		}
	}
}
