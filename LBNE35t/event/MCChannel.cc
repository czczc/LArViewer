#include "MCChannel.h"

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

MCChannel::MCChannel()
{
}

MCChannel::~MCChannel()
{
}

void MCChannel::PrintInfo()
{
    cout << "Channel " << channelNo << "\n";
    cout << "Nwires  " << Nwires << "\n";
    cout << "TPC ";
    for (int i=0; i<Nwires; i++) {
        cout << tpcs.at(i) << " ";
    }
    cout << "\n";

    cout << "Plane ";
    for (int i=0; i<Nwires; i++) {
        cout << planes.at(i) << " ";
    }
    cout << "\n";

    cout << "Wire ";
    for (int i=0; i<Nwires; i++) {
        cout << wires.at(i) << " ";
    }
    cout << "\n";

    cout << endl;
}

