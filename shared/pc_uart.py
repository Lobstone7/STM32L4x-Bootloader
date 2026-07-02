import math
import os
import serial
import time

# -----------------------------
# Configuration
# -----------------------------
PORT = "COM11"
BAUDRATE = 115200
CHUNK_SIZE = 256

# Get the absolute path to app.bin
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
FIRMWARE = os.path.join(SCRIPT_DIR, "..", "build", "app.bin")

# -----------------------------
# Protocol bytes
# -----------------------------
WRITE = b'w'
READY = b'R'
ACK = b'A'
NACK = b'F'
SUCCESS = b'S'
ERROR = b'E'

# -----------------------------
# Open serial port
# -----------------------------
try:
    ser = serial.Serial(PORT, BAUDRATE, timeout=60)
except serial.SerialException as e:
    print(f"Failed to open {PORT}: {e}")
    exit()

# Give the board time to reset after opening the port
time.sleep(2)

# Clear any stale data
ser.reset_input_buffer()
ser.reset_output_buffer()

# -----------------------------
# Send write command
# -----------------------------
print("Sending write request...")
ser.write(WRITE)

print("Waiting for bootloader ready...")

ready = ser.read(1)

if ready != READY:
    if ready == b'':
        print("Timed out waiting for bootloader.")
    else:
        print(f"Unexpected response: {ready}")
    ser.close()
    exit()

print("Bootloader ready.\n")

# -----------------------------
# Read firmware file
# -----------------------------
if not os.path.isfile(FIRMWARE):
    print(f"Firmware file not found:\n{FIRMWARE}")
    ser.close()
    exit()

with open(FIRMWARE, "rb") as f:
    firmware = f.read()

firmware_size = len(firmware)
total_packets = math.ceil(firmware_size / CHUNK_SIZE)

print(f"Firmware file : {FIRMWARE}")
print(f"Firmware size : {firmware_size} bytes")
print(f"Packet size   : {CHUNK_SIZE} bytes")
print(f"Total packets : {total_packets}\n")

# -----------------------------
# Send firmware packets
# -----------------------------
offset = 0
packet_number = 1

while offset < firmware_size:

    # Get next packet
    chunk = firmware[offset:offset + CHUNK_SIZE]

    # Pad the last packet if required
    if len(chunk) < CHUNK_SIZE:
        chunk += b'\x00' * (CHUNK_SIZE - len(chunk))

    retry_count = 0

    while True:

        print(f"[Packet {packet_number}/{total_packets}] Sending...", end=" ")

        ser.write(chunk)

        response = ser.read(1)

        if response == ACK:
            print("ACK")
            break

        elif response == NACK:
            retry_count += 1
            print(f"NACK (Retry {retry_count})")

        elif response == b'':
            print("Timeout waiting for ACK.")
            ser.close()
            exit()

        else:
            print(f"Unknown response: {response}")
            ser.close()
            exit()

    offset += CHUNK_SIZE

    print(
        f"Packet {packet_number}/{total_packets} completed "
        f"({min(offset, firmware_size)}/{firmware_size} bytes sent)\n"
    )

    packet_number += 1

# -----------------------------
# Wait for final status
# -----------------------------
print("All packets transmitted.")
print("Waiting for CRC verification...")

result = ser.read(1)

if result == SUCCESS:
    print("Programming successful.")

elif result == ERROR:
    print("Programming failed (CRC verification failed).")

elif result == b'':
    print("Timed out waiting for final bootloader response.")

else:
    print(f"Invalid bootloader response: {result}")

ser.close()