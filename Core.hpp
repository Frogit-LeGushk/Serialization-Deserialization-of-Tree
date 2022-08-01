#pragma once
#include "Buffer.hpp"


namespace Core {

    /** loading data into buffer in binary representation **/
    inline void encode(int8_t value, Buffer & buffer) {
        buffer.push8bit((uint8_t)value);
    }
    inline void encode(int16_t value, Buffer & buffer) {
        encode((int8_t)(value >> 8), buffer);
        encode((int8_t)(value), buffer);
    }
    inline void encode(int32_t value, Buffer & buffer) {
        encode((int16_t)(value >> 16), buffer);
        encode((int16_t)(value), buffer);
    }
    inline void encode(int64_t value, Buffer & buffer) {
        encode((int32_t)(value >> 32), buffer);
        encode((int32_t)(value), buffer);
    }
    inline void encode(float value, Buffer & buffer) {
        encode(*(int32_t *)(&value), buffer);
    }
    inline void encode(double value, Buffer & buffer) {
        encode(*(int64_t *)(&value), buffer);
    }


    /** retrieving data from buffer in binary representation **/
    inline void decode(int8_t * value, Buffer & buffer) {
        (*value) =  ((int8_t)buffer[buffer.next()]);
    }
    inline void decode(int16_t * value, Buffer & buffer) {
        *value = 0;
        decode((int8_t *)value + 1, buffer);
        decode((int8_t *)value, buffer);
    }
    inline void decode(int32_t * value, Buffer & buffer) {
        *value = 0;
        decode((int16_t *)value + 1, buffer);
        decode((int16_t *)value, buffer);
    }
    inline void decode(int64_t * value, Buffer & buffer) {
        *value = 0;
        decode((int32_t *)value + 1, buffer);
        decode((int32_t *)value, buffer);
    }
    inline void decode(float * value, Buffer & buffer) {
        decode((int32_t *)value, buffer);
    }
    inline void decode(double * value, Buffer & buffer) {
        decode((int64_t *)value, buffer);
    }

}
