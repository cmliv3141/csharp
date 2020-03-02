#pragma config(Sensor, in1,    marbleID,       sensorReflection)
#pragma config(Sensor, dgtl1,  cupRotate,      sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  startButton,    sensorTouch)
#pragma config(Sensor, dgtl4,  ledGood,        sensorLEDtoVCC)
#pragma config(Sensor, dgtl5,  ledCaution,     sensorLEDtoVCC)
#pragma config(Sensor, dgtl6,  ledError,       sensorLEDtoVCC)
#pragma config(Sensor, dgtl7,  led1,           sensorLEDtoVCC)
#pragma config(Sensor, dgtl8,  led2,           sensorLEDtoVCC)
#pragma config(Sensor, dgtl9,  led3,           sensorLEDtoVCC)
#pragma config(Sensor, dgtl10, led4,           sensorLEDtoVCC)
#pragma config(Sensor, dgtl11, led5,           sensorLEDtoVCC)
#pragma config(Sensor, dgtl12, led6,           sensorLEDtoVCC)
#pragma config(Motor,  port1,           flashlight,    tmotorVexFlashlight, openLoop, reversed)
#pragma config(Motor,  port2,           marbleGrabber, tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           cupRotater,    tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           gateServo,     tmotorServoStandard, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*  definitions and variables for the motor slew rate controller.              */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

#define MOTOR_NUM               kNumbOfTotalMotors
#define MOTOR_MAX_VALUE         127
#define MOTOR_MIN_VALUE         (-127)
#define MOTOR_DEFAULT_SLEW_RATE 10      // Default will cause 375mS from full fwd to rev
#define MOTOR_FAST_SLEW_RATE    256     // essentially off
#define MOTOR_TASK_DELAY        15      // task 1/frequency in mS (about 66Hz)
#define MOTOR_DEADBAND          10

// Array to hold requested speed for the motors
// int motorReq[ MOTOR_NUM ];

// Array to hold "slew rate" for the motors, the maximum change every time the task
// runs checking current mootor speed.
// int motorSlew[ MOTOR_NUM ];

// CODEDOC:
// Multitasking - https://www.robotc.net/wikiarchive/Multitasking_tips
// Bumpswitch - http://cdn.robotc.net/pdfs/natural-language/hp_touch.pdf

int bump;
int count;
int injected;
bool done;
bool scanNow;
bool injectNow;
bool gateOpen;
//bool initial = true;

task init()
{
	turnLEDOn(ledError);
	// TurnOff
	turnFlashlightOff(flashlight);
	turnLEDOff(ledGood);
	turnLEDOff(ledCaution);
	turnLEDOff(led1);
	turnLEDOff(led2);
	turnLEDOff(led3);
	turnLEDOff(led4);
	turnLEDOff(led5);
	turnLEDOff(led6);
	stopMotor(marbleGrabber);
	stopMotor(cupRotater);

	//while (initial) {
	//turnLEDOn(ledError);
	// Start Init
	robotType(none);
	turnFlashlightOn(flashlight, 127);
	setServo(gateServo, 40);
	//setServo(gateServo, -100);
	// End Init
	sleep(1000);
	turnLEDOff(ledError);
	turnLEDOn(ledGood);
	//abortTimeslice();
	//}
}

task injectMarble() {
	while (true) {
		if (injectNow == true) {
			startMotor(marbleGrabber, -29);
			sleep(400);
			injected = injected + 1;
			stopMotor(marbleGrabber);
			sleep(650);
		}
		else {
			break;
		}
	}
}

task opperateGate() {
	if (gateOpen == true) {
		setServo(gateServo, 64);
		sleep(500);
		gateOpen = false;
	}
	if (gateOpen == false) {
		setServo(gateServo, -64);
		sleep(500);
		gateOpen = true;
	}
}

task scanMarble() {
	while (true) {
		if (scanNow == true) {
			// ADD CODE
			startMotor(cupRotater, 64);
			sleep(500);
			stopMotor(cupRotater);
			sleep(550);
		}
		else {
			break;
		}
	}
}

task ledDebug() {
	while (injected < 16) {
		if (injected == 2) {
			turnLEDOn(led1);
		}
		if (injected == 4) {
			turnLEDOn(led1);
			turnLEDOn(led2);
		}
		if (injected == 7) {
			turnLEDOn(led1);
			turnLEDOn(led2);
			turnLEDOn(led3);
		}
		if (injected == 10) {
			turnLEDOn(led1);
			turnLEDOn(led2);
			turnLEDOn(led3);
			turnLEDOn(led4);
		}
		if (injected == 12) {
			turnLEDOn(led1);
			turnLEDOn(led2);
			turnLEDOn(led3);
			turnLEDOn(led4);
			turnLEDOn(led5);
		}
		if (injected == 14) {
			turnLEDOn(led1);
			turnLEDOn(led2);
			turnLEDOn(led3);
			turnLEDOn(led4);
			turnLEDOn(led5);
			turnLEDOn(led6);
		}
		if (injected == 15) {
			done = true;
		}
	}
}

task main()
{
	//bump = getBumperValue(startButton);
	startTask(init);

	untilTouch(startButton);

	injectNow = true;

	startTask(injectMarble);

	scanNow = true;

	startTask(scanMarble);

	gateOpen = true;

	startTask(ledDebug);

	for (count=0; count < 15; count++) {
		startTask(opperateGate);
	}

	while (done == false)
	{
		// Keep the program alive
		sleep(1000);

		// FOR MOTOR TESTING PURPOSES
		//startMotor(cupRotater, 127);
		//startMotor(marbleGrabber, 127);
	}

	if (done == true) {
		done = false;
		//bump = getBumperValue(startButton);
		startTask(init);

		untilTouch(startButton);

		injectNow = true;

		startTask(injectMarble);

		scanNow = true;

		startTask(scanMarble);

		gateOpen = true;

		startTask(ledDebug);

		for (count=0; count < 15; count++) {
			startTask(opperateGate);
		}

		while (done == false)
		{
			// Keep the program alive
			sleep(1000);

			// FOR MOTOR TESTING PURPOSES
			//startMotor(cupRotater, 127);
			//startMotor(marbleGrabber, 127);
		}
	}
}
