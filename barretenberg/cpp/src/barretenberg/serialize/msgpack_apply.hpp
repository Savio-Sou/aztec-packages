#pragma once

#include "msgpack.hpp"
#include "msgpack_impl/drop_keys.hpp"

namespace msgpack {
/**
 * @brief Helper method for better error reporting. Clang does not give the best errors for lambdas.
 */
template <msgpack_concepts::HasMsgPack T> void msgpack_apply(const auto& func, auto&... args)
{
    std::apply(func, msgpack::drop_keys(std::tie(args...)));
}
/**
 * @brief Applies a function to the values exposed by the msgpack method.
 * @param value The value whose fields to reflect over.
 * @param func The function to call with each field as an argument.
 */
template <msgpack_concepts::HasMsgPack T> void msgpack_apply(const T& value, const auto& func)
{
    // We must use const_cast as our method is meant to be polymorphic over const, but there's no such concept in C++
    const_cast<T&>(value).msgpack([&](auto&... args) { // NOLINT
        msgpack_apply<T>(func, args...);
    });
}
} // namespace msgpack
