#include "raylib.h"
#include <iostream>
#include <vector>
#include <memory>
#include "Components.h"
#include "StateMachine.h"
 


struct HitEvent //This gets sent to the entity that receives a hit in order to update physics
{
    int pushBack = 0;
    int hitStun = 0;
    int hitStop = 0;
    bool isLaunch = false;
    int launchForce = 0;
};

struct AttackProperties //Holds properties for each attack
{
    /*Properties for the hitbox bounding box*/
    float top;
    float bottom;
    float front;
    float back;
    float left;
    float right;
    float size;
    Vector3 position;


};




class Entity
{
    public:
    Entity();

    /*Common Variables*/
    float maxHealth = 1;
    float currentHealth;
    float maxMeter = 1;
    float currentMeter;
    float walkSpeed;
    float backWalkSpeed;
    float forwarddashDistance;
    float backdashDistance;
    Vector3 velocity;
    float jumpVelocity;
    Vector3 position;
    //Current number of jumps
    int jumpCount;
    //Actions available in the air
    int airActionsAvailable = 2;

    /*Model Related Variables*/
    Model entityModel;
    ModelAnimation *entityAnimations;
    int entityAnimCount;
    int animIndex; 
    int currentAnimFrame = 0;
    Vector3 rotationAxis, scale /*this might be useful for flipping the model need to test more*/;
    float rotationAngle;
    bool isFacingRight;
    bool canFlip;
    bool isStuckInEnemy;


    /*Damage Related*/
    int stunFrames;
    int hitStopFrames;
    bool shouldWallBounce;
    bool shouldGroundBounce;
    bool counterHitActive;
    //Number of frames where an Instant Block is considered valid
    static const int instantBlockWindow = 6;
    //Number of frames needed to pass to not get locked out of instant block
    static const int ibLockOut = 10;
    //Number of frames before another instant block can be performed
    int ibCoolDown;
    //The frame to start checking for an Instant Block. -1 indicates no opportunity for instant block
    int startIBframe;
    bool shouldHardKnockdown;
    bool landedHit;
    float damageDecayModifier;
    float hitStunDecayModifier; 
    int comboCounter;

    /*Input variable*/
    InputData currentInput;
    std::shared_ptr<InputComponent> _inputComponent;
    std::shared_ptr<PhysicsComponent> _physicsComponent;
    
    /*Reference to the other player*/
    std::shared_ptr<Entity> otherEntity;

    /*Pushbox Data*/
    Vector3 size;
    BoundingBox pushBox;
    std::vector<BoundingBox> hurtBoxes;

    /*State Data*/
   std::shared_ptr<StateMachine> _stateMachine;

    bool debug;
    bool hasControl = false;

    bool CheckCollision(std::shared_ptr<Entity> _entity);
    void Draw();
    void Update();
    void UpdateAnimations(unsigned int _animIndex);
    void PlayAnimation(ModelAnimation anim);
    void UpdatePhysics();
    void GatherInput();
    void HandleHitEvent(HitEvent _event);

};