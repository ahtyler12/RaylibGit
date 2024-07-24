#include "raylib.h"
#include "CommonStates.h"
#include <functional>
#include <memory>
#include <vector>
#include <iostream> //Remove this at some point. We really don't need it aside from debugging
#include <string>

#pragma once


struct StateCallbacks 
{
    StateID stateID;
    std::function<void()> OnStart;
    std::function<void(StateContext &)> OnUpdate;
    std::function<void()> OnExit;
    std::function<StateID()> OnTransition;
};




class StateMachine
{
    public:
    StateMachine();

    StateContext context;
    StateCallbacks currentState = {}; //Make sure we don't have a garbage value
    void HandleStateTransitions(StateID _id); //
    void UpdateState();

    private:
       std::vector<StateCallbacks> Callbacks;
       bool canTransition;
       void RegisterState(std::shared_ptr<State> _newState, StateID _id); //May need to return a bool if the state was unable to be added
      
    
    
};