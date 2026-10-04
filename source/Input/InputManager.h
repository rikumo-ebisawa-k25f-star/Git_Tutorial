#pragma once

enum class eInputState : unsigned char
{
	eNone,
	ePressed,
	eReleased,
	eHeld,
};

class InputManager final
{
private:
	static const int key_max = 256;
	static const int button_max = 16;

private:
	static char now_key[key_max];
	static char old_key[key_max];
	static unsigned char now_button[button_max];
	static unsigned char old_button[button_max];

private:
	InputManager() = default;
	~InputManager() = default;

public:
	static void Update();
	static eInputState GetKeyState(int key);
	static eInputState GetButtonState(int button);

private:
	static bool CheckRange(int value, int max_value);

};
