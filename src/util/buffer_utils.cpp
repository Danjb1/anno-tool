#include "util/buffer_utils.h"

namespace Anno { namespace BufferUtils {

void append(std::vector<char>& buf, const std::vector<char>& data)
{
    buf.insert(buf.end(), data.begin(), data.end());
}

}}  // namespace Anno::BufferUtils
