import serial
import time

ser = serial.Serial(               # ser is used as a Serial for one of the ports.
    port='/dev/ttyXRUSB0',
    baudrate=2400,
    timeout = None
)

ser1 = serial.Serial(              # ser1 is used as a serial for the other port.
	port='/dev/ttyXRUSB1',
	baudrate=2400,
	write_timeout = None
	)

ser.isOpen()  # the port corresponding to /dev/ttyXgRUSB0 is opened.

while(1):
	print("123")
	data = ser.read()   ## reading data from ser (/dev/ttyXRUSB0)
	print("456")
	ser1.write(data)    ## writing the same data to ser1 (/dev/ttyXRUSB1)
	print(data)


ser.close()                 ## this function is used to close the serial connection.

