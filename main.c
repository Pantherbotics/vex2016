#pragma platform(VEX)
#pragma config(Motor,  port1,           mFrontLeft,    tmotorVex393_HBridge,       openLoop, reversed)
#pragma config(Motor,  port2,           mShooter2,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           mShooter3,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port4,           mShooter4,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           mBackLeft,     tmotorVex393_MC29,          openLoop, reversed)
#pragma config(Motor,  port6,           mFrontRight,   tmotorVex393_MC29,          openLoop)
#pragma config(Motor,  port7,           mShooter7,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           mShooter8,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           mShooter9,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          mBackRight,    tmotorVex393_HBridge,       openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard, cleaned up by me!!*//

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

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
   motor[mShooter3] = -power;
   motor[mShooter4] = -power;
   motor[mShooter7] = -power;
   motor[mShooter8] = power;
   motor[mShooter9] = power;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                          Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
/////////////////////////////////////////////////////////////////////////////////////////

void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
  // Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task autonomous()
{
  // .....................................................................................
  // Insert user code here.
  // .....................................................................................

	AutonomousCodePlaceholderForTesting();  // Remove this function call once you have "real" code.
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task usercontrol()
{
	//Main operator control loop
  while(true){
	  int x = vexRT[joyDriveA];
	  int y = vexRT[joyDriveB];
  	int z = vexRT[joyDriveC];

  	//Basic configuration for 4 meccanum wheel drive
  	setDriveMotors(x + z + y,
	                 x - z - y,
	                 x + z - y,
	                 x - z + y);

	  //bring the shooter to a full stop permanently
  	if (vexRT[joyShooterZero] == 1) {
	  	shooterSpeed = 0;
	  	setShooterMotors(0);

	  //Increment the motor speed by 10
  	}else if (vexRT[joyShooterIncU] == 1) {
	    shooterSpeed += shooterIncrement;
	    setShooterMotors(shooterSpeed);

	  //Decrement the motor speed by 10
	  }else if (vexRT[joyShooterIncD] == 1) {
	    shooterSpeed -= shooterIncrement;
	    setShooterMotors(shooterSpeed);

	  //Set the shooter speed to the maximum temporarily
    }else if (vexRT[joyShooterFull] == 1) {
		  setShooterMotors(127);

    }else{
		  setShooterMotors(shooterSpeed);
    } //End shooter button if statements
  } //End main program loop
}
