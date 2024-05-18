#pragma once

#include <vector>

namespace Utilities::Compression {

    std::vector<u_int8_t> compressRunLengthEncoding(const u_int8_t* data, int size, int bytesPerPixel = 4) {
        std::vector<u_int8_t> compressed { };
        int i = 0;
        while (i < size) {
            u_int8_t value = data[i];
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

    void decompressRunLengthEncoding(const std::vector<u_int8_t>& compressed, u_int8_t* data, int size) {
        int index = 0;
        int dataIndex = 0;
        while (index < compressed.size() && dataIndex < size) {
            u_int8_t value = compressed[index++];
            u_int8_t count = compressed[index++];
            for (int i = 0; i < count && dataIndex < size; ++i) {
                data[dataIndex++] = value;
            }
        }
    }

}
