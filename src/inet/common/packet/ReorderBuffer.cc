//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "inet/common/packet/ReorderBuffer.h"

namespace inet {

bit ReorderBuffer::getAvailableDataLength() const
{
    if (regions.size() > 0 && regions[0].offset == expectedOffset)
        return regions[0].data->getChunkLength();
    else
        return bit(0);
}

/**
 * Returns the largest next available data chunk starting at the expected
 * offset. The returned data chunk is automatically removed from the buffer.
 * If there's no available data at the expected offset, then it returns a
 * nullptr and the buffer is not modified.
 */
const Ptr<const Chunk> ReorderBuffer::popAvailableData(bit length)
{
    if (regions.size() > 0 && regions[0].offset == expectedOffset) {
        auto data = regions[0].data;
        bit dataLength = data->getChunkLength();
        if (length != bit(-1)) {
            data = data->peek(Chunk::ForwardIterator(bit(0)), length);
            dataLength = length;
        }
        clear(expectedOffset, dataLength);
        expectedOffset += dataLength;
        return data;
    }
    else
        return nullptr;
}

} // namespace inet
