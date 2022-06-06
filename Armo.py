import serial
import ast
import time

PsocSerial = serial.Serial(port = "COM7", baudrate=115200, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)
ArduinoSerial = serial.Serial(port = "COM3", baudrate=115200, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)

PsocSerialString = ""                           # Used to hold data coming over UART from Psoc

dict_start = False
original_str = ""
PsocDict = {}
intent = ''
slots = ''


def grab_pen():
    print("grab_pen() initialized")
    # G0 A0.0 B0.0 C0.0 D0.0 X0.0 Y0.0 Z0.0
    ArduinoSerial.write(b'M3 S80')
    time.sleep(1)
    ArduinoSerial.write(b'G0 A25.0 C-9.0')
    time.sleep(3)
    ArduinoSerial.write(b'G0 Y-76.0')
    time.sleep(2)
    ArduinoSerial.write(b'M3 S0')
    time.sleep(1)
    ArduinoSerial.write(b'G0 C-30.0')
    time.sleep(4)
    ArduinoSerial.write(b'G0 A-60.0')
    time.sleep(5)
    ArduinoSerial.write(b'M3 S80')



while(1):

    # Wait until there is data waiting in the serial buffer
    if(PsocSerial.in_waiting > 0):

        # Read data out of the buffer until a carraige return / new line is found
        PsocSerialString = PsocSerial.readline().decode('Ascii')
        print(PsocSerialString)

        if PsocSerialString == "[Initialized]\r\n":
            print(PsocSerialString)
            PsocSerialString = ""
            original_str = ""
            PsocDict = {}
            dict_start = True

        if dict_start is True and PsocSerialString != "[End]\r\n":
            original_str = original_str + PsocSerialString


        if PsocSerialString == "[End]\r\n" and dict_start == True:
            
            original_str = original_str.replace("\r","").replace("\n", "").replace("\t", "").replace("is_understood", "'is_understood'").replace("intent", "'intent'").replace("slots", "'slots'")

            PsocDict = ast.literal_eval(original_str)

            print(type(PsocDict))
            print(PsocDict)

            dict_start = False
    

    if PsocDict is not None and 'is_understood' in PsocDict:
        if PsocDict['is_understood'] == 'true':
            intent = PsocDict['intent']

            if 'slots' in PsocDict:
                slots = PsocDict['slots']
            



            if intent == 'grab':
                grab_pen()
                PsocDict = {}

            if intent == 'calibrate':
                ArduinoSerial.write(b'G0 A0 B0 C0 D0 X0 Y0 Z0')
                PsocDict = {}


