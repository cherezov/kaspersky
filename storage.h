/// @file Storage.h
/// @date 2019-02-10

#pragma once

#include <iostream>
#include <string>
#include <set>
#include <list>
#include <unordered_map>
#include <mutex>

namespace kaspersky
{

// Forward declaration
struct Item;
using ItemList = std::list<Item>;

/// @brief Represents a storage item
struct Item
{
   Item() = default;
   Item(const Item&) = default;
   Item(Item&&) = default;

   /// @brief .ctor
   /// @param uid - unique id of the item
   /// @param name - item name
   /// @param price - price of the item
   /// @param manufacturer - item manufacturer name
   Item(const std::string& uid, const std::string& name, int price, const std::string& manufacturer);

   /// @brief operator=
   Item& operator=(const Item& rh);

   /// @name Members
   /// @{
   std::string m_uid;   ///< Unique item id.
   std::string m_name;  ///< Item name
   int m_price;         ///< Item price
   std::string m_manufacturer; ///< Item manufacturer name.
   /// @}
}; // struct Item

/// @brief Helper method
std::ostream& operator<<(std::ostream& os, const Item& item);

/// @biref Class represents storage with all required operations
class Storage final
{
public:
   /// @brief .ctor
   Storage() = default;

   /// @brief Add item to the storage
   /// @param item - item to add
   /// @note Complexity average: O(log(N))
   ///                      max: O(N)
   void AddItem(const Item&& item);

   /// @brief Get item from the strorage
   /// @param uid - unique id of the item to get
   /// @param item - [out] requested item
   /// @return true if item found, false otherwise
   /// @note Complexity average: O(1)
   ///                      max: O(N)
   bool GetItem(const std::string& uid, Item& item) const;

   /// @brief Remove item from the storage
   /// @param uid - unique id of the item to get
   /// @note Complexity average: O(log(N))
   ///                      max: O(N)
   void RemoveItem(const std::string& uid);

   /// @brief Get all items from manufacturer
   /// @param manufacturer - manufacturer name to get items for
   /// @return List of items
   /// @note Complexity average: O(log(N))
   ///                      max: O(N)
   ItemList GetItemsFrom(const std::string& manufacturer) const;

   /// @brief Storage size
   int Size() const { return m_storage.size(); }

private:
   /// @brief Map storage item to unique id
   using ItemByIdMap= std::unordered_map<std::string, Item>;
   ItemByIdMap m_storage; ///< Main storage

   /// @brief Helper storage to map manufacturer and all its item ids
   using IdsList = std::set<std::string>;
   using ManufacturerIdsMap = std::unordered_map<std::string, IdsList>;
   ManufacturerIdsMap m_manufacturerIds; ///< Helper storage

   /// @brief Guard
   using Lock = std::unique_lock<std::recursive_mutex>;
   mutable std::recursive_mutex m_storageGuard;
}; // class Storage

} // namespace kaspersky
