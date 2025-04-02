#include "placement.h"
#include <iostream>
#include <ctime>
#include <cstdlib> /* 亂數相關函數 */

using namespace std;

int main(int argc, char *argv[])
{
    srand(time(NULL));
    Placement placement(argv[1], argv[2], argv[3], argv[4], argv[5]);
    placement.Run();
}