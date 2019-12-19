import time
import serial

ser = serial.Serial(          # ser is used as a Serial for one of the ports.
    port='/dev/ttyXRUSB0',
    baudrate=2400,
    timeout = None
)

ser1 = serial.Serial(               # ser1 is used as a serial for the other port.
	port='/dev/ttyXRUSB1',
	baudrate=2400,
	write_timeout = None
	)

ser1.isOpen() # the port corresponding to /dev/ttyXgRUSB1 is opened.

while(1):
	print("123")
	data = ser1.read(1)      ## reading data from ser (/dev/ttyXRUSB1)
	print("456")
	ser.write(data)          ## writing the same data to ser1 (/dev/ttyXRUSB0)
	print(data)
	


ser1.close()    ## this function is used to close the serial connection.

