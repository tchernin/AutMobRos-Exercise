#include "ControlSystem.hpp"

ControlSystem::ControlSystem(double dt)
    : 
      timedomain("Main time domain", dt, true),
      //Ex cs
      //q1("quat1"),
      //servo("servo1"),
      //g(2.0)
      //Ex interfacing the dc motor and encoder
      q1("enc1"),
      motor("motor1"),
      g(2.0)
{
    // Name all blocks

    //Ex cs
    //for the imu
    //q1.setName("q1");
    //for constant to retrieve the rotation angle alpha
    //g.setName("g");
    //for the servo
    //servo.setName("servo");

    //Ex interfacing the dc motor and encoder
    //for the imu
    q1.setName("q1");
    //for constant to retrieve the rotation angle alpha
    g.setName("g");
    //for the servo
    motor.setName("motor");

    // Name all signals
    //Ex cs
    //q1.getOut().getSignal().setName("Alpha/2 [rad]");
    //g.getOut().getSignal().setName("Alpha [rad]");

    //Ex interfacing the dc motor and encoder
    q1.getOut().getSignal().setName(" Phi [rad]");
    g.getOut().getSignal().setName("ang speed [rad/2]");   

    // Connect signals
    //Ex cs
    //servo.getIn().connect(g.getOut());
    //g.getIn().connect(q1.getOut());

    //Ex interfacing the dc motor and encoder
    g.getIn().connect(q1.getOut());
    motor.getIn().connect(g.getOut());


    // Add blocks to timedomain in the order as they are executed
    //Ex cs
    //timedomain.addBlock(q1);
    //timedomain.addBlock(g);
    //timedomain.addBlock(servo);

    //Ex interfacing the dc motor and encoder
    timedomain.addBlock(q1);
    timedomain.addBlock(g);
    timedomain.addBlock(motor);


    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);
}