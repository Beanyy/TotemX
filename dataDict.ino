#include <string.h>
#include "dataDict.hpp"

DataDict::DataDict() {
    memset(allData, 0, sizeof(allData));
}

void DataDict::AddEntry(char cmd, char size, void *data)
{
    for (int i = 0; i < kMaxDataSize; i++)
    {
        if (allData[i].cmd == 0 || allData[i].cmd == cmd) {
            allData[i].cmd = cmd;
            allData[i].size = size;
            allData[i].data = data;
            return;
        }
    }
}

char DataDict::GetIndex(char cmd)
{
    for (int i = 0; i < kMaxDataSize; i++)
        if (allData[i].cmd == cmd)
            return i;
    return -1;
}

void DataDict::SetEntry(char cmd, void *data)
{
    char idx = GetIndex(cmd);
    if (idx >=0)
        memcpy(allData[idx].data, data, allData[idx].size);
}