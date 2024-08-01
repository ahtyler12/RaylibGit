#include <iostream>
#include <functional>
#include <vector>
 #pragma once


struct Inputs
{
   bool up;
   bool down;
   bool right;
   bool left;
};



struct StateContext //Holds infromation from the Entity that owns the state machine
{
   Vector3 position = {0.f,0.f,0.f}; //Position Variable is bugged for whatever reason. the X and Y locations are in screen space instead of world space
   Vector3 velocity = {0.f,0.f,0.f};
   Inputs input = {0};
   bool shouldDraw = false;

   void resetXVelocity() { velocity.x = 0; }
   void resetYVelocity() { velocity.y = 0; }
   void resetPosition(){ position = { 0.f,0.f,0.f }; }

};

enum StateID : __int32
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


struct State
{
   public:
   char name[256] = " ";
   std::vector<BoundingBox> hurtBoxes;
   bool shouldTransition = false;
   StateID nextState = StateID::NONE;
   virtual void OnStart(StateContext& _context) = 0; //Treat like the initialization of the state. Any resources that should be aquired are created here
   virtual void OnUpdate(StateContext &_context) = 0;
   virtual void DebugDraw(StateContext _context) = 0;
   virtual void OnExit(StateContext& _context) = 0; //Treat like the deinitialization of the state. Any resources that are aquired are destroyed here
   virtual StateID TriggerTransition(){return StateID::NONE;};

};


struct Standing : public State
{   
      
   char name[256] = "Standing";

   void OnStart(StateContext& _context) override
   {
      std::cout << "Standing Start\n";
      hurtBoxes.push_back(BoundingBox{Vector3{25, 0, 0},Vector3{25, 50, 0}});
      hurtBoxes.push_back(BoundingBox{Vector3{25, -50, 0},Vector3{25, 100, 0}});
      hurtBoxes.push_back(BoundingBox{Vector3{25, -100, 0},Vector3{25, 125, 0}});
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
      else if (_context.input.right)
      {
          nextState = WALKRIGHT;
      }
      else if (_context.input.left)
      {
          nextState = WALKLEFT;
      }
      
   }

   void DebugDraw(StateContext _context) override
   {

      for (auto i = hurtBoxes.begin(); i < hurtBoxes.end(); i++)
      {
          DrawBoundingBox(BoundingBox{ Vector3{_context.position.x - i->min.x, _context.position.y - i->min.y, i->min.z},
                            Vector3{_context.position.x + i->max.x, _context.position.y + i->max.y, i->max.z} },
              BLUE);
      }
   }

   void OnExit(StateContext& _context) override
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
 
   void OnStart(StateContext& _context)
   {
      std::cout << "Crouch Start\n";
      hurtBoxes.push_back(BoundingBox{ Vector3{25, 0, 0},Vector3{25, 25, 0} });
      hurtBoxes.push_back(BoundingBox{ Vector3{25, -25, 0},Vector3{25, 50, 0} });
      hurtBoxes.push_back(BoundingBox{ Vector3{25, -50, 0},Vector3{25, 75, 0} });
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
       for (auto i = hurtBoxes.begin(); i < hurtBoxes.end(); i++)
       {
           DrawBoundingBox(BoundingBox{ Vector3{_context.position.x - i->min.x, _context.position.y - i->min.y, i->min.z},
                             Vector3{_context.position.x + i->max.x, _context.position.y + i->max.y, i->max.z} },
               BLUE);
       }
   }

   void OnExit(StateContext& _context)
   {
      std::cout << "Crouch Exit\n";
      nextState = StateID::NONE;
      hurtBoxes.clear();
   }

