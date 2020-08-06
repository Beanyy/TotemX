#ifndef DATADICT_HPP
#define DATADICT_HPP

class DataDict
{
private:
    static const int kMaxDataSize = 15;
    struct Data
    {
        char cmd;
        char size;
        void *data;
    };
    Data allData[kMaxDataSize];

public:
    DataDict();
    void AddEntry(char cmd, char size, void *data);
    char GetIndex(char cmd);
    void SetEntry(char cmd, void *data);
};
#endif