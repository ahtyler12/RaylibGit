#include <iostream>
#include <functional>
#include <vector>
 #pragma once


struct Inputs
{
   bool up;
   bool down;
   bool forward;
   bool back;
};



struct StateContext //Holds infromation from the Entity that owns the state machine
{
   Vector3 position = {0.f,0.f,0.f}; //Position Variable is bugged for whatever reason. the X and Y locations are in screen space instead of world space
   Vector3 velocity = {0.f,0.f,0.f};
   Inputs input = {0};
   bool shouldDraw = false;
};

enum StateID : __int32
{
    NONE,
    STANDING,
    CROUCHING,
    JUMPING,
    FALLING,
    ATTACKING,
    SPECIAL,
    REACTION,
    LAUNCHREACTION,
    GUARDREACTION,
    GRABREACTION

};


struct State
{
   public:
   char name[256] = " ";
   std::vector<BoundingBox> hurtBoxes;
   bool shouldTransition = false;
   StateID nextState = StateID::NONE;
   virtual void OnStart() = 0; //Treat like the initialization of the state. Any resources that should be aquired are created here
   virtual void OnUpdate(StateContext &_context) = 0;
   virtual void DebugDraw(StateContext _context) = 0;
   virtual void OnExit() = 0; //Treat like the deinitialization of the state. Any resources that are aquired are destroyed here
   virtual StateID TriggerTransition(){return StateID::NONE;};

};


struct Standing : public State
{   
      
   char name[256] = "Standing";

   void OnStart() override
   {
      std::cout << "Standing Start\n";
      hurtBoxes.push_back(BoundingBox{Vector3{25, 0, 0},Vector3{25, 50, 0}});
      hurtBoxes.push_back(BoundingBox{Vector3{25, 50, 0},Vector3{25, 100, 0}});
      hurtBoxes.push_back(BoundingBox{Vector3{25, 100, 0},Vector3{25, 125, 0}});

   }

   void OnUpdate(StateContext &_context) override
   {
      if(_context.input.up)
      {
         nextState = JUMPING;
      }
      else if(_context.input.down)
      {
         nextState = CROUCHING;
      }

      // if(_context.shouldDraw)
      // {
      //    DebugDraw(_context);
      // }

      
   }

   void DebugDraw(StateContext _context)
   {
      for (auto &box : hurtBoxes)
      {
         DrawBoundingBox(BoundingBox{Vector3{_context.position.x - box.min.x, _context.position.y - box.min.y, box.min.z},
                                     Vector3{_context.position.x + box.max.x, _context.position.y + box.max.y, box.max.z}},
                                      BLUE);
      }
      
   }

   void OnExit() override
   {
      std::cout << "Standing Exit\n";
      nextState = StateID::NONE;
      hurtBoxes.clear();
   }

   StateID TriggerTransition() override
   {
      return nextState;
   }
};

struct Crouch : public State
{
 
   void OnStart()
   {
      std::cout << "Crouch Start\n";
   }

   void OnUpdate(StateContext &_context) override
   {
      if(!_context.input.down)
      {
         nextState = STANDING;
      }

   }

   void DebugDraw(StateContext _context) 
   {
      
   }

   void OnExit()
   {
      std::cout << "Crouch Exit\n";
      nextState = StateID::NONE;
   }

   StateID TriggerTransition()
   {
      return nextState;
   }
};

struct Jumping : public State
{
   char name[256] = "Jumping";
   float initialVelocity = 40.f;
   float jumpDeceleration = -2;
   bool startJump =  true;
   void OnStart()
   {

      std::cout << "Jump Start\n";
      
   }

   void OnUpdate(StateContext &_context)
   {
      if(startJump)
      {
         _context.velocity.y = initialVelocity; //Very Hacky way of doing this. Probably an easier way of doing it
         startJump = false;
      }   
      _context.velocity.y -=4;   

      if(_context.velocity.y < 0)
      {
         nextState = FALLING;
      }
      
      // if(_context.shouldDraw)
      // {
      //    DebugDraw(_context);
      // }

   }

   void DebugDraw(StateContext _context)
   {
      
   }

   void OnExit()
   {
      std::cout << "Jump Exit\n";
      startJump =  true;
      nextState = StateID::NONE;
   }

   StateID TriggerTransition() override
   {
      return nextState;
   }
};

struct Falling : public State
{
   void OnStart()
   {
      std::cout << "Falling Start\n";
   }

   void OnUpdate(StateContext &_context)
   {
      //_context.velocity.y -=2;   
      if(_context.position.y == 0)
      {
         nextState = STANDING;
      }

   }
   
   void DebugDraw(StateContext _context)
   {
      
   }

   void OnExit()
   {
      std::cout << "Falling End\n";
      nextState = StateID::NONE;
   }

   StateID TriggerTransition() override
   {
      return nextState;
   }
};

struct Attack : public State
{
   int duration = 100;

   void OnStart()
   {
      std::cout << "Attack Start\n";
   }

   void OnUpdate(StateContext &_context)
   {
      if(duration == 0)
      {
         nextState = STANDING;
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

   void OnExit()
   {
      std::cout << "Attack End\n";
      nextState = StateID::NONE;
   }

   StateID TriggerTransition() override
   {
      return nextState;
   }

};

struct ReactionState : public State
{
   int duration = 100;

   void OnStart()
   {
      std::cout << "Reaction Start\n";
   }

   void OnUpdate(StateContext &_context)
   {
      if(duration == 0)
      {
         nextState = STANDING;
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

   void OnExit()
   {
      std::cout << "Reaction End\n";
      nextState = StateID::NONE;
   }

   StateID TriggerTransition() override
   {
      return nextState;
   }
};





 
 