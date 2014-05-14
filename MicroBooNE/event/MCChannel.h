#ifndef MCCHANNEL_H
#define MCCHANNEL_H

class MCChannel {
public:
    int channelNo;
    int plane;
    int wire;
    int hash;

    MCChannel(int c=-1, int p=-1, int w=-1);
    virtual ~MCChannel();

    //  methods
    void SetChannel(int c, int p, int w);
    static int Encode(int plane, int wire); 
    static void Decode(int hash, int& plane, int& wire);
    void PrintInfo();
};

#endif
