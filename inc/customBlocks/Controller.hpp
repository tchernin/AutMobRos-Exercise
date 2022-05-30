#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include <eeros/control/Block.hpp>
// Include header files for the inputs, outputs and subblocks

using namespace eeros::control;

template <typename T = double>
class Controller : public Blockio<1,1,T>
{
public:
    Controller(): Targetvalue{0.0}, myGain{0.0}, U{0.0} 
    {
        // Connect subblocks, initialize variables, ...
    }
    
    // Implement getter functions for the inputs and outputs
    void setTarget(double phi) {Targetvalue = phi; }
    
    virtual void run()
    {
        // Calculate output values, set timestamps and 
        // call the run method of the subblocks
        U = myGain*(Targetvalue - this->in.getSignal().getValue());
        this->out.getSignal().setValue(U);
        this->out.getSignal().setTimestamp(this->in.getSignal().getTimestamp());
    }

protected:
    // Define inputs, outputs, intermediate variables and subblocks
    double Targetvalue, myGain, U;
};

#endif //CONTOLLER_HPP_
