#!/usr/bin/env python

# We need to be a ROS node to subscribe to things
import rospy

# We'll be using rosgraph to obtain the topic list
import rosgraph

# This is for converting a type name into an instance of that type
import roslib.message

# Callback function for new messages
def callback(data):
	print(data)

# Initialize our node
rospy.init_node('logger')

# Grab the master for topic listing
master = rosgraph.Master('')

# Get the topic list
topicList = master.getPublishedTopics('')

# Iterate through the topics and list them out
for [topic,type] in topicList:
	rospy.Subscriber(topic, roslib.message.get_message_class(type), callback)

# Infinite loop!
rospy.spin()
