import json
class Drone :
    led = False;
    def __init__(self, id, status, battery) :
        self.__id = id
        self.__status = status
        self.__battery = battery
    #public functions

    # getters
    def getID(self):
        return self.__id
    def getStatus(self):
        return self.__status
    def getBattery(self):
        return self.__battery
    
    # member functions
    def update(self, voltage, arg1):
        __updateBattery(self,voltage)
        __updateStatus(self,arg1)

    # TODO : add code to toggle the LED on the drone
    def toggleLED(self):
        self.led = not self.led
        print("The led on drone #",self.__id," has been toggled to ", self.led)
    #private functions
    
    # TODO : actual calculation of battery level when voltage is known
    def __updateBattery(self, voltage):
        self.__battery += 1 
    # TODO: find a way to get the status of the drone (busy, ready, dead)
    def __updateStatus(self, arg1):
        self.__status = arg1
