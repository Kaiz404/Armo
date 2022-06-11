# Import needed libraries
import serial
import ast
import time


PsocSerial = serial.Serial(port = "<insert Psoc com port, eg 'COM7'>", baudrate=115200, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)
ArduinoSerial = serial.Serial(port = "<insert Arduino com port, eg 'COM3'", baudrate=115200, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)

PsocSerialString = ""    # Used to hold data coming over UART from Psoc
# Creating variables
dict_start = False
original_str = ""
PsocDict = {}
intent = ''
slots = ''


def grab_pen():
    print("grab_pen() initialized")
    # Hard coded sequence of movements for Armo to grab a pen from my desk
    ArduinoSerial.write(b'M80')
    time.sleep(1)
    ArduinoSerial.write(b'A25.0 B-9.0')
    time.sleep(3)
    ArduinoSerial.write(b'D-76.0')
    time.sleep(2)
    ArduinoSerial.write(b'M0')
    time.sleep(1)
    ArduinoSerial.write(b'B-30.0')
    time.sleep(4)
    ArduinoSerial.write(b'A-60.0')
    time.sleep(5)   
    ArduinoSerial.write(b'M80')


# Run the program infinitely
while True:

    # Wait until there is data waiting in the serial buffer
    if(PsocSerial.in_waiting > 0):

        # Read data out of the buffer until a carraige return / new line is found
        PsocSerialString = PsocSerial.readline().decode('Ascii')
        print(PsocSerialString)

        # See if the received serial data is what we want
        if PsocSerialString == "[Initialized]\r\n":
            print(PsocSerialString)
            PsocSerialString = ""
            original_str = ""
            PsocDict = {}
            dict_start = True

        # If serial data hasn't ended yet, add the remaining string into one large string
        if dict_start is True and PsocSerialString != "[End]\r\n":
            original_str = original_str + PsocSerialString


        # Check when intent json has ended
        if PsocSerialString == "[End]\r\n" and dict_start == True:
            
            # Replace all unicode in the string with empty characters
            original_str = original_str.replace("\r","").replace("\n", "").replace("\t", "").replace("is_understood", "'is_understood'").replace("intent", "'intent'").replace("slots", "'slots'")

            # After processing the string, convert it into a dictionary object
            PsocDict = ast.literal_eval(original_str)

            print(PsocDict)

            dict_start = False
    
    # Check if dictionary object of intent exists and if it is an understood intent
    if PsocDict is not None and 'is_understood' in PsocDict:
        if PsocDict['is_understood'] == 'true':
            intent = PsocDict['intent']

            if 'slots' in PsocDict:
                slots = PsocDict['slots']
            
            # If intent is to grab, execute G-code sequence to grab object
            if intent == 'grab':
                grab_pen()
                PsocDict = {}

            # If intent is to calibrate, reset all robot axis to zero position
            if intent == 'calibrate':
                ArduinoSerial.write(b'A0 B0 C0 D0 E0')
                PsocDict = {}
