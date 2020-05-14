#pragma once
#include "mcpch.h"
#include "InputHandler.h"

namespace MC
{
	bool InputHandler::isKeyPressed(const MC_KEYS& key)
	{
		Key* aux = MC_INTERNAL::WindowInput::getKeys();

		for (uint8_t x = 0; x< MAX_KEYS-1; x++ )
		{
			if (aux[x].KeyCode != MC_KEYS::MC_NO_KEY)
				if(aux[x].KeyCode == key)
					return aux[x].isPressed;
		}

		return false;
	}

	bool InputHandler::isKeyHeld(const MC_KEYS& key)
	{
		Key* aux = MC_INTERNAL::WindowInput::getKeys();

		for (uint8_t x = 0; x < MAX_KEYS-1; x++)
		{
			if (aux[x].KeyCode != MC_KEYS::MC_NO_KEY)
				if (aux[x].KeyCode == key)
					return aux[x].isRepeated;
		}

		return false;
	}

	MC::vec2 InputHandler::GetMouseCoords()
	{
		vec2* Coords = MC_INTERNAL::WindowInput::getMouseCoords();

		return { Coords->x, Coords->y };
	}

	MC::vec2 InputHandler::GetMouseDelta()
	{
		vec2 Delta = MC_INTERNAL::WindowInput::getMouseDelta();

		return Delta;
	}

	void InputHandler::setMouseCoords(vec2 coords)
	{
		MC_INTERNAL::WindowInput::setMouseCoords(coords);
	}

	void InputHandler::setMouseDelta(vec2 value)
	{
		MC_INTERNAL::WindowInput::setMouseDelta(value);
	}

	void InputHandler::lockCursorPosition(bool lock)
	{
		MC_INTERNAL::WindowInput::lockCursor(lock);
	}

	void InputHandler::lockCursorDelta(bool lock)
	{
		MC_INTERNAL::WindowInput::lockDelta(lock);
	}

}