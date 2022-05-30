#ifndef MyRobotSAFETYPROPERTIES_HPP_
#define MyRobotSAFETYPROPERTIES_HPP_

#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>
#include "ControlSystem.hpp"

class MyRobotSafetyProperties : public eeros::safety::SafetyProperties
{
public:
    MyRobotSafetyProperties(ControlSystem &cs, double dt);

    // Define all possible events
    eeros::safety::SafetyEvent doSystemOff;
    eeros::safety::SafetyEvent doingSystemOff;
    eeros::safety::SafetyEvent doingSystemOn;
    eeros::safety::SafetyEvent doSystemOn;
    eeros::safety::SafetyEvent doEmergency;
    eeros::safety::SafetyEvent doReleaseError;
    eeros::safety::SafetyEvent doMotorOff;
    eeros::safety::SafetyEvent doMotorOn;
    eeros::safety::SafetyEvent doingMotorOff;
    eeros::safety::SafetyEvent doingMotorOn;


    // Define all possible levels
    eeros::safety::SafetyLevel slSystemOff;
    eeros::safety::SafetyLevel slSystem_ing_Off; 
    eeros::safety::SafetyLevel slSystem_ing_On;
    eeros::safety::SafetyLevel slSystemOn;
    eeros::safety::SafetyLevel slError;
    eeros::safety::SafetyLevel slMotorOff;
    eeros::safety::SafetyLevel slMotor_ing_Off;
    eeros::safety::SafetyLevel slMotor_ing_On;
    eeros::safety::SafetyLevel slMotorOn;
 




private:
    // Define all critical outputs
    eeros::hal::Output<bool>* greenLED;
    eeros::hal::Output<bool>* redLED;

    // Define all critical inputs
    eeros::hal::Input<bool>* buttonPause;
    eeros::hal::Input<bool>* buttonMode;    

    ControlSystem &cs;
};

#endif // MyRobotSAFETYPROPERTIES_HPP_
