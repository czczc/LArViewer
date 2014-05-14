#include "MCChannel.h"

#include <iostream>

using namespace std;

MCChannel::MCChannel(int c, int p, int w)
{
    SetChannel(c, p, w);
}

MCChannel::~MCChannel()
{
}

void MCChannel::SetChannel(int c, int p, int w)
{
    channelNo = c;
    plane = p;
    wire = w;
    hash = Encode(plane, wire);
}


int MCChannel::Encode(int plane, int wire) { return plane*100000+wire; }

void MCChannel::Decode(int hash, int& plane, int& wire)
{
    plane = hash / 100000;
    wire  = hash - plane*100000;
}

void MCChannel::PrintInfo()
{
    cout << channelNo << "\t" << plane << "\t" << wire << "\t" << hash << endl;
}
