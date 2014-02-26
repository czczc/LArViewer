#ifndef MCCHANNEL_H
#define MCCHANNEL_H

#include <vector>

class MCChannel {
public:
    int channelNo;
    int Nwires;
    std::vector<int> tpcs;
    std::vector<int> planes;
    std::vector<int> wires;

    MCChannel();
    virtual ~MCChannel();

    //  methods
    void PrintInfo();
};

#endif