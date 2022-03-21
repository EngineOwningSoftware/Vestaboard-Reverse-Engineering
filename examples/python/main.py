import serial
import serial.rs485
import crc
import time

serialPort = serial.Serial('/dev/ttyAMA0', 38400)

def L1_sendPacket(payload):
    serialPort.rts = False
    serialPort.write([payload[0]])
    for c in payload[1:]:
        if c == 0xfe or c == 0xff:
            serialPort.write([0xfe])
        serialPort.write([c])
    time.sleep(0.01)
    serialPort.rts = True
    
    returnData = bytearray(serialPort.read(6))
    payloadLength = (returnData[5] << 8) + returnData[4]
    serialPort.read(payloadLength + 4)

def L2_sendPacket(addr, flags, payload):
    packetLength = 10 + len(payload)
    buffer = bytearray(packetLength)

    buffer[0] = 0xFF                                        # Magic Byte
    buffer[1] = addr & 0xFF                                 # Addr Lower
    buffer[2] = addr >> 0x8                                 # Addr Upper
    buffer[3] = flags                                       # Flags
    buffer[4] = len(payload) & 0xFF                         # Payload Len Lower
    buffer[5] = len(payload) >> 8                           # Payload Len Upper
    buffer[6:6+len(payload)] = payload                      # Copy payload
    buffer[6+len(payload)+0] = 0xFF                         # Dummy CRC Byte 1
    buffer[6+len(payload)+1] = 0xFF                         # Dummy CRC Byte 2
    buffer[6+len(payload)+2] = 0xFF                         # Dummy CRC Byte 3
    buffer[6+len(payload)+3] = 0xFF                         # Dummy CRC Byte 4

    crc32 = crc.CRC32(buffer, packetLength - 4)             # Calculate CRC

    buffer[6+len(payload)+0] = (crc32 >> 0) & 0xFF      # CRC Byte 1
    buffer[6+len(payload)+1] = (crc32 >> 8) & 0xFF      # CRC Byte 2
    buffer[6+len(payload)+2] = (crc32 >> 16) & 0xFF      # CRC Byte 3
    buffer[6+len(payload)+3] = (crc32 >> 24) & 0xFF      # CRC Byte 4

    L1_sendPacket(buffer)

def sendCol(colId, colBuffer):
    payload = bytearray(8)
    payload[0] = 0x01 # Set Target
    payload[1:1+len(colBuffer)] = colBuffer
    payload[7] = 0x20

    L2_sendPacket(colId, 0, payload)

def armCol(colId):
    payload = bytearray(1)
    payload[0] = 0x02 # Arm

    L2_sendPacket(colId, 0, payload)

def goCol(colId):
    payload = bytearray(1)
    payload[0] = 0x03 # Go

    L2_sendPacket(colId, 0, payload)

def fillBoard(charId):
	for i in range(1,22):
		sendCol(i, [charId] * 6)
		armCol(i)
	goCol(1)

def clearBoard():
	fillBoard(0x00)

fillBoard(0x45) # Set every flap to 'E'
