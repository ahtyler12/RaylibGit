#include <iostream>
#include <functional>
#include <vector>
#include "Components.h"

 #pragma once
/*
    Common states that every character will eventually have. Should be the basis of 
*/

/*
    State List:
    -1: NULL
     0: STANDING
     1: CROUCH
     2: WALKRIGHT
     3: WALKLEFT
     4: JUMPING
     5: FALLING
     6: ATTACKING
     7: SPECIAL
     8: REACTION
     9: LAUNCHREACTION
    10: GUARDREACTION
    11: GRABREACTION

*/

struct StateContext //Holds infromation from the Entity that owns the state machine
{
    std::shared_ptr<PhysicsComponent> _physicsComponent = {};
    std::shared_ptr<InputComponent> _inputComponent = {};
    bool facingRight = true;
    bool shouldDraw = false;

    /*From Here the information is straight from Zinac*/

    bool bTransition = false;
    int nextState = -1;
    /*

    CharacterAction nextAction = NULL;
    */
    std::string nextNamedAction = "";
    bool refTransition = false;
    bool namedTransition = false;
    InputData inputCommand;

public:
    void TransitionToState(int _stateID)
    {
        bTransition = true;
        nextState = _stateID;
        namedTransition = false;
        refTransition = false;
    }

    void TransitionToStateRef(/*ActionData _actionData*/)
    {
        bTransition = true;
        namedTransition = false;
        //nextAction = _actionData;
        refTransition = true;
    }

    void TransitionToNamedAction(std::string _nextState)
    {
        bTransition = true;
        nextState = 6;
        nextNamedAction = _nextState;
        namedTransition = true;
        refTransition = false;
    }

};

struct Inputs /* May not need this if I just use the stuff from the input component */
{
   bool up;
   bool down;
   bool right;
   bool left;
   bool forward;
   bool backward;
   bool lAttack;
   bool mAttack;
   bool hAttack;
   bool grab;


};

struct State
{
   public:
   char name[256] = " ";
   std::vector<BoundingBox> hurtBoxes;
   bool shouldTransition = false;
   int nextState = -1;
   virtual void OnStart(StateContext& _context) = 0; 
   virtual void OnUpdate(StateContext& _context) = 0;
   virtual void DebugDraw(StateContext _context) = 0;
   virtual void OnExit(StateContext& _context) = 0; 
   virtual int TriggerTransition(){return -1;};

};


struct Standing : public State
{   
      
   char name[256] = "Standing";
    
   void OnStart(StateContext& _context) 
   {
      std::cout << "Standing Start\n";
      hurtBoxes.push_back(BoundingBox{Vector3{25, 0, 0},Vector3{25, 50, 0}});
      hurtBoxes.push_back(BoundingBox{Vector3{25, -50, 0},Vector3{25, 100, 0}});
      hurtBoxes.push_back(BoundingBox{Vector3{25, -100, 0},Vector3{25, 125, 0}});
   }

   void OnUpdate(StateContext &_context)
   {
      if(_context._inputComponent->inputCommand.up)
      {
         nextState = 4;
      }
      else if(_context._inputComponent->inputCommand.down)
      {
         nextState = 1;
      }
      else if (_context._inputComponent->inputCommand.right)
      {
          nextState = 2;
      }
      else if (_context._inputComponent->inputCommand.left)
      {
          nextState = 3;
      }
      else if (_context._inputComponent->wasInputPressedOnFrame(InputTypes::LIGHT, 20, _context.facingRight))
      {
          nextState = 6;
      }
      
   }

   void DebugDraw(StateContext _context) 
   {

      for (auto i = hurtBoxes.begin(); i < hurtBoxes.end(); i++)
      {
          DrawBoundingBox(BoundingBox{ Vector3{_context._physicsComponent->_position.x - i->min.x, _context._physicsComponent->_position.y - i->min.y, i->min.z},
                            Vector3{_context._physicsComponent->_position.x + i->max.x, _context._physicsComponent->_position.y + i->max.y, i->max.z} },
              BLUE);
      }
   }

   void OnExit(StateContext& _context) 
   {
      std::cout << "Standing Exit\n";
      nextState = -1;
      hurtBoxes.clear();
   }

   int TriggerTransition() 
   {
      return nextState;
   }
};

struct Crouch : public State
{
 
