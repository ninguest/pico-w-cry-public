import socket

def main():
    server_ip = "192.168.68.106"  # Replace with your server's IP address
    server_port = 4242
    message = "Hello, TCP Server!"

    # Create a TCP/IP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # Connect to the server
        sock.connect((server_ip, server_port))
        print(f"Connected to {server_ip}:{server_port}")

        # Send the message
        sock.sendall(message.encode())
        print(f"Sent: {message}")

        # Receive the echoed message
        received = sock.recv(1024)
        print(f"Received: {received.decode()}")

    finally:
        # Close the connection
        sock.close()
        print("Connection closed")

if __name__ == "__main__":
    main()