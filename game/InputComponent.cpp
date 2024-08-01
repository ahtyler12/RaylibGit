#include "InputComponent.h"

InputComponent::InputComponent()
{
    inputBuffer.resize(INPUT_BUFFER_SIZE);
}

bool InputComponent::wasInputPressedOnFrame(InputTypes inputToCheck, int frame, bool facingRight)
{
    const auto _bufferIndex = frame % inputBuffer.size();
    const auto _lastBufferIndex = (inputBuffer.size() + frame - 1) % inputBuffer.size();

    const InputData currentInput = inputBuffer.at(_bufferIndex);
    const InputData lastInput = inputBuffer.at(_lastBufferIndex);

    const bool _leftCheck = currentInput.left && !lastInput.left;
    const bool _rightCheck = currentInput.right && !lastInput.right;

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
    case InputTypes::RIGHT:
        Pressed = _rightCheck;
        break;
    case InputTypes::LEFT:
        Pressed = _leftCheck;
        break;
    case  InputTypes::FORWARD:
    {
        if (facingRight)
            Pressed = _rightCheck;
        else
            Pressed = _leftCheck;
        break;
    }
    case  InputTypes::BACKWARD:
    {
        if (facingRight)
            Pressed = _leftCheck;
        else
            Pressed = _rightCheck;
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

bool InputComponent::wasInputPressed(InputTypes inputToCheck, bool facingRight)
{
    const InputData currentInput = GetCurrentInputCommand();
    const InputData lastInput = GetLastInputCommand();
    bool Pressed = false;

    const bool _leftCheck = currentInput.left && !lastInput.left;
    const bool _rightCheck = currentInput.right && !lastInput.right;

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
    case InputTypes::RIGHT:
        Pressed = _rightCheck;
        break;
    case InputTypes::LEFT:
        Pressed = _leftCheck;
        break;
    case  InputTypes::FORWARD:
    {
        if (facingRight)
            Pressed = _rightCheck;
        else
            Pressed = _leftCheck;
        break;
    }
    case  InputTypes::BACKWARD:
    {
        if (facingRight)
            Pressed = _leftCheck;
        else
            Pressed = _rightCheck;
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

void InputComponent::UpdateInput()
{
	bufferIndex = (bufferIndex + 1) % inputBuffer.max_size();
	inputBuffer.at(bufferIndex) = inputCommand;
}

InputData InputComponent::GetCurrentInputCommand()
{
    return inputBuffer.at(bufferIndex);
}

InputData InputComponent::GetLastInputCommand()
{
    return inputBuffer.at((inputBuffer.max_size() + bufferIndex - 1) % inputBuffer.max_size());
}

bool InputComponent::WasInputPressBuffered(InputTypes _input, int duration, bool facingRight)
{
    int i = 0;
    while (i < duration)
    {
        if (wasInputPressedOnFrame(_input, (inputBuffer.size() - bufferIndex - i), facingRight))
        {
            return true;
        }
        i += 1;
    }
    return false;
}

bool InputComponent::WasMotionExecuted(MotionNames _motion, int timeLimit, bool facingRight)
{
    int adjustedLimit = timeLimit;

    if (adjustedLimit > (INPUT_BUFFER_SIZE + bufferIndex))
    {
        adjustedLimit = INPUT_BUFFER_SIZE + bufferIndex;
    }

    int currentMotionIndex = 0;
    
    std::vector<int> MotionList;
    
    for (int i = 0; i < sizeof(MotionInputs[_motion]); i++)
    {
        MotionList.push_back(MotionInputs[_motion][i]);
    }

    for (int i = 0; i < adjustedLimit; i++)
    {
        const int _bufferPosition = ((inputBuffer.size() + bufferIndex) - (static_cast<unsigned long long>(adjustedLimit) - 1)) % inputBuffer.size();
        const auto _currentInput = inputBuffer.at(_bufferPosition);
        if (CheckNumpadDirection(_currentInput, MotionList[currentMotionIndex], facingRight))
        {
            currentMotionIndex += 1;
            if (currentMotionIndex > MotionList.size())
            {
                return true;
            }
        }
    }
    return false;
}

bool InputComponent::CheckNumpadDirection(InputData _input, int direction, bool facingRight)
{
    const bool back = (facingRight) ? _input.left : _input.right;
    const bool forward = (facingRight) ? _input.right : _input.left;
    
    switch (direction)
    {
    case 1:
        return back && _input.down;
    case 2:
        return _input.down && !(_input.right || _input.left);
    case 3:
        return forward && _input.down;
    case 4:
        return back && !(_input.up || _input.down);
    case 6:
        return forward && !(_input.up || _input.down);
    case 7:
        return back && _input.up;
    case 8:
        return _input.up;
    case 9:
        return forward && _input.up;
    default:
        return false;
        break;
    }
}
