import Adafruit_BBIO.PWM as PWM

motor1="p8_13"
duty_stop=9
duty_forward=12
duty_back=6
PWM.start(motor1,duty_stop,60)

print"Ready"

key='0'
while key != 'q':
	key=raw_input(">")
	print key
	if key=='1':
		PWM.set_duty_cycle(motor1,duty_forward)
	elif key =='2':
		PWM.set_duty_cycle(motor1,duty_back)
	elif key=='3':
		PWM.set_duty_cycle(motor1,duty_stop)
	elif key=='4':
		PWM.stop(motor1)

PWM.cleanup()