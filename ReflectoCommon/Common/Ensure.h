#pragma once

#include <assert.h>

#define ensure(expression)		\
	[&]							\
	{							\
		if(!!(expression))		\
		{						\
			return true;		\
		}						\
		else					\
		{						\
			assert(expression);	\
			return false;		\
		}						\
	}()
