#include "raylib.h"
#include "CommonStates.h"
#include <any>
#include <functional>
#include <memory>
#include <vector>
#include <iostream> //Remove this at some point. We really don't need it aside from debugging
#include <string>
#include "Components.h"


#pragma once

enum StateID : __int32 //These are going away in favor of integer based ID's. That way there is no need to constatnly update the list of state id's and they can be assigned when the state is created instead
{
    NONE,
    STANDING,
    CROUCHING,
    WALKRIGHT,
    WALKLEFT,
    JUMPING,
    FALLING,
    ATTACKING,
    SPECIAL,
    REACTION,
    LAUNCHREACTION,
    GUARDREACTION,
    GRABREACTION
};

struct StateCallbacks 
{
    StateID stateID;
    std::function<void(StateContext&)> OnStart;
    std::function<void(StateContext&)> OnUpdate;
    std::function<void(StateContext&)> OnExit;
    std::function<int()> OnTransition;
    std::function<void(StateContext)> OnDraw;
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