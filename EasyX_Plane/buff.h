#ifndef BUFF_H
#define BUFF_H

class Buff {
public:
    int startTime;
    int endTime;
    int type;
    bool isActive;
    Buff(int start, int end, int type);
};

#endif