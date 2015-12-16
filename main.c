#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in8,    ballDetect,     sensorLineFollower)
#pragma config(Sensor, dgtl1,  rampSolenoidA,  sensorDigitalOut)
#pragma config(Sensor, dgtl2,  rampSolenoidB,  sensorDigitalOut)
#pragma config(Sensor, dgtl3,  alignSolenoid,  sensorDigitalOut)
#pragma config(Sensor, dgtl4,  shootSolenoid,  sensorDigitalOut)
#pragma config(Sensor, dgtl5,  tournamentJumper, sensorDigitalIn)
#pragma config(Sensor, dgtl6,  autonJumper,    sensorDigitalIn)
#pragma config(Sensor, dgtl12, ShooterReadyLED, sensorLEDtoVCC)
#pragma config(Sensor, I2C_1,  encLeftFront10, sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  encShooterLeft7B, sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_3,  encLeftBack6,   sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_4,  endBackRight5,  sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_5,  encShooterRight2, sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_6,  encRightFront1, sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           mFrontRight,   tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           mShooter2,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           mShooter3,     tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           mShooter4,     tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           mBackRight,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           mBackLeft,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           mShooter7,     tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           mShooter8,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           mShooter9,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          mFrontLeft,    tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#ifdef _MSC_VER
int SensorValue[];int motor[];int vexRT[];int nLCDButtons;int nImmediateBatteryLevel;int bLCDBacklight;
typedef int bool;
#define true 1
#define false 0
typedef char* string;
typedef void task;

int bumpLeft;int bumpRight;int mShooter2;int mShooter3;int mShooter4;int mShooter7;int mShooter8;int mShooter9;int mFrontLeft;int mFrontRight;int mBackLeft;int mBackRight;int ShooterReadyLED;int encShooterRight2;int rampSolenoidA;int rampSolenoidB;int Ch1;int Ch2;int Ch3;int Ch4;int Btn5U;int Btn6D;int Btn5D;int Btn6U;int Btn7U;int Btn7D;int Btn7L;int Btn7R;int Btn8U;int Btn8D;int Btn8L;int Btn8R;int alignSolenoid;int shootSolenoid;int bIfiAutonomousMode;int bIfiRobotDisabled;int time1[];int T1;int T2;int T4;int autonJumper;int nSysTime;
#endif

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(15)
#pragma userControlDuration(120)

#define USE_BUTTON_PLACEHOLDER
//#include "../CustomLCD.c"

#ifdef _MSC_VER
#include "C:\Program Files (x86)\Robomatter Inc\ROBOTC Development Environment 4.X\Includes\Vex_Competition_Includes.c"   //Main competition background code...do not modify!
#else
#include "Vex_Competition_Includes.c"
#endif

//--------------------Button Mapping--------------------//
//Drive axis
#define joyDriveA Ch2 //First axis for the drive joysticks
#define joyDriveB Ch4 //Second axis for the drive joysticks
#define joyDriveC Ch1 //Third axis for the drive joysticks
#define joyTurnRight Btn6U
#define joyTurnLeft Btn5U
#define joyTurnRightSlow Btn6D
#define joyTurnLeftSlow Btn5D

//Shooter buttons
#define joyShooterZero Btn7L //Set the shooter speed to zero (E-STOP)
#define joyShooterIncU Btn7U //Increment shooter speed
#define joyShooterIncD Btn7D //Decrement the shooter speed
#define joyShooterFull Btn7R //Set the shooter speed to max (127)

//Pneumatics Buttons
#define joyRampActivate  Btn8U //Activates the ramp solenoids
#define joyAlignActivate Btn8D //Toggles the alignment solenoids
#define JoyShooterManual Btn8R //Manual trigger for the shooter solenoid
#define joyShooterAuto   Btn8L //Toggles automatic triggering of the shooter solenoid

//--------------------Constants--------------------//
const int ballDetectThreshold = 2525;
const int defaultManualSpeed = 75;
const int optimalSpeed = 39.0

