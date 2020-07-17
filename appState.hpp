#ifndef APPSTATE_HPP
#define APPSTATE_HPP

class AppState
{
private:
    static const int nModifiers = 10;
    struct Modifier
    {
        char cmd;
        char size;
        void *value;
    };
    Modifier modifiers[nModifiers];

public:
    AppState();
    CHSV color;
    int animation;
    int speed;
    char off;
    char pause;
    char colorOverride;
    char motionOverride;
    void Update(char *cmd);
    void Reset();
};
#endif