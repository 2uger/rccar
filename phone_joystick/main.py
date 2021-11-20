from kivy.app import App
from kivy.uix.widget import Widget
from kivy.uix.button import Button
from kivy.properties import (
    NumericProperty, ReferenceListProperty, ObjectProperty
)
from kivy.vector import Vector
from kivy.clock import Clock
from random import randint
import plyer

import time
import socket


class EnterButton(Button):
    
    def on_presss(self, event):
        HOST = '192.168.100.19'  # The server's hostname or IP address
        PORT = 9997 
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print('Connecting')
        self.s.connect((HOST, PORT))

        while True:
            time.sleep(.25)
            r = self.gyro.rotation or 0.0
            r = (round(r[0], 3), round(r[1], 3))
            print(r)
            self.s.sendall(str(r).encode())

    def send_msg(self, *args):
        self.counter += 1
        gyro_data = 'Hello'# self.gyro.rotation()
        self.s.sendall(str(gyro_data))


class Background(Widget):
    connect_bttn = ObjectProperty(None)

    def setup(self):
        # initing gyroscope
        self.connect_bttn.gyro = plyer.gyroscope
        self.connect_bttn.gyro.enable()
        self.connect_bttn.bind(on_press=self.connect_bttn.on_presss)

    def update(self, dt):
        pass


class Joystick(App):
    def build(self):
        b = Background()
        b.setup()

        return b


if __name__ == '__main__':
    Joystick().run()
