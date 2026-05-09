#pragma once
#include "Component.h"
#include "Item.h"
#include <vector>

namespace RogaliqueGame {
class InventoryComponent : public MyEngine::Component{
   public:
    InventoryComponent(MyEngine::GameObject* gameObject)
        : Component(gameObject) {}

    void Update(float) override {}
    void Render() override {}

    void AddItem(const Item& item) { items.push_back(item); }
    void RemoveItem(const std::string& name){
        items.erase(std::remove_if(items.begin(), items.end(), 
            [&](const Item& i) { return i.name == name; }), items.end());
    }
    const std::vector<Item>& GetItems() const { return items; }

   private:
    std::vector<Item> items;
};
}
