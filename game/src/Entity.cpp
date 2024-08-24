#include "Entity.h"


Entity::Entity()
{
    currentMeter = maxMeter;
    currentHealth = maxHealth;
    entityModel = LoadModel("Eve.glb"); //Should have a variable for the path so that every entity can load it's own model
    entityAnimations = LoadModelAnimations("Eve.glb", &entityAnimCount);
    animIndex = 1;
    rotationAngle = 90.f;
    rotationAxis =  {1.f, 0.f,0.f};
    scale = {1.f,1.f,1.f};
    debug = true;
    comboCounter = 0;
    hurtBoxes.resize(3); 
    velocity = {0};
   
    otherEntity = nullptr;
    animIndex = 0;
    jumpVelocity = 25.f;
    hitStopFrames = 0;
    _physicsComponent = std::make_shared<PhysicsComponent>();
    _inputComponent = std::make_shared<InputComponent>();

    _stateMachine = std::make_shared<StateMachine>();
    _stateMachine->context._inputComponent = _inputComponent;
    _stateMachine->context._physicsComponent = _physicsComponent;
}

void Entity::HandleHitEvent(HitEvent _event)
{
    if(hitStopFrames == 0){hitStopFrames = _event.hitStop;}
}


bool Entity::CheckCollision(std::shared_ptr<Entity> entity)
{
    if(CheckCollisionBoxes(pushBox, entity->pushBox))
    {
        return true;
    }
    return false;
}

void Entity::Draw()
{
    //DrawModelEx(entityModel, position, rotationAxis, rotationAngle, scale, WHITE);
    if(debug)
    {
        DrawCubeWires(position, 10.f, 10.f, .1f, YELLOW);
        //Visual representation of the pushbox for debuging purposes
        DrawBoundingBox(pushBox, GREEN);
        _stateMachine->context.shouldDraw = true;
        _stateMachine->currentState.OnDraw(_stateMachine->context);
    }
}

void Entity::Update()
{
    UpdatePhysics();

    _stateMachine->context.shouldDraw = debug;

    _stateMachine->UpdateState();
    //PlayAnimation(entityAnimations[animIndex]);
    
}

void Entity::UpdateAnimations(unsigned int _animIndex)
{
    /*May need to make these variables globals for state machine functionality*/
    ModelAnimation currentAnim = entityAnimations[_animIndex]; //Set the current animation
    currentAnimFrame = 0;
    PlayAnimation(currentAnim);
}

void Entity::PlayAnimation(ModelAnimation anim)
{
    UpdateModelAnimation(entityModel, anim, currentAnimFrame);
    if(currentAnimFrame >= anim.frameCount)
    {
        currentAnimFrame = 0; //Should Automatically loop the animation
    }
    else
    {
        currentAnimFrame++;
    }
}

void Entity::UpdatePhysics()
{
    if(hitStopFrames > 0)
    {
        std::cout << "In hitstop for:" << hitStopFrames << " frames!!!"<< std::endl;
        hitStopFrames-=1;
    }
    /*Determine if oponent is on right or left*/
    if(otherEntity != nullptr)
    {
        if(otherEntity->position.x > position.x)
        {
            isFacingRight = true;
        }
        else
        {
            isFacingRight = false;
        }
    }


    position =  {position.x + _stateMachine->context._physicsComponent->_velocity.x, position.y + _stateMachine->context._physicsComponent->_velocity.y, position.z + _stateMachine->context._physicsComponent->_velocity.z}; //Bug where the velocity from the last state carries over to new state. Need to fix     
    //Ensure that the player never goes below the "Floor" of the level. 
    if(position.y < 0)
    {
        position.y = 0;
    }
    pushBox = { Vector3{position.x - 15, position.y, position.z},Vector3{position.x + 15, position.y + 100, position.z} }; //Update the position of the push box
    _stateMachine->context._physicsComponent->_position = position;
}

void Entity::GatherInput()
{
    if(hasControl)
    {
        if(IsKeyDown(KEY_LEFT))
        {
            currentInput.left = true;
        }
        else
        {
           currentInput.left = false;
        }

        if(IsKeyDown(KEY_RIGHT))
        {
            currentInput.right = true;
                               
        }
        else
        {
            currentInput.right = false;
           
        }

        if(IsKeyDown(KEY_DOWN))
        {
            currentInput.down = true;           
        }
        else
        {
            currentInput.down = false;
        }
    

        if(IsKeyPressed(KEY_UP))
        {
            currentInput.up = true;            
        }
        else
        {
            currentInput.up = false;
        }
        

        if(IsKeyPressed(KEY_C))
        {
            _stateMachine->HandleStateTransitions(ATTACKING);
            
        }


        _stateMachine->context._inputComponent->inputCommand = {
                                        currentInput.up,
                                        currentInput.down,
                                        currentInput.right,
                                        currentInput.left
                                        };
    }
    
}