//--------------------Variables--------------------//
int lastSysTime = 0;   //Stores the previous system time
float lastSpeedA = 0;  //The previously measured speed for shooter encoder A
int shooterMotorRaw = 0; //stores the current set speed for the shooter motors
int lastEncA = 0;      //The previous encoder count of shooter encoder A
int currentDistA = 0;  //The current encoder count of shooter encoder A
float speedAverages = 0; //The calculated average of both shooter encoders
float manualSetSpeed = 0;  //the manually adjusted speed
bool ready = false;       //true if the shooter is within a wide margin of the target speed
int lastShootTime = 0;

//--------------------Helper Functions-------------//
//Helper function for setting all drive motors in one command
void setDriveMotors(int fL, int fR, int bL, int bR) {
	motor[mFrontLeft] = fL;
	motor[mFrontRight] = fR;
	motor[mBackLeft] = bL;
	motor[mBackRight] = bR;
}

//Helper function for setting all shooter motors in one command, with correct polarity
void setShooterMotors(int power) {
	motor[mShooter2] = power;
	motor[mShooter3] = power;
	motor[mShooter4] = power;
	motor[mShooter7] = power;
	motor[mShooter8] = power;
	motor[mShooter9] = power;
}

void calculateShooter() {
	wait1Msec(50);
	lastEncA = currentDistA;                   //Get the prevoius speed of the shooter

	currentDistA = SensorValue[encShooterRight2];    //Get the current speed of the shooter SensorValue[encShooterRight2]
	int currSysTime;
	//Calculate the motor speed based on the system timer and the motor distance. Average the results, weighing
	//heavily on the previous value
	float speed = ((currentDistA - lastEncA) * 50.0 / ((currSysTime = nSysTime) - lastSysTime));
	speedAverages = speedAverages*0.9+ speed*0.1;

	//Save the previous states for next iteration
	lastSpeedA = speed;
	lastSysTime = currSysTime;

	if (speed > 80) { speed = 80; }           //Clamp the aspeed to make sure it doesn't go over 50/s
	else if (speed < -80) { speed = -80; }    // (prevents it from generating erronously high values)

	//Calculate error and add in a reversal gain if we are approaching the speed
	//(prevents overshooting due to the flywheel behavior of the shooter wheels)
  float error = optimalSpeed - speedAverages;
  if (abs(error) < 0.25) {error = 0;}
  else if (abs(error) < 0.5) {error = error*-0.35;}


  //Calculate power based on the error
  shooterMotorRaw = manualSetSpeed+ error*2.9;
  if (shooterMotorRaw > 127) { shooterMotorRaw = 127; }                    //Clamp the motor output to prevent error
	else if (shooterMotorRaw < -127) { shooterMotorRaw = -127; }             //accumulation from going too crazy

	clearLCDLine(1);
	ready = (speedAverages > optimalSpeed - 1 && speedAverages < optimalSpeed + 1);
  bLCDBacklight = ready;
  string str;
	stringFormat(str, "M %-2i/%-3is:%-2i/%i",manualSetSpeed,shooterMotorRaw,speedAverages,optimalSpeed);
  displayLCDCenteredString(1, str);

}

//Takes manual joystick inputs to control solenoids
void solenoidsManual() {
	if (vexRT[joyRampActivate] ) {
		SensorValue[rampSolenoidA] = 1; //Set the state of the ramp
		SensorValue[rampSolenoidB] = 1; //Set the state of the ramp
	}
	else if (!vexRT[joyRampActivate]) {
		SensorValue[rampSolenoidA] = 0; //Set the state of the ramp
		SensorValue[rampSolenoidB] = 0; //Set the state of the ramp
	}

	//SensorValue[shootSolenoid] = vexRT[JoyShooterManual];
}

//--------------------Initalization Code--------------------//
void pre_auton() {
	bStopTasksBetweenModes = false; //Set false for user tasks to run between mode switches
	SensorValue[shootSolenoid] = 0;
	SensorValue[rampSolenoidA] = 0;
	SensorValue[rampSolenoidB] = 0;
	SensorValue[alignSolenoid] = 1;

}

