#pragma once

#include <vector>
#include <memory>
#include <set>
#include <unordered_map>

namespace ECS {

namespace Types {

struct None {};

template <typename T>
using SPtr      = std::shared_ptr<T>;

template <typename T>
using SetPtr     = std::unique_ptr<std::set<T>>;

template <typename T, typename G>
using UMap       = std::unordered_map<T, SPtr<G>>;

template <typename T, typename G>
using UMapVec    = std::unordered_map<T, std::vector<SPtr<G>>>;

template <typename T, typename G>
using UMapPtr    = std::unique_ptr<UMap<T, G>>;

template <typename T, typename G>
using UMapVecPtr = std::unique_ptr<UMapVec<T, G>>;

template <typename T>
using VecSPtr    = std::shared_ptr<std::vector<SPtr<T>>>;

} // namespace Types

} // namespace ECS
