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
    inputBuffer.resize(INPUT_BUFFER_SIZE);
    hurtBoxes.resize(3); 
    velocity = {0};
   
    otherEntity = nullptr;
    animIndex = 0;
    jumpVelocity = 25.f;
    hitStopFrames = 0;
    _stateMachine = std::make_shared<StateMachine>();
}

bool Entity::wasInputPressedOnFrame(InputTypes inputToCheck, int frame)
{
    const unsigned int _bufferIndex = frame % inputBuffer.max_size();
    const unsigned int _lastBufferIndex = (inputBuffer.max_size() + frame -1) % inputBuffer.max_size();

    const InputData currentInput = inputBuffer.at(_bufferIndex);
    const InputData lastInput = inputBuffer.at(_lastBufferIndex);

    bool Pressed =  false;
    switch (inputToCheck)
    {
        case  InputTypes::UP:
        {             
            Pressed = currentInput.up && !lastInput.up;
            break;
        }
        case  InputTypes::DOWN:
        {             
            Pressed = currentInput.down && !lastInput.down;
            break;
        }
        case  InputTypes::FORWARD:
        {             
            Pressed = currentInput.forward && !lastInput.forward;
            break;
        }
        case  InputTypes::BACKWARD:
        {             
            Pressed = currentInput.backward && !lastInput.backward;
            break;
        }
        case  InputTypes::ATTACK:
        {             
            Pressed = currentInput.attack && !lastInput.attack;
            break;
        }
        
        default:
        {     
            Pressed = false;
            break;
        }
    }

    return Pressed;
}

bool Entity::wasInputPressed(InputTypes inputToCheck)
{
    const InputData currentInput = GetCurrentInputCommand();
    const InputData lastInput = GetLastInputCommand();
    bool Pressed = false;

    switch (inputToCheck)
    {
        case  InputTypes::UP:
        {             
            Pressed = currentInput.up && !lastInput.up;
            break;
        }
        case  InputTypes::DOWN:
        {             
            Pressed = currentInput.down && !lastInput.down;
            break;
        }
        case  InputTypes::FORWARD:
        {             
            Pressed = currentInput.forward && !lastInput.forward;
            break;
        }
        case  InputTypes::BACKWARD:
        {             
            Pressed = currentInput.backward && !lastInput.backward;
            break;
        }
        case  InputTypes::ATTACK:
        {             
            Pressed = currentInput.attack && !lastInput.attack;
            break;
        }
        
        default:
        {     
            Pressed = false;
            break;
        }
    }

    return Pressed;
}

void Entity::UpdateInputs()
{
    bufferIndex = (bufferIndex +1) % inputBuffer.max_size();
}

InputData Entity::GetCurrentInputCommand()
{
    return inputBuffer.at(bufferIndex);
}

InputData Entity::GetLastInputCommand()
{
    return inputBuffer.at((inputBuffer.max_size() + bufferIndex -1) % inputBuffer.max_size());
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

    // if(position.y > 0)
    // {
    //     _stateMachine->context.velocity.y -= 2; //Should be a variable rather than a literal. 
    // }

    position =  {position.x + _stateMachine->context.velocity.x, position.y + _stateMachine->context.velocity.y, position.z + _stateMachine->context.velocity.z}; //Bug where the velocity from the last state carries over to new state. Need to fix
    pushBox = {Vector3{position.x - 15, position.y, position.z},Vector3{position.x + 15, position.y + 100, position.z}}; //Update the position of the push box
    hurtBoxes.at(0) = {Vector3{position.x - 25, position.y, position.z},Vector3{position.x + 25, position.y + 50, position.z}};
    hurtBoxes.at(1) = {Vector3{position.x - 25, position.y+50, position.z},Vector3{position.x + 25, position.y + 100, position.z}};
    hurtBoxes.at(2) = {Vector3{position.x - 25, position.y+100, position.z},Vector3{position.x + 25, position.y + 125, position.z}};
    _stateMachine->context.position = position;
    

    //Ensure that the player never goes below the "Floor" of the level. 
    if(position.y < 0)
    {
        position.y = 0;
    }
}

void Entity::GatherInput()
{
    if(hasControl)
    {
        if(IsKeyDown(KEY_LEFT))
        {
            inputCommand.backward = true;
        }
        else
        {
           inputCommand.backward = false;
        }

        if(IsKeyDown(KEY_RIGHT))
        {
            inputCommand.forward = true;
                               
        }
        else
        {
            inputCommand.forward = false;
           
        }

        if(IsKeyDown(KEY_DOWN))
        {
            inputCommand.down = true;           
        }
        else
        {
            inputCommand.down = false;
        }
    

        if(IsKeyPressed(KEY_UP))
        {
            inputCommand.up = true;            
        }
        else
        {
            inputCommand.up = false;
        }
        

        if(IsKeyPressed(KEY_C))
        {
            _stateMachine->HandleStateTransitions(ATTACKING);
            
        }


        _stateMachine->context.input = {
                                        inputCommand.up,
                                        inputCommand.down,
                                        inputCommand.forward,
                                        inputCommand.backward
                                        };
    }
    
}
