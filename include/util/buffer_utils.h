#pragma once

#include <vector>

namespace Anno { namespace BufferUtils {

/** Writes `data` to the end of the given buffer. */
void append(std::vector<char>& buf, const std::vector<char>& data);

/** Writes a value to the end of the given buffer. */
template <typename T>
void append(std::vector<char>& buf, const T& value)
{
    const char* data = reinterpret_cast<const char*>(&value);
    buf.insert(buf.end(), data, data + sizeof(T));
}

}}  // namespace Anno::BufferUtils
