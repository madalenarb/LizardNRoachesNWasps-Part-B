import zmq
import sys
import random
import signal
from time import sleep

protos_path = "../../protos/python"
sys.path.insert(0, protos_path)
import protocol_pb2

# Global flag for signal handling
shutdown_requested = False

def handle_signal(signal_received, frame):
    # Set the global flag to True when Ctrl+C is pressed
    global shutdown_requested
    shutdown_requested = True
    print("\nCtrl+C detected, shutting down...")

def main():
    global shutdown_requested
    # Set up signal handler
    signal.signal(signal.SIGINT, handle_signal)

    # Check command-line arguments
    if len(sys.argv) != 3:
        print("Usage: wasps-client address Port")
        sys.exit(1)

    address, port = sys.argv[1], sys.argv[2]
    server_name = f"tcp://{address}:{port}"

    # Set up ZeroMQ context and socket
    context = zmq.Context()
    socket = context.socket(zmq.REQ)
    socket.connect(server_name)

    # Create and send a WASPS_CONNECT message
    wasp_msg = protocol_pb2.GameMessage()
    wasp_msg.msg_type = protocol_pb2.WASPS_CONNECT
    n_wasps = random.randint(1, 10)  # Random number of wasps between 1 and 10
    wasp_msg.n_wasps = n_wasps

    # Serialize and send the message
    socket.send(wasp_msg.SerializeToString())

    # Receive and unpack acknowledgement
    ack_msg = protocol_pb2.GameMessage()
    ack_msg.ParseFromString(socket.recv())

    if ack_msg.msg_type == protocol_pb2.WASPS_DISCONNECT:
        socket.close()
        context.term()
        sys.exit(1)

    index = ack_msg.index
    print(f"Wasp client {index} connected")

    try:
        while not shutdown_requested:
            for i in range(n_wasps):
                sleep(random.uniform(0, 3))  # Sleep for up to 3 seconds
                direction = random.choice([protocol_pb2.UP, protocol_pb2.DOWN, protocol_pb2.LEFT, protocol_pb2.RIGHT])

                movement_msg = protocol_pb2.GameMessage()
                movement_msg.msg_type = protocol_pb2.WASPS_MOVEMENT
                movement_msg.index = index
                movement_msg.wasp_index = i
                movement_msg.direction = direction

                socket.send(movement_msg.SerializeToString())

                ack_msg.ParseFromString(socket.recv())

                if ack_msg.msg_type == protocol_pb2.WASPS_DISCONNECT:
                    print("Received disconnect from server.")
                    break

                if shutdown_requested:
                    raise KeyboardInterrupt

    except KeyboardInterrupt:
        # Send disconnect message
        disconnect_msg = protocol_pb2.GameMessage()
        disconnect_msg.msg_type = protocol_pb2.WASPS_DISCONNECT
        disconnect_msg.index = index
        socket.send(disconnect_msg.SerializeToString())

        # Wait for server acknowledgement
        ack_msg.ParseFromString(socket.recv())
        if ack_msg.msg_type == protocol_pb2.WASPS_DISCONNECT:
            print("Disconnect acknowledged by server.")

    finally:
        socket.close()
        context.term()

if __name__ == "__main__":
    main()
