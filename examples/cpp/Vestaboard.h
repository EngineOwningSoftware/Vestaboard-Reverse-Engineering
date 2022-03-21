#include <string>
#include <cstdint>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cstring>

class Vestaboard
{
public:
    Vestaboard(std::string serialPortName);
    
    bool open();

    bool setCol(uint8_t colId, uint8_t* colData);
    bool armCol(uint8_t colId);

    bool go();

    void clear();
private:
    void L1_sendPacket(uint8_t* payload, uint16_t size);
    bool L2_sendPacket(uint16_t addr, uint8_t flags, uint8_t* payload, uint16_t payloadSize);
    bool L2_sendCommand(uint8_t addr, uint8_t commandId);

    void setRTS(bool value);

    std::string m_portName;
    int m_serialPort;
};