//--------------------Autonomous mode--------------------//
task autonomous() {
	SensorValue[shootSolenoid] = 0;
	clearTimer(T1);
	clearTimer(T3);
	int state = 0;
	ready = false;
	speedAverages = 0;
	manualSetSpeed = defaultManualSpeed;
		while (bIfiAutonomousMode && !bIfiRobotDisabled && !SensorValue[autonJumper]) {
			if (ready && state == 0) { //Robot is spinning up, 0 balls shot
				SensorValue[shootSolenoid] = 1;
				clearTimer(T1);
				state = 1;

			}else if (state == 1){     //Robot is spun up, 1 ball shot
	  		SensorValue[shootSolenoid] = 1;
				clearTimer(T1);
				state = 2;

		  }else if (state == 2 && time1[T1] > 1000 && ready) { //Robot is spun up, 1 ball shot
		    SensorValue[shootSolenoid] = 0;
		    state = 3;
		    clearTimer(T1);

		  }else if (state == 3 && time1[T1] > 1000) {
		    //SensorValue[shootSolenoid] = 1;
				clearTimer(T1);
				state = 1;}

			if (SensorValue[ballDetect] <= ballDetectThreshold && time1[T3] > 800) {
			  lastShootTime = time1[T3];
		  	writeDebugStreamLine("%i",lastShootTime);
		  	clearTimer(T3);
		  }
		  writeDebugStreamLine("T1%i T3%i ready %i  %f",time1[T3],time1[T1],ready, speedAverages);

		  calculateShooter();                //Calculate the shooter's speed and the motor speed
      if (time1[T3] < 300) {shooterMotorRaw = 127;}
			setShooterMotors(shooterMotorRaw); //set the shooter motor's speed

		}

	setShooterMotors(0);


}

//--------------------Manual Control Loop--------------------//
task usercontrol() {
	//Main operator control loop
	SensorValue[shootSolenoid] = 0; //Set the shooter to open
	manualSetSpeed = defaultManualSpeed;
	setShooterMotors(0);
	clearTimer(T3);

	while (true) {

		int x = vexRT[joyDriveA];
		int y = vexRT[joyDriveB];
		int z = (vexRT[joyTurnRight] - vexRT[joyTurnLeft]) * 127 + (vexRT[joyTurnRightSlow] - vexRT[joyTurnLeftSlow]) * 50;

		//Basic configuration for 4 meccanum wheel drive
		setDriveMotors(x + z + y,
			x - z - y,
			x + z - y,
			x - z + y);

		//bring the shooter to a full stop permanently
		if (vexRT[joyShooterZero] == 1) {
			shooterMotorRaw = 0;
			setShooterMotors(0);
			manualSetSpeed = 0;

			//Increment the target
		}
		else if (vexRT[joyShooterIncU] == 1) {
			manualSetSpeed+= 0.5;

			//Decrement the target
		}
		else if (vexRT[joyShooterIncD] == 1) {
			manualSetSpeed-=0.5;
			//Set the shooter speed to the optimal speed
		}
		else if (vexRT[joyShooterFull] == 1) {
			manualSetSpeed = 58;
		} //shooter button if statements

		//Display the last shooter time on the LCD
		clearLCDLine(0);
		string str;
		stringFormat(str, "Timer:%ims",lastShootTime);
		displayLCDCenteredString(0, str);

		//Record the time since the last ball was shot
		if (SensorValue[ballDetect] <= ballDetectThreshold && time1[T3] > 800) {
			lastShootTime = time1[T3];
			writeDebugStreamLine("%i",lastShootTime);
			clearTimer(T3);
		}

		calculateShooter();
		if (time1[T3] < 300) {shooterMotorRaw = 127;} //Set the power briefly to max after shooting a ball
		setShooterMotors(shooterMotorRaw); //set the shooter motor's speed

		solenoidsManual(); //Get button innputs for solenoid control

	} //Main Loop


}

void frameUpdate(int frameNumber) {

}
