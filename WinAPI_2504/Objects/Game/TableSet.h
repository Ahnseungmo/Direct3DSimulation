#pragma once

class TableSet : public Item {
public:
    vector<Item*> items;  // 테이블 위의 개별 아이템들

    TableSet() {
        id = "table_set";
        name = "Table Set";
        stackable = false;
    }

    void AddItem(Item* item) {
        items.push_back(item);
    }

    void Use() override {
        // 테이블 전체를 놓는 로직
    }
};