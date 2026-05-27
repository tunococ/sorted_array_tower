#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <concepts>

namespace sorted_array_tower {

/**
 * @brief A template class for managing a sorted array with efficient lookups.
 * 
 * This data structure maintains elements in sorted order and provides
 * efficient insertion, deletion, and searching operations.
 * 
 * @tparam T The element type. Must support comparison operators.
 */
template <typename T>
requires std::totally_ordered<T>
class SortedArrayTower {
public:
    using value_type = T;
    using container_type = std::vector<T>;
    using size_type = typename container_type::size_type;
    using const_iterator = typename container_type::const_iterator;
    using iterator = typename container_type::iterator;

    /**
     * @brief Constructs an empty sorted array.
     */
    SortedArrayTower() = default;

    /**
     * @brief Constructs a sorted array from an initializer list.
     */
    SortedArrayTower(std::initializer_list<T> init) {
        for (const auto& elem : init) {
            insert(elem);
        }
    }

    /**
     * @brief Inserts an element while maintaining sorted order.
     * @param value The value to insert.
     */
    void insert(const T& value) {
        auto it = std::lower_bound(data_.begin(), data_.end(), value);
        data_.insert(it, value);
    }

    /**
     * @brief Inserts an element while maintaining sorted order (move semantics).
     * @param value The value to insert.
     */
    void insert(T&& value) {
        auto it = std::lower_bound(data_.begin(), data_.end(), value);
        data_.insert(it, std::move(value));
    }

    /**
     * @brief Removes the first occurrence of a value.
     * @param value The value to remove.
     * @return true if the element was found and removed, false otherwise.
     */
    bool remove(const T& value) {
        auto it = std::lower_bound(data_.begin(), data_.end(), value);
        if (it != data_.end() && *it == value) {
            data_.erase(it);
            return true;
        }
        return false;
    }

    /**
     * @brief Searches for an element in the array.
     * @param value The value to search for.
     * @return A const_iterator to the element if found, end() otherwise.
     */
    const_iterator find(const T& value) const {
        auto it = std::lower_bound(data_.begin(), data_.end(), value);
        if (it != data_.end() && *it == value) {
            return it;
        }
        return data_.end();
    }

    /**
     * @brief Checks if an element exists in the array.
     * @param value The value to check.
     * @return true if the element exists, false otherwise.
     */
    bool contains(const T& value) const {
        return find(value) != data_.end();
    }

    /**
     * @brief Returns the number of elements in the array.
     */
    size_type size() const noexcept {
        return data_.size();
    }

    /**
     * @brief Checks if the array is empty.
     */
    bool empty() const noexcept {
        return data_.empty();
    }

    /**
     * @brief Returns a const reference to the element at the specified index.
     * @throws std::out_of_range if the index is out of bounds.
     */
    const T& at(size_type index) const {
        return data_.at(index);
    }

    /**
     * @brief Clears all elements from the array.
     */
    void clear() noexcept {
        data_.clear();
    }

    /**
     * @brief Returns a const iterator to the beginning of the array.
     */
    const_iterator begin() const noexcept {
        return data_.begin();
    }

    /**
     * @brief Returns a const iterator to the end of the array.
     */
    const_iterator end() const noexcept {
        return data_.end();
    }

    /**
     * @brief Returns a const iterator to the beginning of the array.
     */
    const_iterator cbegin() const noexcept {
        return data_.cbegin();
    }

    /**
     * @brief Returns a const iterator to the end of the array.
     */
    const_iterator cend() const noexcept {
        return data_.cend();
    }

    /**
     * @brief Returns the minimum element in the array.
     * @throws std::out_of_range if the array is empty.
     */
    const T& min() const {
        if (empty()) {
            throw std::out_of_range("Array is empty");
        }
        return data_.front();
    }

    /**
     * @brief Returns the maximum element in the array.
     * @throws std::out_of_range if the array is empty.
     */
    const T& max() const {
        if (empty()) {
            throw std::out_of_range("Array is empty");
        }
        return data_.back();
    }

private:
    container_type data_;
};

} // namespace sorted_array_tower
