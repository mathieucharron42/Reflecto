#pragma once

namespace Reflecto
{
	template <class type>
	struct BasicRelationalOperators 
	{
		friend bool operator<=(const type& a, const type& b) 
		{ 
			return a < b || a == b; 
		}
	
		friend bool operator>(const type& a, const type& b)
		{ 
			return !(a <= b); 
		}
	
		friend bool operator!=(const type& a, const type& b)
		{ 
			return !(a == b); 
		}
	
		friend bool operator>=(const type& a, const type& b)
		{ 
			return !(a < b); 
		}
	};

	template <class type>
	struct RelationalOperators : public BasicRelationalOperators<type>
	{
		friend bool operator==(const type& a, const type& b)
		{
			return !(a < b) && !(b < a);
		}
	};
}