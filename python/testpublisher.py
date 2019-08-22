#!/usr/bin/env python3
from nanomsg import Socket, PUB
import time


s1 = Socket(PUB)
s1.bind('ipc:///tmp/testsocket'.encode('utf-8'))



while True:
	print("send something")
	s1.send("sending a message from python".encode('utf-8'))
	time.sleep(1)

s1.close()

