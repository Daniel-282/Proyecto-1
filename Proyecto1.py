# Universidad del Valle de Guatemala
# Juan Daniel Cortez - 17161

from Adafruit_IO import Client, RequestError, Feed
import serial
import time


ser = serial.Serial('COM4', baudrate = 9600)

ADAFRUIT_IO_KEY = "aio_Jzba05Sxa1cIXx0rUulMkjHayroe"
ADAFRUIT_IO_USERNAME = "jdanielcortez"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

while(1):
    ser.close()
    ser.open()
    
#------------------------------------------------------------------------------    

    #Conversión datos recibidos del Pic
    DatoRecibido = str(ser.read(9))
    print(DatoRecibido)
    ListaRecibida = DatoRecibido.split(',')
    ValorTemp = ListaRecibida[1]
    
#------------------------------------------------------------------------------    
    
    #Temperatura - Valor para enviar a Adafruit IO
    Feed_Temp = aio.feeds('temperatura')
    aio.send_data(Feed_Temp.key, ValorTemp)
    Data_Temp = aio.receive(Feed_Temp.key)
    print(f'Temperatura: {Data_Temp.value}')

#------------------------------------------------------------------------------    

    #Humedad - Valor recibido de Adafruit IO para enviar al Pic
    Feed_Hum = aio.feeds('humedad')
    Data_Hum = aio.receive(Feed_Hum.key)
    print(f'Humedad: {Data_Hum.value}')
    ValorHum1 = int(Data_Hum.value)
    ValorHum2 = ValorHum1.to_bytes(3, 'big')
    ser.write(ValorHum2)
    time.sleep(5)
