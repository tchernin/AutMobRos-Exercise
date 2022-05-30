#include "MyRobotSafetyProperties.hpp"

MyRobotSafetyProperties::MyRobotSafetyProperties(ControlSystem &cs, double dt)
    : cs(cs),
    
      slSystemOff("System is offline"),
      slSystemOn("System is online"),
      slSystem_ing_Off("System ing off"),
      slSystem_ing_On("System ing on"),
      slError("Error"),
      slMotorOff("Motor is off"),
      slMotor_ing_Off("Motor ing off"),
      slMotor_ing_On("Motor ing on"),
      slMotorOn("Motor is on"),
  

      doSystemOn("Startup the system"),
      doSystemOff("Shutdown the system"),
      doingSystemOn("Starting the system"),
      doingSystemOff("Stopping the system"),
      doEmergency("Emergency"),
      doReleaseError("ReleaseError"),
      doMotorOff("Motor Off"),
      doMotorOn("Motor On"),
      doingMotorOff("Stopping the motor"),
      doingMotorOn("Starting the motor")
 
{
    eeros::hal::HAL &hal = eeros::hal::HAL::instance();

    // Declare and add critical outputs
    greenLED = hal.getLogicOutput("onBoardLEDgreen");
    redLED = hal.getLogicOutput("onBoardLEDred");

    criticalOutputs = { greenLED, redLED };


    // Declare and add critical inputs
    buttonPause = eeros::hal::HAL::instance().getLogicInput("onBoardButtonPause");
    buttonMode = eeros::hal::HAL::instance().getLogicInput("onBoardButtonMode");

    criticalInputs = { buttonPause, buttonMode };

    // Add all safety levels to the safety system
    addLevel(slSystemOff);
    addLevel(slSystem_ing_Off);
    addLevel(slError);
    addLevel(slSystem_ing_On);
    addLevel(slSystemOn);
    addLevel(slMotorOff);
    addLevel(slMotor_ing_Off);
    addLevel(slMotor_ing_On);
    addLevel(slMotorOn);


    // Add events to individual safety levels
    slSystemOff.addEvent(doSystemOn, slSystem_ing_On, kPublicEvent);
    slSystem_ing_On.addEvent(doingSystemOn, slSystemOn, kPrivateEvent); 
    slSystem_ing_Off.addEvent(doingSystemOff, slSystemOff, kPrivateEvent);
    slError.addEvent(doReleaseError, slSystem_ing_On, kPublicEvent);
    slSystemOn.addEvent(doMotorOn, slMotor_ing_On, kPublicEvent);
    slMotorOff.addEvent(doMotorOn, slMotor_ing_On, kPublicEvent);
    slMotor_ing_On.addEvent(doingMotorOn, slMotorOn, kPrivateEvent);
    slMotorOn.addEvent(doMotorOff, slMotor_ing_Off, kPublicEvent);
    slMotor_ing_Off.addEvent(doingMotorOff, slMotorOff, kPrivateEvent);

    
    // Add events to multiple safety levels
    // addEventToAllLevelsBetween(lowerLevel, upperLevel, event, targetLevel, kPublicEvent/kPrivateEvent);
    //exple: addEventToAllLevelsBetween(slSystemOn, slMotorPowerOn, emergency, slEmergency, kPublicEvent);
    addEventToAllLevelsBetween(slError, slMotorOn, doSystemOff,slSystem_ing_Off,kPublicEvent);
    addEventToAllLevelsBetween(slSystemOn, slMotorOn, doEmergency, slError,kPublicEvent);

    // Define input actions for all levels
    // level.setInputActions({ ... });
    slSystemOff.setInputActions({      ignore(buttonPause),                    ignore(buttonMode) });
    slSystem_ing_Off.setInputActions({      ignore(buttonPause),                    ignore(buttonMode) });
    slError.setInputActions({ ignore(buttonPause),                    check(buttonMode, false, doReleaseError) });
    slSystem_ing_On.setInputActions({  ignore(buttonPause),                    ignore(buttonMode) });
    slSystemOn.setInputActions({  check(buttonPause, false, doEmergency),   ignore(buttonMode)});
    slMotorOff.setInputActions({  check(buttonPause, false, doEmergency),   ignore(buttonMode)});
    slMotor_ing_Off.setInputActions({ ignore(buttonPause),                    ignore(buttonMode)});
    slMotor_ing_On.setInputActions({ ignore(buttonPause),                    ignore(buttonMode)});
    slMotorOn.setInputActions({ check(buttonPause, false, doEmergency),   ignore(buttonMode)});


    // Define output actions for all levels
    // level.setOutputActions({ ... });
    slSystemOff.setOutputActions({        set(greenLED, false),   set(redLED, false) });
    slSystem_ing_Off.setOutputActions({   set(greenLED, false),   set(redLED, false) });
    slError.setOutputActions({            set(greenLED, false),    set(redLED, true) });
    slSystem_ing_On.setOutputActions({    set(greenLED, true),   set(redLED, false)});
    slSystemOn.setOutputActions({         set(greenLED, true),    set(redLED, true)});
    slMotorOff.setOutputActions({         set(greenLED, true),   set(redLED, false)});
    slMotor_ing_Off.setOutputActions({    set(greenLED, true),   set(redLED, false)});
    slMotor_ing_On.setOutputActions({     set(greenLED, true),   set(redLED, false)});
    slMotorOn.setOutputActions({          set(greenLED, true),   set(redLED, true)});

    // Define and add level actions
    slSystemOff.setLevelAction([&](SafetyContext *privateContext) {
        eeros::Executor::stop();
        //or
        //privateContext->triggerEvent(doSystemOn);
    });


    slSystem_ing_Off.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.stop();
        privateContext->triggerEvent(doingSystemOff);
    });

    slError.setLevelAction([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(doReleaseError);
        //or
        //privateContext->triggerEvent(doSystemOff);
        
    });

    slSystem_ing_On.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.start();
        privateContext->triggerEvent(doingSystemOn);
    });

    
    slSystemOn.setLevelAction([&, dt](SafetyContext *privateContext) {
        if (slSystemOn.getNofActivations()*dt >= 1)   // wait 1 sec
        {
            privateContext->triggerEvent(doMotorOn);

        }
    });
    
    slMotorOff.setLevelAction([&](SafetyContext *privateContext) {
        // Check if motors are standing sill
        privateContext->triggerEvent(doMotorOn);
    });


    slMotor_ing_Off.setLevelAction([&](SafetyContext *privateContext) {
        // Check if motors are standing sill
        privateContext->triggerEvent(doingMotorOff);
    });


    slMotor_ing_On.setLevelAction([&](SafetyContext *privateContext) {
        // Check if motors are standing sill
        privateContext->triggerEvent(doingMotorOn);
    });

    slMotorOn.setLevelAction([&](SafetyContext *privateContext) {
        // Check if motors are standing sill
        privateContext->triggerEvent(doMotorOff);
    });




    // Define entry level
    setEntryLevel(slSystemOff);

    // Define exit function
    exitFunction = ([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(doSystemOff);
    });
}
