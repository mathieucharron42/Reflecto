#pragma once

namespace Reflecto
{
	// http://bloglitb.blogspot.com/2010/07/access-to-private-members-thats-easy.html
	// http://bloglitb.blogspot.com/2011/12/access-to-private-members-safer.html
	// http://www.gotw.ca/gotw/076.htm
	// http://cpp.kjx.cz/private_backdoor.html

	template<typename tag_t, typename tag_t::member_pointer_t M>
	struct EncapsulationBreaker
	{
		friend typename tag_t::member_pointer_t GetPrivateMemberPointer(tag_t)
		{
			return M;
		}

		friend typename auto GetPrivateMember(typename tag_t::object_t& object, tag_t)
		{
			tag_t::member_pointer_t memberPointer = GetPrivateMemberPointer(tag_t());
			return &(object.*memberPointer);
		}
	};

	template <typename object_type, typename member_t>
	struct TypeMemberTag
	{
		using object_t = object_type;
		using member_pointer_t = member_t object_type::*;
	};
}