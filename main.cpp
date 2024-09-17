#include <iostream>
#include <windows.h>
#include <atomic>
#include <random>
#include <ctime>

HHOOK hMHook;
int x, xs[2];
int y, ys[2];
std::atomic<bool> running(true);
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis_1(0, 20);
std::uniform_int_distribution<> dis_2(-5, 5);
std::uniform_int_distribution<> dis_3(300, 700);
std::uniform_int_distribution<> dis_4(100, 500);
std::uniform_int_distribution<> dis_5(20, 150);
std::uniform_int_distribution<> dis_6(0, 120);

int tip = 0;

LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        if (wParam == WM_LBUTTONDOWN) {
            std::cout << "Mouse left button clicked!" << std::endl;
            POINT p;
            if (GetCursorPos(&p)) {
                x = p.x;
                y = p.y;
            }
            PostQuitMessage(0);
            return 1;
        }
    }
    return CallNextHookEx(hMHook, nCode, wParam, lParam);
}

void moveMouseRelative(int dx, int dy) {

    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;

    input.mi.dx = dx;
    input.mi.dy = dy;
    input.mi.dwExtraInfo = 0;

    SendInput(1, &input, sizeof(INPUT));
}

void clickMouse() {
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(INPUT));

    Sleep(100 + dis_1(gen) - 10 - dis_2(gen));

    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
    switch(tip){
        case 0:
            Sleep(dis_5(gen));break;
        case 1:
            Sleep(dis_4(gen));break;
        case 2:
            Sleep(dis_3(gen));break;
    }
}

int main() {
    std::cout<<"start!"<<std::endl;
    for (size_t i = 0; i < 2; i++)
    {
        hMHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, NULL, 0);
        if (hMHook == NULL) {
            std::cerr << "Failed to install mouse hook!" << std::endl;
            return 1;
        }

        MSG mMsg;
        while (GetMessage(&mMsg, NULL, 0, 0)) {
            TranslateMessage(&mMsg);
            DispatchMessage(&mMsg);
        }
        UnhookWindowsHookEx(hMHook);
        xs[i] = x;
        ys[i] = y;
        std::cout<<"捕获位置:"<< x << "||" << y << std::endl;
    }

    std::uniform_int_distribution<> dis_x(xs[0], xs[1]);
    std::uniform_int_distribution<> dis_rx(-(xs[1] - xs[0]) / 20,(xs[1] - xs[0]) / 20);
    std::uniform_int_distribution<> dis_y(ys[0], ys[1]);
    std::uniform_int_distribution<> dis_ry(-(ys[1] - ys[0]) / 20,(ys[1] - ys[0]) / 20);
         
    int oriX = dis_x(gen);
    int oriY = dis_y(gen);
    for (size_t i = 0; i < 3; i++)
    {
        std::cout<<3-i << "...." << std::endl;
        Sleep(1000);
    }
    SetCursorPos(oriX, oriY);


    std::time_t currentTime_2 = std::time(nullptr);
    std::tm localTime_2 = *std::localtime(&currentTime_2);
    std::tm* nextTime = &localTime_2;
    std::cout<< nextTime->tm_min << ":" << nextTime->tm_sec << std::endl;
    nextTime->tm_sec += dis_1(gen) + 15;
    std::mktime(nextTime);
    std::cout<< nextTime->tm_min << ":" << nextTime->tm_sec << std::endl;

    while (running) {
        std::time_t currentTime = std::time(nullptr);
        std::tm* localTime = std::localtime(&currentTime);
        std::time_t currentTimeT = std::mktime(localTime);
        std::time_t nextTimeT = std::mktime(nextTime);
        if(currentTimeT > nextTimeT){
            int d1 = dis_1(gen), d2 = dis_2(gen);
            currentTime_2 = std::time(nullptr);
            localTime_2 = *std::localtime(&currentTime_2);
            std::cout<< d2 << "||" << d1<< std::endl;
            if(d2 < 0){
                std::cout<<"tip = 1"<<std::endl;
                tip = 1;
                nextTime = &localTime_2;
                nextTime->tm_sec += dis_1(gen) + 5;
                std::mktime(nextTime);
            }else if(d1 > d2 + 5){
                std::cout<<"tip = 0"<<std::endl;
                tip = 0;
                nextTime = &localTime_2;
                nextTime->tm_sec += dis_1(gen) + 5;
                std::mktime(nextTime);
            }else{
                std::cout<<"tip = 2"<<std::endl;
                tip = 2;
                nextTime = &localTime_2;
                nextTime->tm_sec += dis_1(gen) + 5;
                std::mktime(nextTime);
            }
        std::cout<< nextTime->tm_min << ":" << nextTime->tm_sec << std::endl;
        }
        
        // std::cout<< oriX << "||" << oriY <<std::endl;
        int rx, ry;
        POINT p;
        int x, y;
        if (GetCursorPos(&p) &&( p.x < xs[0] || p.x > xs[1] || p.y < ys[0] || p.y > ys[1])) {
            oriX = p.x;
            oriY = p.y;
        }   
        do{
            rx = dis_rx(gen);
            ry = dis_ry(gen);
            }while(
            oriX + rx < xs[0] || 
            oriX + rx > xs[1] ||
            oriY + ry < ys[0] ||
            oriY + ry > ys[1]);
        oriX += rx;
        oriY += ry;
        moveMouseRelative(rx, ry);
        clickMouse();
    }

    return 0;
}