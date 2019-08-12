#include <stdio.h>

class MyTest{
public:
    MyTest();
    ~MyTest();
    bool CheckIfSuccess(){return successful;}
private:
    bool successful = false;
}