   void OnStart(StateContext& _context)
   {
      std::cout << "Crouch Start\n";
      hurtBoxes.push_back(BoundingBox{ Vector3{25, 0, 0},Vector3{25, 25, 0} });
      hurtBoxes.push_back(BoundingBox{ Vector3{25, -25, 0},Vector3{25, 50, 0} });
      hurtBoxes.push_back(BoundingBox{ Vector3{25, -50, 0},Vector3{25, 75, 0} });
   }

   void OnUpdate(StateContext &_context) 
   {
      if(!_context._inputComponent->inputCommand.down)
      {
         nextState = 0;
      }

   }

   void DebugDraw(StateContext _context) 
   {
       for (auto i = hurtBoxes.begin(); i < hurtBoxes.end(); i++)
       {
           DrawBoundingBox(BoundingBox{ Vector3{_context._physicsComponent->_position.x - i->min.x, _context._physicsComponent->_position.y - i->min.y, i->min.z},
                             Vector3{_context._physicsComponent->_position.x + i->max.x, _context._physicsComponent->_position.y + i->max.y, i->max.z} },
               BLUE);
       }
   }

   void OnExit(StateContext& _context)
   {
      std::cout << "Crouch Exit\n";
      nextState = -1;
      hurtBoxes.clear();
   }

   int TriggerTransition()
   {
      return nextState;
   }
};

struct Jumping : public State
{
   char name[256] = "Jumping";
   float initialVelocity = 65.f;
   float jumpDeceleration = 8;
   bool startJump =  true;
   void OnStart(StateContext& _context)
   {

      std::cout << "Jump Start\n";
      hurtBoxes.push_back(BoundingBox{ Vector3{25, 0, 0},Vector3{25, 50, 0} });
      hurtBoxes.push_back(BoundingBox{ Vector3{25, -50, 0},Vector3{25, 100, 0} });
      hurtBoxes.push_back(BoundingBox{ Vector3{25, -100, 0},Vector3{25, 125, 0} });
   }

   void OnUpdate(StateContext &_context)
   {
      if(startJump)
      {
         _context._physicsComponent->_velocity.y = initialVelocity; //Very Hacky way of doing this. Probably an easier way of doing it
         startJump = false;
      }   
      _context._physicsComponent->_velocity.y -= jumpDeceleration;

      if(_context._physicsComponent->_velocity.y < 0)
      {
         nextState = 5;
      }
      

   }

   void DebugDraw(StateContext _context)
   {
       for (auto i = hurtBoxes.begin(); i < hurtBoxes.end(); i++)
       {
           DrawBoundingBox(BoundingBox{ Vector3{_context._physicsComponent->_position.x - i->min.x, _context._physicsComponent->_position.y - i->min.y, i->min.z},
                             Vector3{_context._physicsComponent->_position.x + i->max.x, _context._physicsComponent->_position.y + i->max.y, i->max.z} },
               BLUE);
       }
   }

   void OnExit(StateContext& _context)
   {
      std::cout << "Jump Exit\n";
      startJump =  true;
      nextState = -1;
      hurtBoxes.clear();
   }

   int TriggerTransition() 
   {
      return nextState;
   }
};

struct Falling : public State
{
   void OnStart(StateContext& _context)
   {
      std::cout << "Falling Start\n";
      hurtBoxes.push_back(BoundingBox{ Vector3{25, 0, 0},Vector3{25, 50, 0} });
      hurtBoxes.push_back(BoundingBox{ Vector3{25, -50, 0},Vector3{25, 100, 0} });
      hurtBoxes.push_back(BoundingBox{ Vector3{25, -100, 0},Vector3{25, 125, 0} });
   }

   void OnUpdate(StateContext &_context)
   {
      _context._physicsComponent->_velocity.y = -8;   
      if(_context._physicsComponent->_position.y <= 0)
      {
         nextState = 0;
      }

   }
   
   void DebugDraw(StateContext _context)
   {
       for (auto i = hurtBoxes.begin(); i < hurtBoxes.end(); i++)
       {
           DrawBoundingBox(BoundingBox{ Vector3{_context._physicsComponent->_position.x - i->min.x, _context._physicsComponent->_position.y - i->min.y, i->min.z},
                             Vector3{_context._physicsComponent->_position.x + i->max.x, _context._physicsComponent->_position.y + i->max.y, i->max.z} },
               BLUE);
       }
   }

   void OnExit(StateContext& _context)
   {
      std::cout << "Falling End\n";
      nextState = -1;
      hurtBoxes.clear();
   }

   int TriggerTransition() 
   {
      return nextState;
   }
};

struct Attack : public State
{
   int duration = 100;

