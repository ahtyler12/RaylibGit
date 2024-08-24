#include <vector>
#pragma once
enum InputTypes
{
    UP,
    DOWN,
    RIGHT,
    LEFT,
    BACKWARD,
    FORWARD,
    ATTACK,
    LIGHT,
    MEDIUM,
    HEAVY,
    THROW
};



struct InputData
{
    bool up;
    bool down;
    bool left;
    bool right;
    bool forward;
    bool backward;
    bool attack;

    void Reset()
    {
        up = false;
        down = false;
        right = false;
        forward = false;
        backward = false;
        left = false;
        attack = false;
    }

};

const int Directions[9] =
{
    1, //Down Back
    2, //Down
    3, //Down Forward
    4, //Back
    5, //Neutral
    6, //Forward
    7, //Up Back
    8, //Up
    9  // Up Forward
};

enum MotionNames
{
    FIRST,
    QCF,
    QCB,
    DP,
    RDP,
    HCF,
    HCB,
    LAST
};

const int MotionInputs[MotionNames::LAST][8] =
{
    {},
    {2,3,6}, //Quarter Circle Forward
    {2,1,4}, //Quarter Circle Back
    {6,2,3}, //Dragon Punch Forward
    {4,2,1},  //Dragon Punch Back
    {4,1,2,3,6}, // Half Circle Forward
    {6,3,2,1,4} // Half Circle Back
};



class InputComponent
{
public:
    InputComponent();
    void UpdateInput();
    InputData GetCurrentInputCommand();
    InputData GetLastInputCommand();
    bool CheckNumpadDirection(InputData _input, int direction, bool facingRight);
    bool wasInputPressedOnFrame(InputTypes inputToCheck, int frame, bool facingRight);
    bool wasInputPressed(InputTypes inputToCheck, bool facingRight);
    bool WasInputPressBuffered(InputTypes _input, int duration, bool facingRight);
    bool WasMotionExecuted(MotionNames _motion, int timeLimit,bool facingRight);


    /*Input variables*/
    int INPUT_BUFFER_SIZE = 60;
    InputData inputCommand = {};
    std::vector<InputData> inputBuffer;
    int bufferIndex = -1;



};

