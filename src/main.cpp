/*
#define RED true
enum project_modes{BASIC, LOWERCOMP, UPPERCOMP, SKILLS, NOAUTONOMOUS, UNITTEST};
#define PROJECT_MODE UPPERCOMP


#define RED true
enum project_modes{BASIC, LOWERCOMP, UPPERCOMP, SKILLS, NOAUTONOMOUS, UNITTEST};
#define PROJECT_MODE UPPERCOMP


#define RED false
enum project_modes{BASIC, LOWERCOMP, UPPERCOMP, SKILLS, NOAUTONOMOUS, UNITTEST};
#define PROJECT_MODE LOWERCOMP


#define RED true
enum project_modes{BASIC, LOWERCOMP, UPPERCOMP, SKILLS, NOAUTONOMOUS, UNITTEST};
#define PROJECT_MODE UPPERCOMP


#define RED true
enum project_modes{BASIC, LOWERCOMP, UPPERCOMP, SKILLS, NOAUTONOMOUS, UNITTEST};
#define PROJECT_MODE NOAUTONOMOUS


#define RED false
enum project_modes{BASIC, LOWERCOMP, UPPERCOMP, SKILLS, NOAUTONOMOUS, UNITTEST};
#define PROJECT_MODE BASIC*/

#include "main.h"
#include "../include/okapi/api.hpp"

#include <future>
#include <chrono>
#include <thread>

using namespace okapi;

#if !defined RED
#define RED true
#endif

#if !defined PROJECT_MODE
enum project_modes{BASIC, LOWERCOMP, UPPERCOMP, SKILLS, NOAUTONOMOUS, UNITTEST};
#define PROJECT_MODE SKILLS
#endif



#define WHEEL_SIZE 3_in
#define CHASSIS_SIZE 9.5_in
#define GEAR_RATIO (1 / 1)

#define FRONT_RIGHT_MOTOR 19
#define FRONT_LEFT_MOTOR 12
#define BACK_RIGHT_MOTOR 20
#define BACK_LEFT_MOTOR 11

#define ARM_MOTOR 1
#define EXTENDER_MOTOR 13
#define PULL_RIGHT_MOTOR 10
#define PULL_LEFT_MOTOR 2
#define INTAKE_SPEED 200


Controller masterController;
ControllerId operationControls = ControllerId::partner;

auto drive = ChassisControllerBuilder()
        .withMotors({FRONT_RIGHT_MOTOR, BACK_RIGHT_MOTOR}, {-FRONT_LEFT_MOTOR, -BACK_LEFT_MOTOR})
        .withDimensions(AbstractMotor::gearset::green, {{WHEEL_SIZE, CHASSIS_SIZE}, imev5GreenTPR * GEAR_RATIO})
        .build();

auto profileController = AsyncMotionProfileControllerBuilder()
        .withLimits({1, 2.0, 5.0})
        .withOutput(drive)
        .buildMotionProfileController();

MotorGroup intake{PULL_LEFT_MOTOR, -PULL_RIGHT_MOTOR};

const int downCompletely = 0;
const int upForExtend = 1000;
const int upForLowTower = 2250;
const int upForMedTower = 3000;
const int upForHighTower = 3000;
auto armControl = AsyncPosControllerBuilder().withMotor(-ARM_MOTOR).build();

const int in = 0;
const int half = 4500;
const int out = 6000;
auto extenderControl = AsyncPosControllerBuilder().withMotor(EXTENDER_MOTOR).build();

void initialize() {
    std::cout << "Initialized" << std::endl;
    armControl->tarePosition();
    extenderControl->tarePosition();
    if(!RED)
        drive->setTurnsMirrored(true);
}

void disabled() {}

void competition_initialize() {
    pros::lcd::set_text(1, "OH F*CK");
}

void startIntake() {
    intake.moveVelocity(200);
}

void stopIntake() {
    intake.moveVelocity(0);
}

void flipOut() {
    std::cout << armControl->getTarget() << std::endl;
    armControl->setTarget(2750);
    armControl->waitUntilSettled();
    armControl->setTarget(downCompletely);
    armControl->waitUntilSettled();
}

