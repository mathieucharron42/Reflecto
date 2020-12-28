#pragma once

#include "Definitions.h"

namespace Platform
{
	bool IsLittleEndian()
	{
		static short int number = 0x1;
		static byte* ptr = reinterpret_cast<byte*>(&number);
		return (ptr[0] == 1);
	}
}