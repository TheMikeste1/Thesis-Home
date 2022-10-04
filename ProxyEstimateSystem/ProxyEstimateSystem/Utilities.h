#pragma once

#include <functional>
#include <vector>


template<class T>
void binaryInsert(std::vector<T>& orderedVector, const T& item,
                  std::function<bool(const T&, const T&)> comparator =
                  [](const T& a, const T& b) {
                     return a < b;
                  }
)
{
   auto it = std::lower_bound(orderedVector.begin(), orderedVector.end(), item,
                              comparator);
   orderedVector.insert(it, item);
}
