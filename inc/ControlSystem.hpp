#ifndef CONTROLSYSTEM_HPP_
#define CONTROLSYSTEM_HPP_

#include <eeros/control/TimeDomain.hpp>
#include <eeros/core/Executor.hpp>
#include <eeros/control/Gain.hpp>
#include <eeros/control/PeripheralInput.hpp> // to define the block sensor
#include <eeros/control/PeripheralOutput.hpp> // to define the block for the actuator
//#include "customBlocks/Controller.hpp"

using namespace eeros::control;

class ControlSystem
{
public:
    ControlSystem(double dt);

    // Define Blocks
    //conttroller block
    //Controller<> controller;
    //Ex cs
    //Gain<> g;
    //PeripheralInput<> q1;
    //PeripheralOutput<> servo;

    //Ex interfacing the dc motor and encoder
    Gain<> g;
    PeripheralInput<> q1;
    PeripheralOutput<> motor;

    TimeDomain timedomain;
};

#endif // CONTROLSYSTEM_HPP