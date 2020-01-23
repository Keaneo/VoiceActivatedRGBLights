import serial
import time
import speech_recognition as sr
from pocketsphinx import LiveSpeech, get_model_path
import os

model_path = get_model_path()

s = serial.Serial('COM6')
time.sleep(2)
#s.write("g".encode())
def parse(text):
    text.lower()
    if "make the lights go" in text:
        if "red" in text:
            s.write('r'.encode())
        if "blue" in text:
            s.write('b'.encode())
        if "green" in text:
            s.write('g'.encode())
        if "yellow" in text:
            s.write('y'.encode())
        if "white" in text:
            s.write('w'.encode())
        if "purple" in text:
            s.write('p'.encode())

r = sr.Recognizer()
while True:
    with sr.Microphone() as source:
        print("Say Anything: ")
        audio = r.listen(source)        
        try:
            text = r.recognize_google(audio)
            print("You said: {}".format(text.lower()))
            parse(text)
        except:
            print("Sorry, didn't catch that!")

# speech = LiveSpeech(
#     verbose = False,
#     sampling_rate=16000,
#     buffer_size=2048,
#     no_search=False,
#     full_utt=False,
#     hmm=os.path.join(model_path, 'en-us'),
#     lm=os.path.join(model_path, 'en-us.lm.bin'),
#     dic=os.path.join(model_path, 'cmudict-en-us.dict')
# )
# for phrase in speech:
#     print(phrase)
#     parse(str(phrase).lower())


