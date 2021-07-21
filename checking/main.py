import serial
import time 
ser = serial.Serial('/dev/ttyUSB0', 9600)
data ="1,90,1"
while True:
  ser.write(data.encode()) 
  print("try")
  time.sleep(0.1)