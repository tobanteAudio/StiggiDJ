
#pragma once

#include <juce_core/juce_core.h>

namespace ta
{

template<typename T>
auto fillArray(T const& obj, int count) -> juce::Array<T>
{
    auto array = juce::Array<T>{};
    array.ensureStorageAllocated(count);
    for (auto i{0}; i < count; ++i) { array.add(obj); }
    return array;
}

}  // namespace ta