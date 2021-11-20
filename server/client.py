import asyncio
import websockets

async def hello():
    uri = "ws://localhost:9999"
    async with websockets.connect(uri) as websocket:
        while True:
            recv = await websocket.recv()
            print(recv)
asyncio.get_event_loop().run_until_complete(hello())
