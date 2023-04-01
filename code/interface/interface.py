import pyglet
from src import CarCrtl

window = pyglet.window.Windows()

from pyglet.window import key

@window.event
def on_key_press(symbol):
    direction_angle = 0
    direction = 0
    if symbol == key.Z or symbol == key.MOTION_UP:
        direction += 8192
    elif symbol == key.Q or symbol == key.MOTION_LEFT:
        direction_angle += 32767
    elif symbol == key.D or symbol == key.MOTION_RIGHT:
        direction_angle -= 32768
    elif symbol == key.S or symbol == key.SPACE or symbol == key.MOTION_DOWN:
        direction -= 8192

def video_capture (status):
    video = True
    black_screen  = False    
    if status == "ON":
        return video # pas fini a changé 
    elif status == "OFF": 
        return black_screen # a revoir 