//Robot should be one foot away from 45° of corner of thing.
void deliverBlocks() {
    drive->moveDistance(18_in);
    armControl->setTarget(250);
    intake.moveVelocity(50);
    extenderControl->setTarget(out);
    extenderControl->waitUntilSettled();
    drive->moveDistance(1.5_in);
    extenderControl->setTarget(in);
    intake.moveVelocity(100);
    drive->moveDistance(-12_in);
    drive->waitUntilSettled();
    armControl->setTarget(0);
    stopIntake();
}

void intakeBlock() {
    double initialVel = drive->getMaxVelocity();
    drive->setMaxVelocity(125);
    intake.moveVelocity(-200);
    drive->moveDistance(8_in);
    intake.moveVelocity(0);
    drive->setMaxVelocity(initialVel);
}

void tower() {
    intakeBlock();
    drive->moveDistance(-8_in);
    armControl->setTarget(upForMedTower);
    armControl->waitUntilSettled();
    drive->setMaxVelocity(100);
    drive->moveDistance(12_in);
    drive->setMaxVelocity(150);
    intake.moveVelocity(100);
    pros::delay(3000);
    intake.moveVelocity(0);
}

void basicAutonomous(){
    std::cout << "Basic Autonomous" << std::endl;
    drive->moveDistance(-24_in);
    drive->moveDistance(24_in);
}

void upperAutonomous(){
    std::cout << "Upper Autonomous" << std::endl;
    masterController.setText(2, 1, "Running");
    //drive->moveDistance(4_in);
    std::cout << armControl->getTarget() << std::endl;
    flipOut();
    armControl->waitUntilSettled();
    intake.moveVelocity(-200);
    drive->setMaxVelocity(85);
    drive->moveDistance(10_in);
    intake.moveVelocity(-0);
    drive->moveDistance(8_in);
    drive->turnAngle(-93_deg);
    drive->moveDistance(7_in);
    intake.moveVelocity(-200);
    drive->moveDistance(8_in);
    intake.moveVelocity(0);
    drive->setMaxVelocity(150);
    drive->turnAngle(-40_deg);

    drive->moveDistance(12_in);
    armControl->setTarget(250);
    intake.moveVelocity(50);
    extenderControl->setTarget(out);
    extenderControl->waitUntilSettled();
    drive->moveDistance(1.5_in);
    extenderControl->setTarget(in);
    intake.moveVelocity(100);
    drive->moveDistance(-12_in);
    drive->waitUntilSettled();
    armControl->setTarget(0);
    stopIntake();


    /*intakeBlock();
    armControl->setTarget(2800);
    extenderControl->setTarget(out - 1500);
    armControl->waitUntilSettled();
    extenderControl->waitUntilSettled();
    armControl->setMaxVelocity(40);
    intake.moveVelocity(-100);
    drive->moveDistance(21.5_in);
    intake.moveVelocity(-200);
    extenderControl->setMaxVelocity(40);
    extenderControl->setTarget(in);
    armControl->setTarget(400);
    extenderControl->waitUntilSettled();
    armControl->waitUntilSettled();
    intake.moveVelocity(0);
    drive->turnAngle(-135_deg);
    drive->moveDistance(36_in);
    deliverBlocks();*/
}

void lowerAutonomous() {
    std::cout << "Lower Autonomous" << std::endl;
    drive->setMaxVelocity(200);
    //drive->moveDistance(5_in);
    flipOut();
    drive->setMaxVelocity(85);
    intake.moveVelocity(-200);
    drive->moveDistance(42_in);
    intake.moveVelocity(0);
    drive->setMaxVelocity(150);
    drive->moveDistance(-17_in);
    drive->turnAngle(132_deg);
    drive->moveDistance(20_in);
    armControl->setTarget(250);
    intake.moveVelocity(50);
    extenderControl->setTarget(out);
    extenderControl->waitUntilSettled();
    drive->moveDistance(1.5_in);
    extenderControl->setTarget(in);
    intake.moveVelocity(100);
    drive->moveDistance(-12_in);
    drive->waitUntilSettled();
    armControl->setTarget(0);
    stopIntake();
}

