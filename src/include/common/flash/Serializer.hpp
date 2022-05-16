#pragma once

#include <stdint.h>
#include <array>
#include <concepts>
#include <functional>
#include <numeric>
#include <span>
#include <bit>
#include <string_view>
#include "common.hpp"

namespace flash
{

/**
 * \class Serializer
 * \brief CRTP helper for serializiation
 *
 * This class is a CRTP class for serialization of data.
 * The problem with just doing
 * \code
 * memcpy(buffer, &struct,sizeof(struct));
 * \endcode
 * is that it do not regard the memory alignment and padding.
 * There problem with the above code is that memory padding
 * is compiler specific, which means it can be changed. Padding
 * also has the property of adding extra bytes if not constructed
 * properly.
 *
 * This class will do serialization and compress everything into
 * a byte buffer, this can then later be changed for example to
 * a some other size buffer (32-bit) (@see get_word_buffer())
 *
 * How to use it, lets do an example of an easy struct.
 * \code
 * struct A {
 *  uint32_t aa{};
 *  uint16_t an{}
 *  void do_span_serialize(serial_span_t buffer)
 *  static MagicVersion do_deserialize(serial_span_t const buffer)
 * };
 * \endcode
 * Both of the "do_" functions needs to be implemented. And to serialize.
 * \code
 *  b.span_serialize();
 *  for( const auto& item : b.get_buffer() )
 *  {
 *       std::cout << std::hex << static_cast<int>(item) << "\n";
 *  }
 *  \endcode
 *
 *  There are 3 important methods to remeber
 *  - span_serialize
 *  - deserialize
 *  - get_buffer
 *
 *  For more information checkout the unit tests.
 */

template <typename T, size_t BYTE_BUFFER_SIZE>
struct Serializer
{

    constexpr static size_t WORD_SIZE =
        BYTE_BUFFER_SIZE / sizeof(uint32_t) + (BYTE_BUFFER_SIZE % sizeof(uint32_t) != 0 ? 1 : 0);
    constexpr static size_t BYTE_SIZE = WORD_SIZE * sizeof(uint32_t);
    using buffer_t                    = std::array<uint8_t, BYTE_SIZE>;
    using serial_span_t               = std::span<uint8_t>;
    using word_buffer                 = std::array<uint32_t, WORD_SIZE>;

    static buffer_t serialized_buffer;

    void span_serialize()
    {
        static_cast<T*>(this)->do_span_serialize(serial_span_t(serialized_buffer));
    }

    T deserialize()
    {

        serial_span_t sp{serialized_buffer};
        return static_cast<T*>(this)->do_deserialize(sp);
    }

    serial_span_t get_buffer() const noexcept
    {
        return std::span(serialized_buffer.begin(), BYTE_SIZE);
    }

    word_buffer get_word_buffer() const noexcept
    {
        return std::bit_cast<std::array<uint32_t, WORD_SIZE>>(serialized_buffer);
    }

    static void set_buffer(buffer_t buffer) { serialized_buffer = buffer; }
    static void set_buffer(word_buffer buffer) { set_buffer(std::bit_cast<buffer_t>(buffer)); }
};

template <typename T, size_t BYTE_BUFFER_SIZE>
Serializer<T, BYTE_BUFFER_SIZE>::buffer_t Serializer<T, BYTE_BUFFER_SIZE>::serialized_buffer{};
}  // namespace flash
