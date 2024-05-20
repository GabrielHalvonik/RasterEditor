#pragma once

#include <vector>
#include <cstdint>

namespace Utilities::Compression {

    std::vector<uint8_t> compressRunLengthEncoding(const uint8_t* data, int size, int bytesPerPixel = 4) {
        std::vector<uint8_t> compressed { };
        int i = 0;
        while (i < size) {
            uint8_t value = data[i];
            int count = 1;
            while (i + count < size && data[i + count] == value && count < 255) {
                ++count;
            }
            compressed.push_back(value);
            compressed.push_back(count);
            i += count;
        }
        return compressed;
    }

    void decompressRunLengthEncoding(const std::vector<uint8_t>& compressed, uint8_t* data, int size) {
        int index = 0;
        int dataIndex = 0;
        while (index < compressed.size() && dataIndex < size) {
            uint8_t value = compressed[index++];
            uint8_t count = compressed[index++];
            for (int i = 0; i < count && dataIndex < size; ++i) {
                data[dataIndex++] = value;
            }
        }
    }

}
