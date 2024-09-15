/*
 * MIT License
 *
 * Copyright (c) 2024 Giovanni Santini

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#pragma once

#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

namespace brenta
{

namespace ecs
{

namespace types
{

/**
 * @brief None type
 *
 * This type is used to represent a null value.
 */
struct none
{
};

template <typename T> using SPtr = std::shared_ptr<T>;

template <typename T> using SetPtr = std::unique_ptr<std::set<T>>;

template <typename T, typename G> using UMap = std::unordered_map<T, SPtr<G>>;

template <typename T, typename G>
using UMapVec = std::unordered_map<T, std::vector<SPtr<G>>>;

template <typename T, typename G> using UMapPtr = std::unique_ptr<UMap<T, G>>;

template <typename T, typename G>
using UMapVecPtr = std::unique_ptr<UMapVec<T, G>>;

template <typename T> using VecSPtr = std::shared_ptr<std::vector<SPtr<T>>>;

} // namespace types

} // namespace ecs

} // namespace brenta
