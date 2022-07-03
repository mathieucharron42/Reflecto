#pragma once

#include "Utils/AreTrait.h"
#include "Utils/IdentityTransform.h"

#include <cstdio>
#include <codecvt>
#include <iterator>
#include <locale>
#include <string>
#include <sstream>
#include <vector>
#include <type_traits>

namespace Reflecto
{
	namespace StringExt
	{
		template<typename string_t>
		using char_t = typename string_t::value_type;

		template<typename string_t>
		using StringStreamType = std::basic_stringstream<char_t<string_t>, std::char_traits<char_t<string_t>>, std::allocator<char_t<string_t>>>;

		template<typename string_t, typename collection_t, typename projection_t>
		string_t Join(const collection_t elems, const string_t& separator, projection_t proj)
		{
				
			StringStreamType<string_t> buffer;
			for (auto it = begin(elems); it != end(elems); ++it)
			{
				if (it != begin(elems))
				{
					buffer << separator;
				}
				buffer << std::invoke(proj, *it);
			}

			return buffer.str();
		}

		template<typename string_t, typename collection_t>
		string_t Join(const collection_t elems, const string_t& separator)
		{
			const IdentityTransform projection;
			return Join(elems, separator, projection);
		}

		template<typename string_t>
		std::vector<string_t> Tokenize(const string_t& string, const string_t& delimiter)
		{
			std::vector<string_t> tokens;
			std::size_t start = 0U;
			std::size_t end = string.find(delimiter);
			while (end != std::string::npos)
			{
				const std::string token = string.substr(start, end - start);
				tokens.push_back(token);
				start = end + delimiter.length();
				end = string.find(delimiter, start);
			}
			const std::string token = string.substr(start, end);
			tokens.push_back(token);
			return tokens;
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
			static_assert(AreTrait<std::is_pod, Args...>::value, "All arguments must be pod");

			using char_t = typename string_t::value_type;
			// Compute required length
			const std::size_t size = Snprintf<string_t>(nullptr, 0, format.c_str(), args ...);
			// Format string
			string_t formatted(size, char_t{});
			Snprintf<string_t>(formatted.data(), size+1, format.c_str(), args ...);
			return formatted;
		}

		static std::wstring ToWstring(const std::string& str)
		{
			// Avoid using std::wstring_convert as deprecated in c++17
			// std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
			std::wstringstream s;
			s << str.c_str(); 
			return s.str();
		}

		template<typename string_t, typename collection_t, typename projection_t>
		std::enable_if_t<
			std::is_same_v<string_t, std::wstring>,
			std::wstring> StringifyCollection(const collection_t collection, projection_t proj)
		{
			string_t elements = Join<string_t>(collection, L",", proj);
			return Format<string_t>(L"{%s}", elements.c_str());
		}

		template<typename string_t, typename collection_t, typename projection_t>
		std::enable_if_t<
			std::is_same_v<string_t, std::string>,
			std::string> StringifyCollection(const collection_t collection, projection_t proj)
		{
			string_t elements = Join<string_t>(collection, ",", proj);
			return Format<string_t>("{%s}", elements.c_str());
		}

		template<typename string_t, typename collection_t, typename projection_t>
		string_t StringifyCollection(const collection_t collection)
		{
			const IdentityTransform projection;
			return StringifyCollection(collection, projection);
		}

		template<typename string_t>
		string_t ReplaceAll(string_t string, const string_t& from, const string_t& to)
		{
			size_t position = 0;
			while ((position = string.find(from, position)) != std::string::npos)
			{
				string.replace(position, from.length(), to);
				position += to.length();
			}
			return string;
		}
	}
}