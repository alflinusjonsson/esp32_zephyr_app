#!/usr/bin/env python3

from socket import AF_INET, SO_REUSEADDR, SOCK_STREAM, SOL_SOCKET, SOCK_DGRAM, socket
from argparse import ArgumentParser
import datetime
import threading
import struct

class SocketListener:
    def __init__(self, address, port, protocol):
        self.__address = address
        self.__port = port
        self.__protocol = protocol

    def __logger__(self, message):
        print(f"[{datetime.datetime.now()}] [{self.__protocol.upper()}] {message}")

    def start(self):
        if self.__protocol == "tcp":
            self._start_tcp_server()
        elif self.__protocol == "udp":
            self._start_udp_server()
        else:
            raise ValueError(f"Unsupported protocol: {self.__protocol}")

    def _start_tcp_server(self):
        server = socket(AF_INET, SOCK_STREAM)
        server.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
        server.bind((self.__address, self.__port))
        server.listen(1)

        self.__logger__(f"Listening on {self.__address}:{self.__port}")

        try:
            while True:
                client, addr = server.accept()
                self.__logger__(f"Accepted TCP connection from: {addr[0]}:{addr[1]}")
                client_handler = threading.Thread(target=self._handle_tcp_client, args=(client, addr))
                client_handler.daemon = True
                client_handler.start()
        except KeyboardInterrupt:
            self.__logger__("Shutting down server...")
        finally:
            server.close()

    def _start_udp_server(self):
        server = socket(AF_INET, SOCK_DGRAM)
        server.bind((self.__address, self.__port))

        self.__logger__(f"Listening on {self.__address}:{self.__port}")

        try:
            while True:
                data, addr = server.recvfrom(1024)
                if len(data) == 8:
                    val1, val2 = struct.unpack('<ii', data)
                    temp_celsius = val1 + (val2 / 1000000.0)
                    self.__logger__(f"Received temperature from {addr[0]}:{addr[1]}: {temp_celsius:.6f} °C")
                else:
                    self.__logger__(f"Received data from {addr[0]}:{addr[1]}: {data}")
        except KeyboardInterrupt:
            self.__logger__("Shutting down server...")
        finally:
            server.close()

    def _handle_tcp_client(self, client_socket, addr):
        try:
            while True:
                request = client_socket.recv(1024)
                if not request:
                    self.__logger__(f"{addr[0]}:{addr[1]} disconnected")
                    break
                self.__logger__(f"Received data from {addr[0]}:{addr[1]}: {request}")
        except Exception as e:
            self.__logger__(f"Error handling {addr[0]}:{addr[1]}: {e}")
        finally:
            client_socket.close()

if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("--ip", type=str, required=True)
    parser.add_argument("--port", type=int, required=True)
    parser.add_argument("--protocol", type=str, default="udp", choices=["udp", "tcp"])
    args = parser.parse_args()

    SocketListener = SocketListener(args.ip, args.port, args.protocol)
    SocketListener.start()
