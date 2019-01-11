#!/usr/bin/env python

import serial
import rospy
from std_msgs.msg import String
import time


class Arduino_Gateaway(object):

	def __init__(self):
		rospy.init_node("arduino", anonymous=True)
		self.pub = rospy.Publisher("arduino", String, queue_size=10)
		self.quit = False
		self.dev = raw_input("Device ?")
		self.Ser = serial.Serial(self.dev, 9600, timeout=1, writeTimeout=1)

	def get_cmd(self):
		while (not self.quit and not rospy.is_shutdown()):

			cmd = raw_input("CMD ?")

			if(cmd == "quit"):
				print "Adios amigos !"
				self.quit = True
				self.pub.publish(cmd)


			elif(cmd == "l1" or cmd == "l0" or cmd == "ls"):
				self.Ser.write(cmd)

				try:
					while(1):
						text = self.Ser.readline()
						if (text != ""):
							print text
							self.pub.publish(text)

				except KeyboardInterrupt:
					self.Ser.write("q")

			else:
				print "Usage : quit, l1 (on) l0(off) or ls(led state)"


if __name__ == '__main__':
	try:
		Arduino = Arduino_Gateaway()
		time.sleep(1)
		Arduino.get_cmd()
	except rospy.ROSInterruptException:
		pass

