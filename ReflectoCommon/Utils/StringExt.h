#pragma once

#include "Utils/AreTrait.h"
#include "Utils/IdentityTransform.h"

#include <cstdio>
#include <codecvt>
#include <iterator>
#include <locale>
#include <string>
#include <type_traits>

namespace Reflecto
{
	namespace Utils
	{
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
				const IdentityTransform projection;
				return Join(elems, separator, projection);
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
				return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
			}
		}
	}
}