#pragma once

#include "Common/Definitions.h"
#include "Type/TypeDescriptorType.h"
#include "Serialization/TextSerialization.h"
#include "Serialization/Writer/ISerializationWriter.h"
#include "Serialization/Writer/SerializationWriterTypeTrait.h"

#include <stack>
#include <string>
#include <sstream>
#include <vector>

namespace Reflecto
{
	namespace Serialization
	{
		class JsonSerializationWriter
		{
		public:
			void WriteBeginObject()
			{
				_bufferStack.emplace();
				_bufferStack.top().WriteBeginObject();
			}

			void WriteEndObject()
			{
				_bufferStack.top().WriteEndObject();
				_bufferStack.pop();
			}

			void Transpose(std::vector<byte>& bytes)
			{
				//TextSerialization::Serialize(_buffer.str(), bytes);
			}

		private:
			class JsonBuffer
			{
			public:
				void WriteBeginObject()
				{
					_buffer << "{";
				}

				void WriteObjectContent(const JsonBuffer& objectBuffer)
				{
					_buffer << objectBuffer.ToString();
				}

				void WriteEndObject()
				{
					_buffer << "}";
				}

				std::string ToString() const
				{
					return _buffer.str();
				}

			private:
				std::stringstream _buffer;
			};

			std::stack<JsonBuffer> _bufferStack;
		};

	}
}