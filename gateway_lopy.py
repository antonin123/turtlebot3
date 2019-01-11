#!/usr/bin/env python

# mosquitto_sub -d -t application/# -u userdemo1 -P icube01 -h mqttnet.u-strasbg.fr -i pytrack-01
import rospy
from std_msgs.msg import String
import time
import paho.mqtt.client as mqtt

serv = "mqttnet.u-strasbg.fr"
user = "userdemo1"
pwd = "icube01"
str_payload = ""
pub = rospy.Publisher("lopy", String, queue_size=10)


# The callback for when the client receives a CONNACK response from the server.
# Callback quand le client a recu un mess CONNACK du serveur
def connack_callback(client, userdata, flags, rc):
    rospy.loginfo("Connected with result code " + str(rc))
	# 0: Connection successful
	# 1: Connection refused - incorrect protocol version
	# 2: Connection refused - invalid client identifier
	# 3: Connection refused - server unavailable
	# 4: Connection refused - bad username or password
	# 5: Connection refused - not authorised
	# 6-255: Currently unused.
    # Si on perd la connection et que l'on se reconnecte
	# alors les souscription sont renouvelees
    client.subscribe("#")


# Callback quand le server a recu un mess PUBLISH d'un client
def publish_callback(client, userdata, msg):
	str_payload = str(msg.payload)
	rospy.loginfo(msg.topic + " " + str_payload)
	pub.publish(str_payload)

def Lopy_Client():

	rospy.init_node("lopy", anonymous=True)
	#user= raw_input("Enter user mqtt: ")
	#pwd= raw_input("Enter pwd mqtt: ")

	client = mqtt.Client()
	client.username_pw_set(user, pwd)
	client.connect(serv)
	client.on_connect = connack_callback
	client.on_message = publish_callback

	client.connect(serv, 1883, 60)
	client.loop_forever()

if __name__ == '__main__':
	try:
		Lopy_Client()
	except rospy.ROSInterruptException:
		pass

