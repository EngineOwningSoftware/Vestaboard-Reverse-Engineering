#include "Vestaboard.h"
#include "CRC.h"

Vestaboard::Vestaboard(std::string serialPortName) : m_portName(serialPortName)
{}

bool Vestaboard::open()
{
    // Connect to serial port
    printf("[+] Connecting to '%s'\n", m_portName.c_str());
    m_serialPort = ::open(m_portName.c_str(), O_RDWR|O_NOCTTY|O_LARGEFILE);
    if (m_serialPort < 0)
    {
        printf("[!] Failed to connect to '%s'\n", m_portName.c_str());
        return false;
    }
    printf("[+] Connected!\n");

    // Setup serial params
    struct termios tty;
    if(tcgetattr(m_serialPort, &tty) != 0)
    {
        printf("[!] Failed to read config from serial port\n");
        return false;
    }

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CRTSCTS;

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ECHONL;
    tty.c_lflag &= ~ISIG;
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

    tty.c_oflag &= ~OPOST;
    tty.c_oflag &= ~ONLCR;

    tty.c_cc[VTIME] = 0;
    tty.c_cc[VMIN] = 0;


    cfsetispeed(&tty, B38400);
    cfsetospeed(&tty, B38400);

    if(tcsetattr(m_serialPort, TCSANOW, &tty) != 0)
    {
        printf("[!] Failed to read config from serial port\n");
        return false;
    }

    int dtr = TIOCM_DTR;
    ioctl(m_serialPort, TIOCMBIS, &dtr);

    int rts = TIOCM_RTS;
    ioctl(m_serialPort, TIOCMBIS, &rts);

    int flsh = TCIFLUSH;
    ioctl(m_serialPort, TCFLSH, TCIFLUSH);

    return true;
}

void Vestaboard::setRTS(bool value)
{
    int rts = TIOCM_RTS;
    if(value)
        ioctl(m_serialPort, TIOCMBIS, &rts);
    else
        ioctl(m_serialPort, TIOCMBIC, &rts);
}

void Vestaboard::L1_sendPacket(uint8_t* payload, uint16_t size)
{
    setRTS(false);

    unsigned char payloadByte = 0xFF;
    write(m_serialPort, &payload[0], 0x1);

    for(int i = 1; i < size; i++)
    {
        auto payloadByte = payload[i];
        if(payloadByte == 0xFE || payloadByte == 0xFF)
        {
            auto escapeByte = 0xFE;
            write(m_serialPort, &escapeByte, 0x1);
            usleep(1500);
        }

        write(m_serialPort, &payloadByte, 0x1);
        usleep(1500);
    }
    setRTS(true);

    // TODO: Handle buffer, check size etc
    char readBuffer[128]{};
    read(m_serialPort, &readBuffer, 6);
    int totalLength = (readBuffer[5] << 8) + readBuffer[4];
    read(m_serialPort, &readBuffer, totalLength + 4);
}

bool Vestaboard::L2_sendPacket(uint16_t addr, uint8_t flags, uint8_t* payload, uint16_t payloadSize)
{
    // Alloc buffer
    uint16_t packetLength = 10 + payloadSize;
    char* sendBuffer = (char*)malloc(packetLength);
    if(!sendBuffer)
        return false;
    
    // Wipe buffer
    memset(sendBuffer, 0x00, packetLength);

    // Setup buffer, TODO: Struct
    sendBuffer[0] = 0xFF;
    sendBuffer[1] = addr & 0xFF;
    sendBuffer[2] = addr >> 0x8;
    sendBuffer[3] = flags;
    sendBuffer[4] = payloadSize & 0xFF;
    sendBuffer[5] = payloadSize >> 8;
    memcpy(sendBuffer + 6, payload, payloadSize);
    memset(sendBuffer + 6 + payloadSize, 0xFF, 4);

    uint32_t crcValue = CRC::Calculate(sendBuffer, packetLength - 4, CRC::CRC_32());
    memcpy(sendBuffer + 6 + payloadSize, &crcValue, sizeof(crcValue));

    L1_sendPacket((uint8_t*)sendBuffer, packetLength);

    // Free buffer
    free(sendBuffer);
}

bool Vestaboard::L2_sendCommand(uint8_t addr, uint8_t commandId)
{
    uint8_t packet[] = {commandId};

    return L2_sendPacket(addr, 0, packet, sizeof(packet));
}

bool Vestaboard::setCol(uint8_t colId, uint8_t* colData)
{
    // TODO: Struct?
    uint8_t l2_buffer[] = {
        0x01, // Command - Set Target
        0x41, 0x42, 0x43, 0x44, 0x45, 0x46, // 6  Cols
        0x20 // Overhang
    };
    memcpy(l2_buffer + 1, colData, 6);

    L2_sendPacket(colId, 0x00, l2_buffer, sizeof(l2_buffer)); // Set Target
    usleep(40000); // Needed, else we get desynced

    return true;
}

bool Vestaboard::armCol(uint8_t colId)
{
    bool success = L2_sendCommand(colId, 0x02);
    usleep(50000); // Needed, or we get desynced

    return success;
}

bool Vestaboard::go()
{
    bool success = L2_sendCommand(0x01, 0x03);
    usleep(50000); // Needed, or we get desynced

    return success;
}

void Vestaboard::clear()
{
    for(int col = 1; col <= 22; col++)
    {
        uint8_t colClear[6]{};
        setCol(col, colClear);
        armCol(col);
    }
    go();
}
