import time
import threading
import asyncio
import websockets
import queue

import keyboard

HOST = '0.0.0.0'
PORT = 8000

# Multiple threads better working with consume-send module
# !!! even in such simple keyboard controller !!!
# to avoid random moves while waiting inpute, we use queue
# to make it continious
motion_queue = queue.Queue()

#********* Server part ***********

server_prefix = 'WEB-SOCKET SERVER:'
async def server(websocket, *args):
    while True:
        time.sleep(0.05)
        (velocity, rotation) = motion_queue.get()
        print("Geting from queue", velocity, rotation)
        if velocity > 0:
            vel = velocity * 50 / 100 + 50
        else:
            vel = velocity * 50 / 100 - 50
        vel = 0 if velocity == 0 else vel
        await websocket.send(str(f'({int(vel)},{rotation});'))

def start_server():
    print(server_prefix, 'Start server')
    start_server = websockets.serve(server, HOST, PORT)

    asyncio.get_event_loop().run_until_complete(start_server)
    asyncio.get_event_loop().run_forever()
    
#********* Controller part ***********

# will indicate how fast we should move
# from -100 to 100
VELOCITY = 0
# will indicate how we should rotate
# from -100(left) to 100(right)
ROTATION = 0 

MOVE_FORWARD_PRESSED = False
MOVE_BACKWARD_PRESSED = False
MOVE_RIGHT_PRESSED = False
MOVE_LEFT_PRESSED = False

def control_movement():
    """Change smoothly velocity and rotation according to pressed button"""
    global VELOCITY
    global ROTATION
    while True:
        time.sleep(0.1)
        # change velocity
        if MOVE_FORWARD_PRESSED:
            if VELOCITY < 0:
                VELOCITY = 0
            VELOCITY += 5 if VELOCITY < 96 else 0
        elif MOVE_BACKWARD_PRESSED:
            if VELOCITY > 0:
                VELOCITY = 0
            VELOCITY -= 5 if VELOCITY > -96 else 0
        else:
            # stay slowly
            velocity_direction = 1 if VELOCITY > 0 else -1
            VELOCITY -= 5 * (velocity_direction) if VELOCITY != 0 else 0
        # change rotation
        if MOVE_RIGHT_PRESSED:
            ROTATION = 100
        elif MOVE_LEFT_PRESSED:
            ROTATION = -100
        else:
            ROTATION = 0 
        motion_queue.put((VELOCITY, ROTATION))

def move_forward(event):
    """Check if button pressed or released"""
    global MOVE_FORWARD_PRESSED
    global MOVE_BACKWARD_PRESSED

    if event.event_type == keyboard.KEY_UP:
        MOVE_FORWARD_PRESSED = False
    elif event.event_type == keyboard.KEY_DOWN:
        # only if not moving backward already
        if not MOVE_BACKWARD_PRESSED:        
            MOVE_FORWARD_PRESSED = True

def move_backward(event):
    global MOVE_FORWARD_PRESSED
    global MOVE_BACKWARD_PRESSED

    if event.event_type == keyboard.KEY_UP:
        MOVE_BACKWARD_PRESSED = False
    elif event.event_type == keyboard.KEY_DOWN:
        # only if not moving forward already
        if not MOVE_FORWARD_PRESSED:        
            MOVE_BACKWARD_PRESSED = True

def move_left(event):
    global MOVE_LEFT_PRESSED

    if event.event_type == keyboard.KEY_UP:
        MOVE_LEFT_PRESSED = False
    elif event.event_type == keyboard.KEY_DOWN:
        # only if not rotate right already
        if not MOVE_RIGHT_PRESSED:        
            MOVE_LEFT_PRESSED = True

def move_right(event):
    global MOVE_RIGHT_PRESSED

    if event.event_type == keyboard.KEY_UP:
        MOVE_RIGHT_PRESSED = False
    elif event.event_type == keyboard.KEY_DOWN:
        # only if not rotate left already
        if not MOVE_LEFT_PRESSED:        
            MOVE_RIGHT_PRESSED = True

def setup_keyboard_events():
    keyboard.on_press_key('up', move_forward)
    keyboard.on_press_key('down', move_backward)
    keyboard.on_press_key('left', move_left)
    keyboard.on_press_key('right', move_right)

    keyboard.on_release_key('up', move_forward)
    keyboard.on_release_key('down', move_backward)
    keyboard.on_release_key('left', move_left)
    keyboard.on_release_key('right', move_right)
    keyboard.wait()

def start_controller():
    _control_movement = threading.Thread(target=control_movement)
    _setup_keyboard_events = threading.Thread(target=setup_keyboard_events)
    _control_movement.start()
    _setup_keyboard_events.start()

if __name__ == '__main__':
    start_controller()
    start_server()
