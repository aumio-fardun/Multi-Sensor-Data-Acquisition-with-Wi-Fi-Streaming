import asyncio
import socket
import time

# ESP32 server settings
HOST = '192.168.10.35'  # Replace with your ESP32's IP address
PORT = 80               # The port you set up in the ESP32 code
DELAY = 0.5 

async def connect_and_receive():
    while True:
        try:
            reader, writer = await asyncio.open_connection(HOST, PORT)
            print(f"Connected to {HOST}:{PORT}")
            
            while True:
                data = await reader.read(256)
                if data:
                    print(data.decode(), end='', flush=True)
                    time.sleep(DELAY)
                else:
                    print("\nConnection closed by server. Attempting to reconnect...")
                    break
        
        except (ConnectionRefusedError, OSError) as e:
            print(f"Connection failed: {e}. Retrying in 5 seconds...")
            await asyncio.sleep(1)
        
        finally:
            if 'writer' in locals() and not writer.is_closing():
                writer.close()
                await writer.wait_closed()

async def main():
    while True:
        try:
            await connect_and_receive()
        except KeyboardInterrupt:
            print("\nProgram terminated by user.")
            break
        except Exception as e:
            print(f"An unexpected error occurred: {e}. Restarting the connection process...")
            await asyncio.sleep(1)

if __name__ == "__main__":
    asyncio.run(main())