#pragma config(Sensor, S1,     LightLeft,      sensorEV3_Color)
#pragma config(Sensor, S2,     LightRight,      sensorEV3_Color)
#pragma config(Sensor, S4,     UltraSonic,     sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          Left,          tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD,          Right,         tmotorEV3_Large, PIDControl, encoder)

//Setup Motor Speeds
int motorSpeed = 30;
int reverseSpeed = -5;

//Temporary values for colours.
int colourBlackMin = 0;
int colourBlackMax = 16;

int colourGreenMin = 16;
int colourGreenMax = 20;

int colourWhiteMin = 20;
int colourWhiteMax = 90;

int colourFoilMin = 90;


//Setup integers for current light sensor values
int colourSensorLeft;
int colourSensorRight;

task setup() {
	//Calibrate black by working out average of two light sensor values plus 5.
	waitUntil(getTouchValue(S4) == 1);
	playSound(soundBlip);
	colourBlackMax = SensorValue(LightLeft) + SensorValue(LightRight) / 2 + 5;
	delay(500);

	//Calibrate white by working out average of two light sensor values minus 5.
	waitUntil(getTouchValue(S4) == 1);
	playSound(soundBlip);
	colourWhiteMin = SensorValue(LightLeft) + SensorValue(LightRight) / 2 - 5;
	delay(500);
}

task chemicalSpill() {
	playSound(soundLowBuzz);
	while (true){
		//Get to the chopper!
	}
}

task main(){
	startTask(setup);
	while (true){
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
