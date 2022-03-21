#include <stdio.h>
#include <unistd.h>

#include "Vestaboard.h"

int main(int argc, char* argv[])
{
    printf("[+] Loading Vestaboard Device-Level API!\n");
    if(!board.open())
    {
        printf("[!] Vestaboard::open(); failed!\n");
        return 0;
    }
  
    board.clear();
  
    usleep(1000000 * 10); // Wait 10 seconds
  
    uint8_t colRedGreen[] = {0x05, 0x02, 0x05, 0x02, 0x05, 0x02};
    for(int col = 1; col <= 22; col++)
    {
        board.setCol(col, colRedGreen);
        board.armCol(col);
    }
    board.go();
  
    return 0;
}
