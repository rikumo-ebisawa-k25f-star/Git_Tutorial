#include "InputManager.h"
#include <DxLib.h>

char InputManager::now_key[256] = {};
char InputManager::old_key[256] = {};
unsigned char InputManager::now_button[16] = {};
unsigned char InputManager::old_button[16] = {};

void InputManager::Update()
{
	memcpy(old_key, now_key, (sizeof(char) * 256));
	memcpy(old_button, now_button, (sizeof(char) * 16));

	GetHitKeyStateAll(now_key);

	XINPUT_STATE controller = {};
	GetJoypadXInputState(DX_INPUT_PAD1, &controller);
	memcpy(now_button, controller.Buttons, (sizeof(char) * 16));

}

eInputState InputManager::GetKeyState(int key)
{
	if (CheckRange(key, key_max))
	{
		if (old_key[key] == TRUE)
		{
			if (now_key[key] == TRUE)
			{
				return eInputState::eHeld;
			}
			else
			{
				return eInputState::eReleased;
			}
		}
		else
		{
			if (now_key[key] == TRUE)
			{
				return eInputState::ePressed;
			}
		}
	}
	return eInputState::eNone;
}

eInputState InputManager::GetButtonState(int button)
{
	if (CheckRange(button, button_max))
	{
		if (old_button[button] == TRUE)
		{
			if (now_button[button] == TRUE)
			{
				return eInputState::eHeld;
			}
			else
			{
				return eInputState::eReleased;
			}
		}
		else
		{
			if (now_button[button] == TRUE)
			{
				return eInputState::ePressed;
			}
		}
	}
	return eInputState::eNone;
}

bool InputManager::CheckRange(int value, int max_value)
{
	return (0 <= value && value < max_value);
}
