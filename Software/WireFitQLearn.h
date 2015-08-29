#ifndef WIREFITQLEARN_H
#define WIREFITQLEARN_H

#include <iostream>
#include <vector>
#include <stdlib.h>

#include "NeuralNet.h"
#include "BackPropagation.h"

namespace net {
    struct Wire {
        std::vector<double> action;
        double reward;
    };
    
    class WireFitQLearn {
    public:
        NeuralNet *network;
        Backpropagation backprop;
        int numberOfWires, actionDimensions;
        double learningRate, devaluationFactor;
        double scalingFactorToMillis;
        std::vector<double> lastState, lastAction;
        
        WireFitQLearn(NeuralNet *modelNetwork, Backpropagation backprop_, double learningRate_, double devaluationFactor_, int actionDimensions_, int numberOfWires_);
        
        WireFitQLearn(std::string filename);
        
        WireFitQLearn();
        
        std::vector<double> chooseBestAction(std::vector<double> currentState);
        
        std::vector<double> chooseBoltzmanAction(std::vector<double> currentState, double explorationConstant);
        
        void applyReinforcementToLastAction(double reward, std::vector<double> newState, double elapsedTimeMillis);
        
        void storeWireFitQLearn(std::string filename);
    private:
        std::vector<Wire> getWires(std::vector<double> state);
        
        std::vector<double> getRawOutput(std::vector<Wire> wires);
        
        double highestReward(std::vector<double> state);
        
        std::vector<double> bestAction(std::vector<double> state);
        
        double getRewardUsingInterpolator(std::vector<double> state, std::vector<Wire> controlWires, std::vector<double> action);
        
        std::vector<Wire> newControlWires(double newReward, std::vector<Wire> oldControlWires);
        
        double distanceBetweenWireAndAction(std::vector<double> &state, Wire &wire, std::vector<double> &action, double maxReward);
        double weightedSum(std::vector<double> &state, std::vector<Wire> &wires, std::vector<double> &action);
        double normalize(std::vector<double> &state, std::vector<Wire> &wires, std::vector<double> &action);
    };
};

#endif