   StateID TriggerTransition()
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
         _context.velocity.y = initialVelocity; //Very Hacky way of doing this. Probably an easier way of doing it
         startJump = false;
      }   
      _context.velocity.y -= jumpDeceleration;   

      if(_context.velocity.y < 0)
      {
         nextState = FALLING;
      }
      

   }

   void DebugDraw(StateContext _context)
   {
       for (auto i = hurtBoxes.begin(); i < hurtBoxes.end(); i++)
       {
           DrawBoundingBox(BoundingBox{ Vector3{_context.position.x - i->min.x, _context.position.y - i->min.y, i->min.z},
                             Vector3{_context.position.x + i->max.x, _context.position.y + i->max.y, i->max.z} },
               BLUE);
       }
   }

   void OnExit(StateContext& _context)
   {
      std::cout << "Jump Exit\n";
      startJump =  true;
      nextState = StateID::NONE;
      hurtBoxes.clear();

      _context.resetYVelocity();
   }

   StateID TriggerTransition() override
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
      _context.velocity.y = -8;   
      if(_context.position.y <= 0)
      {
         nextState = STANDING;
      }

   }
   
   void DebugDraw(StateContext _context)
   {
       for (auto i = hurtBoxes.begin(); i < hurtBoxes.end(); i++)
       {
           DrawBoundingBox(BoundingBox{ Vector3{_context.position.x - i->min.x, _context.position.y - i->min.y, i->min.z},
                             Vector3{_context.position.x + i->max.x, _context.position.y + i->max.y, i->max.z} },
               BLUE);
       }
   }

   void OnExit(StateContext& _context)
   {
      std::cout << "Falling End\n";
      nextState = StateID::NONE;
      hurtBoxes.clear();
      _context.resetYVelocity();
   }

   StateID TriggerTransition() override
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

   void OnExit(StateContext& _context)
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

   void OnStart(StateContext& _context)
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

   void OnExit(StateContext& _context)
   {
      std::cout << "Reaction End\n";
      nextState = StateID::NONE;
   }

   StateID TriggerTransition() override
   {
      return nextState;
   }
};

struct WalkRight : public State
{

    char name[256] = "Walk Right";

    void OnStart(StateContext& _context) override
    {
        std::cout << "Walk Right Start\n";
        hurtBoxes.push_back(BoundingBox{ Vector3{25, 0, 0},Vector3{25, 50, 0} });
        hurtBoxes.push_back(BoundingBox{ Vector3{25, -50, 0},Vector3{25, 100, 0} });
        hurtBoxes.push_back(BoundingBox{ Vector3{25, -100, 0},Vector3{25, 125, 0} });

    }

    void OnUpdate(StateContext& _context) override
    {
        _context.velocity.x = 2;
        if (!_context.input.right)
        {
            nextState = STANDING;
            
        }
        else if (_context.input.down)
        {
            nextState = CROUCHING;
        }
        else if (_context.input.left)
        {
            nextState = WALKLEFT;
        }

    }

    void DebugDraw(StateContext _context)
    {
        for (auto& box : hurtBoxes)
        {
            DrawBoundingBox(BoundingBox{ Vector3{_context.position.x - box.min.x, _context.position.y - box.min.y, box.min.z},
                                        Vector3{_context.position.x + box.max.x, _context.position.y + box.max.y, box.max.z} },
                BLUE);
        }

    }

    void OnExit(StateContext& _context) override
    {
        std::cout << "Walk Righ Exit\n";
        nextState = StateID::NONE;
        hurtBoxes.clear();
        _context.resetXVelocity();
    }

    StateID TriggerTransition() override
    {
        return nextState;
    }
};

struct WalkLeft : public State
{

    char name[256] = "Walk Left";

    void OnStart(StateContext& _context) override
    {
        std::cout << "Walk Left Start\n";
        hurtBoxes.push_back(BoundingBox{ Vector3{25, 0, 0},Vector3{25, 50, 0} });
        hurtBoxes.push_back(BoundingBox{ Vector3{25, -50, 0},Vector3{25, 100, 0} });
        hurtBoxes.push_back(BoundingBox{ Vector3{25, -100, 0},Vector3{25, 125, 0} });
        
    }

    void OnUpdate(StateContext& _context) override
    {
        _context.velocity.x = -2;
        if (!_context.input.left)
        {
            nextState = STANDING;
        }
        else if (_context.input.down)
        {
            nextState = CROUCHING;
        }
        else if (_context.input.right)
        {
            nextState = WALKRIGHT;
        }
    }

    void DebugDraw(StateContext _context)
    {
        for (auto& box : hurtBoxes)
        {
            DrawBoundingBox(BoundingBox{ Vector3{_context.position.x - box.min.x, _context.position.y - box.min.y, box.min.z},
                                        Vector3{_context.position.x + box.max.x, _context.position.y + box.max.y, box.max.z} },
                BLUE);
        }

    }

    void OnExit(StateContext& _context) override
    {
        std::cout << "Walk Left Exit\n";
        nextState = StateID::NONE;
        hurtBoxes.clear();
        _context.resetXVelocity();
    }

    StateID TriggerTransition() override
    {
        return nextState;
    }
};

 
 