   void OnStart(StateContext& _context)
   {
      std::cout << "Attack Start\n";
   }

   void OnUpdate(StateContext &_context)
   {
      if(duration == 0)
      {
         nextState = 0;
      }
      else
      {
         duration -= 1;
         std::cout << "Duration left " << duration << " \n";
      }

   }
   
   void DebugDraw(StateContext _context)
   {
      
   }

   void OnExit(StateContext& _context)
   {
      std::cout << "Attack End\n";
      nextState = -1;
   }

   int TriggerTransition() 
   {
      return nextState;
   }

};

struct ReactionState : public State
{
   int duration = 100;

   void OnStart(StateContext& _context)
   {
      std::cout << "Reaction Start\n";
   }

   void OnUpdate(StateContext &_context)
   {
      if(duration == 0)
      {
         nextState = 0;
      }
      else
      {
         std::cout << "Duration left " << duration << " \n";
         duration -= 1;         
      }

   }
   
   void DebugDraw(StateContext _context)
   {
      
   }

   void OnExit(StateContext& _context)
   {
      std::cout << "Reaction End\n";
      nextState = -1;
   }

   int TriggerTransition() 
   {
      return nextState;
   }
};

struct WalkRight : public State
{

    char name[256] = "Walk Right";

    void OnStart(StateContext& _context) 
    {
        std::cout << "Walk Right Start\n";
        hurtBoxes.push_back(BoundingBox{ Vector3{25, 0, 0},Vector3{25, 50, 0} });
        hurtBoxes.push_back(BoundingBox{ Vector3{25, -50, 0},Vector3{25, 100, 0} });
        hurtBoxes.push_back(BoundingBox{ Vector3{25, -100, 0},Vector3{25, 125, 0} });

    }

    void OnUpdate(StateContext& _context) 
    {
        _context._physicsComponent->_velocity.x = 2;
        if (!_context._inputComponent->inputCommand.right)
        {
            nextState = 0;
            
        }
        else if (_context._inputComponent->inputCommand.down)
        {
            nextState = 1;
        }
        else if (_context._inputComponent->inputCommand.left)
        {
            nextState = 3;
        }

    }

    void DebugDraw(StateContext _context)
    {
        for (auto& box : hurtBoxes)
        {
            DrawBoundingBox(BoundingBox{ Vector3{_context._physicsComponent->_position.x - box.min.x, _context._physicsComponent->_position.y - box.min.y, box.min.z},
                                        Vector3{_context._physicsComponent->_position.x + box.max.x, _context._physicsComponent->_position.y + box.max.y, box.max.z} },
                BLUE);
        }

    }

    void OnExit(StateContext& _context) 
    {
        std::cout << "Walk Righ Exit\n";
        nextState = -1;
        hurtBoxes.clear();
    }

    int TriggerTransition() 
    {
        return nextState;
    }
};

struct WalkLeft : public State
{

    char name[256] = "Walk Left";

    void OnStart(StateContext& _context) 
    {
        std::cout << "Walk Left Start\n";
        hurtBoxes.push_back(BoundingBox{ Vector3{25, 0, 0},Vector3{25, 50, 0} });
        hurtBoxes.push_back(BoundingBox{ Vector3{25, -50, 0},Vector3{25, 100, 0} });
        hurtBoxes.push_back(BoundingBox{ Vector3{25, -100, 0},Vector3{25, 125, 0} });
        
    }

    void OnUpdate(StateContext& _context) 
    {
        _context._physicsComponent->_velocity.x = -2;
        if (!_context._inputComponent->inputCommand.left)
        {
            nextState = 0;
        }
        else if (_context._inputComponent->inputCommand.down)
        {
            nextState = 1;
        }
        else if (_context._inputComponent->inputCommand.right)
        {
            nextState = 2;
        }
    }

    void DebugDraw(StateContext _context)
    {
        for (auto& box : hurtBoxes)
        {
            DrawBoundingBox(BoundingBox{ Vector3{_context._physicsComponent->_position.x - box.min.x, _context._physicsComponent->_position.y - box.min.y, box.min.z},
                                        Vector3{_context._physicsComponent->_position.x + box.max.x, _context._physicsComponent->_position.y + box.max.y, box.max.z} },
                BLUE);
        }

    }

    void OnExit(StateContext& _context) 
    {
        std::cout << "Walk Left Exit\n";
        nextState = -1;
        hurtBoxes.clear();
    }

    int TriggerTransition() 
    {
        return nextState;
    }
};

 
 