/// @file Storage.cpp
/// @date 2019-02-10

#include "storage.h"

namespace kaspersky
{

// ----------------------------------------------------------------------------------------------------
Item::Item(const std::string& uid, const std::string& name, int price, const std::string& manufacturer)
    : m_uid(uid)
    , m_name(name)
    , m_price(price)
    , m_manufacturer(manufacturer)
{}

// ----------------------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const Item& item)
{
   return os << "(" << item.m_uid << ", " << item.m_name << ")";
}

// ----------------------------------------------------------------------------------------------------
Item& Item::operator=(const Item& rh)
{
    m_uid = rh.m_uid;
    m_name = rh.m_name;
    m_price = rh.m_price;
    m_manufacturer = rh.m_manufacturer;
    return *this;
}

// ----------------------------------------------------------------------------------------------------
void Storage::AddItem(const Item&& item)
{
    Lock guard(m_storageGuard);
    m_storage.emplace(item.m_uid, item);   // O(1):O(N)
    m_manufacturerIds[item.m_manufacturer] // O(1):O(M)
                     .emplace(item.m_uid); // O(log(I)), M <= I <= N
}

// ----------------------------------------------------------------------------------------------------
bool Storage::GetItem(const std::string& uid, Item& item) const
{
   Lock guard(m_storageGuard);
   auto itemIt = m_storage.find(uid); // O(1):O(N)
   const bool success = itemIt != m_storage.end();
   if (success)
   {
       item = itemIt->second;
   }
   return success;
}

// ----------------------------------------------------------------------------------------------------
void Storage::RemoveItem(const std::string& uid)
{
   Lock guard(m_storageGuard);

   Item item;
   if (!GetItem(uid, item))  // O(1):O(N)
   {
      return;
   }

   auto idsIt = m_manufacturerIds.find(item.m_manufacturer); // O(1):O(N)
   const bool success = idsIt != m_manufacturerIds.end();
   if (!success)
   {
       return;
   }

   idsIt->second.erase(uid); // O(log(N))
   m_storage.erase(uid); // O(1):O(N)
}

// ----------------------------------------------------------------------------------------------------
ItemList Storage::GetItemsFrom(const std::string& manufacturer) const
{
   ItemList result;

   {
      Lock guard(m_storageGuard);
      auto uidListIt = m_manufacturerIds.find(manufacturer); // O(1):O(M)
      const bool success = uidListIt != m_manufacturerIds.end();
      if (success)
      {
         for (const auto& uid : uidListIt->second) // O(I)
         {
            result.emplace_back(m_storage.at(uid)); // O(log(N)), M <= I <= N
         }
      }
   }

   return std::move(result);
}

} // namespace kaspersky