void skillsAutonomous() {
    std::cout << "Skills Autonomous" << std::endl;
    Timer timer;
    drive->setMaxVelocity(200);
    flipOut();
    drive->setMaxVelocity(85);
    intake.moveVelocity(-200);
    drive->moveDistance(45_in);
    pros::delay(500);
    intake.moveVelocity(0);
    drive->setMaxVelocity(150);
    drive->moveDistance(-23_in);
    drive->turnAngle(135_deg);
    deliverBlocks();
    drive->turnAngle(135_deg);
    deliverBlocks();
    std::string time = std::to_string(timer.getDtFromStart().getValue());
    masterController.setText(1, 1, "Time: " + time);
    drive->turnAngle(127.5_deg);
    drive->moveDistance(22_in);
    tower();
    drive->moveDistance(-18_in);
    armControl->setTarget(downCompletely);
    drive->turnAngle(90_deg);
    drive->moveDistance(16_in);
    tower();
}

void autonomous() {
    std::cout << "Autonomous" << std::endl;
    masterController.setText(2, 1, "Autonomous");
    drive->setMaxVelocity(150);
    //lowerAutonomous();
      switch (PROJECT_MODE) {
        case BASIC:
            basicAutonomous();
            break;
        case LOWERCOMP:
            lowerAutonomous();
            break;
        case UPPERCOMP:
            masterController.setText(2, 1, "UPPERCOMP");
            upperAutonomous();
            break;
        case SKILLS:
            skillsAutonomous();
            break;
        case UNITTEST:
            break;
        case NOAUTONOMOUS:
            return;
    }
    drive->setMaxVelocity(200);
}

void opcontrol() {
    Motor arm(ARM_MOTOR);
    arm.setBrakeMode(AbstractMotor::brakeMode::hold);
    ControllerButton armUpButton(operationControls, ControllerDigital::A);
    ControllerButton armDownButton(operationControls, ControllerDigital::B);

    Motor extender(-EXTENDER_MOTOR);
    extender.setBrakeMode(AbstractMotor::brakeMode::hold);
    ControllerButton extendButton(operationControls, ControllerDigital::up);
    ControllerButton contractButton(operationControls, ControllerDigital::down);

    intake.setBrakeMode(AbstractMotor::brakeMode::hold);
    ControllerButton intakeButton(operationControls, ControllerDigital::R1);
    ControllerButton outpushButton(operationControls, ControllerDigital::R2);

    ControllerButton slowButton(ControllerId::master, ControllerDigital::L2);
    ControllerButton speedButton(ControllerId::master, ControllerDigital::L1);

    ControllerButton fastTurnButton(ControllerId::partner, ControllerDigital::L1);
    ControllerButton normalTurnButton(ControllerId::partner, ControllerDigital::L1);

    bool slowDown = false;
    bool fastTurn = false;

    if(operationControls != ControllerId::master) {
        fastTurnButton = ControllerButton(ControllerId::master, ControllerDigital::R2);
        normalTurnButton = ControllerButton(ControllerId::master, ControllerDigital::R1);
    }

    while (true) {
        double driveFactor = slowDown ? 0.5 : 1;
        double turnFactor = fastTurn ? 1 : 0.5;
        drive->getModel()->arcade(masterController.getAnalog(ControllerAnalog::leftY) * driveFactor,
                                  masterController.getAnalog(ControllerAnalog::rightX) * driveFactor * turnFactor);

        if (armUpButton.isPressed())
            arm.moveVelocity(100);
        else if (armDownButton.isPressed())
            arm.moveVelocity(-100);
        else
            arm.moveVelocity(0);

        if (extendButton.isPressed())
            extender.moveVelocity(100);
        else if (contractButton.isPressed())
            extender.moveVelocity(-100);
        else
            extender.moveVelocity(0);


        if (intakeButton.isPressed())
            intake.moveVelocity(-INTAKE_SPEED);
        else if (outpushButton.isPressed())
            intake.moveVelocity(INTAKE_SPEED);
        else
            intake.moveVelocity(0);

        if (slowButton.isPressed())
            slowDown = true;
        else if (speedButton.isPressed())
            slowDown = false;

        if(operationControls != ControllerId::master) {
            if (fastTurnButton.isPressed())
                fastTurn = true;
            else if (normalTurnButton.isPressed())
                fastTurn = false;
        }

        pros::delay(10);
    }
}
