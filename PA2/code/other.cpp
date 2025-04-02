// g++ std-set.cpp -o a.out -std=c++11
#include <iostream>
#include <set>
#include <chrono>

using namespace std;

double random_float()
{

    return static_cast<double>(rand()) / RAND_MAX;
}

int main()
{
    // 開始計時
    auto start = std::chrono::steady_clock::now();

    // 迴圈開始
    int i = 0;
    // while (true)
    // {
    //     // 在此處放置你的迴圈程式碼

    //     // 檢查是否超過限制時間
    //     auto end = std::chrono::steady_clock::now();
    //     auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    //     if (elapsed >= 600000)
    //     { // 假設限制為 1000 毫秒（即 1 秒）
    //         cout << i << endl;
    //         std::cout
    //             << "Time limit exceeded!" << std::endl;
    //         break;
    //     }

    //     // 在此處更新迴圈條件
    //     ++i;
    // }

    return 0;
}