#include <iostream>
#include <random>
#include <windows.h>
int main(){
    for (size_t i = 0; i < 2000; i++)
    {
        Sleep(100);
        POINT p;
        int x, y;
        if (GetCursorPos(&p)) {
            x = p.x;
            y = p.y;
        }   
        std::cout<< x << "||" << y <<std::endl;
        /* code */
    }
    
}
