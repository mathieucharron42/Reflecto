#pragma once

#include <cstdio>
#include <codecvt>
#include <iterator>
#include <locale>
#include <string>
#include <type_traits>

namespace StringExt
{
	template<typename string_t, typename collection_t, typename projection_t>
	string_t Join(const collection_t elems, const string_t& separator, projection_t proj)
	{
		using char_t = typename string_t::value_type;
		using StringStreamType = std::basic_stringstream<char_t, std::char_traits<char_t>, std::allocator<char_t>>;

		StringStreamType buffer;
		for (auto it = begin(elems); it != end(elems); ++it)
		{
			if (it != begin(elems))
			{
				buffer << separator;
			}
			buffer << proj(*it);
		}

		return buffer.str();
	}

	template<typename string_t, typename collection_t>
	string_t Join(const collection_t elems, const string_t& separator)
	{
		auto identityProj = [](const auto& elem) { return elem; };
		return Join(elems, separator, identityProj);
	}

	template<typename string_t, typename ... Args>
	std::enable_if_t<
		std::is_same_v<string_t, std::string>,
		std::size_t> Snprintf(typename string_t::value_type* buffer, std::size_t size, const string_t& format, Args ... args)
	{
		return std::snprintf(buffer, size, format.c_str(), args ...);
	}

	template<typename string_t, typename ... Args>
	std::enable_if_t<
		std::is_same_v<string_t, std::wstring>,
		std::size_t> Snprintf(typename string_t::value_type* buffer, std::size_t size, const string_t& format, Args ... args)
	{
		return _snwprintf(buffer, size, format.c_str(), args ...);
	}

	template<typename string_t, typename ... Args>
	string_t Format(const string_t& format, Args ... args)
	{
		using char_t = typename string_t::value_type;
		const std::size_t size = Snprintf<string_t>(nullptr, 0, format.c_str(), args ...)+1;
		std::unique_ptr<char_t[]> buf(new char_t[size]);
		Snprintf<string_t>(buf.get(), size, format.c_str(), args ...);
		return string_t(buf.get());
	}

	static std::wstring ToWstring(const std::string& str)
	{
		return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
	}
}