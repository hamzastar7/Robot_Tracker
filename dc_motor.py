import time as t
import Adafruit_BBIO.PWM as PWM
import Adafruit_BBIO.GPIO as GPIO
print "Importation is ready"


motor1="p8_13"

duty_stop=9
duty_forward=12
duty_back=6

PWM.start("P8_13", 0)
PWM.start("P8_19", 0)
print "Importation PWM is OK"

GPIO.setup("P8_7", GPIO.OUT)
GPIO.setup("P8_8", GPIO.OUT)
GPIO.setup("P8_9", GPIO.OUT)
GPIO.setup("P8_10", GPIO.OUT)
print "Setup GPIO is OK"

GPIO.output("P8_7", GPIO.HIGH)
GPIO.output("P8_9", GPIO.LOW)

GPIO.output("P8_8", GPIO.HIGH)
GPIO.output("P8_10", GPIO.LOW)

print"Ready"

key='0'
while key != 'q':
	key=raw_input(">")
	print key
	if key=='1':
		
		GPIO.output("P8_7", GPIO.HIGH)
		GPIO.output("P8_9", GPIO.LOW)

		GPIO.output("P8_8", GPIO.HIGH)
		GPIO.output("P8_10", GPIO.LOW)

		PWM.set_duty_cycle("P8_13", 85)
		PWM.set_duty_cycle("P8_19", 85)
		t.sleep(0.2)
                PWM.set_duty_cycle("P8_13", 65)
                PWM.set_duty_cycle("P8_19", 65)


	elif key =='2':
		
		GPIO.output("P8_7", GPIO.LOW)
		GPIO.output("P8_9", GPIO.HIGH)

		GPIO.output("P8_8", GPIO.HIGH)
		GPIO.output("P8_10", GPIO.LOW)

		PWM.set_duty_cycle("P8_13", 65)
		PWM.set_duty_cycle("P8_19", 65)
		
	elif key=='3':
		GPIO.output("P8_7", GPIO.HIGH)
		GPIO.output("P8_9", GPIO.LOW)

		GPIO.output("P8_8", GPIO.LOW)
		GPIO.output("P8_10", GPIO.HIGH)

		PWM.set_duty_cycle("P8_13", 65)
		PWM.set_duty_cycle("P8_19", 65)


	elif key=='4':
                GPIO.output("P8_7", GPIO.HIGH)
                GPIO.output("P8_9", GPIO.LOW)

                GPIO.output("P8_8", GPIO.HIGH)
                GPIO.output("P8_10", GPIO.LOW)

                PWM.set_duty_cycle("P8_13", 0)
                PWM.set_duty_cycle("P8_19", 0)




	elif key=='5':
		PWM.stop("P8_13")
		PWM.stop("P8_19")
		#PWM.start("P8_13", 0)

PWM.cleanup()
