#!/usr/bin/python3

import pyttsx3
import sys

engine = pyttsx3.init()
engine.setProperty("voice","es-la")
engine.say(sys.argv[1])
engine.runAndWait()