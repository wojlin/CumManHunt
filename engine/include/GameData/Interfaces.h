using namespace std;

#ifndef INTERFACES_H
#define INTERFACES_H

class baseResourceWAD
{
    public:
        virtual void printInfo(){}
};


class baseSound
{
    public:
        virtual void printInfo(){}

        virtual void play(){}
};

#endif // INTERFACES_H