#pragma once
class TableSet : public Item
{
public:
    TableSet() : Item("TableSet")
    {
        AddPart("SM_Table_1", { 0,0,0 }, {}, { 1,1,1 });
        AddPart("SM_Chair_1", { 1,0,1 }, {}, { 1,1,1 });
        AddPart("SM_Chair_1", { -1,0,1 }, {}, { 1,1,1 });
        AddPart("SM_Chair_1", { 1,0,-1 }, {}, { 1,1,1 });
        AddPart("SM_Chair_1", { -1,0,-1 }, {}, { 1,1,1 });
    }
};