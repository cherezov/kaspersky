/// @file main.cpp
/// @date 2019-02-10
/// @brief Kaspersky test task

#include "storage.h"

int main()
{
   kaspersky::Storage storage;
   storage.AddItem({"uid11", "item11", 100, "manufacture1"});
   storage.AddItem({"uid12", "item12", 200, "manufacture1"});
   storage.AddItem({"uid13", "item13", 300, "manufacture1"});
   storage.AddItem({"uid21", "item21", 10, "manufacture2"});
   storage.AddItem({"uid22", "item22", 20, "manufacture2"});
   storage.AddItem({"uid23", "item23", 30, "manufacture2"});

   assert(storage.Size() == 6);

   kaspersky::Item item;
   auto success = storage.GetItem("uid13", item);
   assert(success);
   assert(item.m_uid == "uid13");

   storage.RemoveItem("uid22");
   assert(storage.Size() == 5);

   success = storage.GetItem("uid22", item);
   assert(!success);

   auto items = storage.GetItemsFrom("manufacture2");
   assert(items.size() == 2);

   for (const auto& i : items)
   {
      assert(i.m_manufacturer == "manufacture2");
   }

   return 0;
}
