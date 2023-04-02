import socket
import struct

MCAST_GRP = '239.255.0.1'
MCAST_PORT = 4211
IS_ALL_GROUPS = True

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
if IS_ALL_GROUPS:
    # on this port, receives ALL multicast groups
    sock.bind(('', MCAST_PORT))
else:
    # on this port, listen ONLY to MCAST_GRP
    sock.bind((MCAST_GRP, MCAST_PORT))
mreq = struct.pack("4sl", socket.inet_aton(MCAST_GRP), socket.INADDR_ANY)


sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

while True:
    msg, address = sock.recvfrom(10240)
    print(msg, address)
    STATUS_RSSI = (msg[3])
    if STATUS_RSSI == 1:
        RSSI = msg[4]-255
        print(RSSI)
        IR = msg[9]
        print(IR)
        print(msg[11]+msg[12])
        print(msg[13]+msg[14])
        print(msg[15]+msg[16])
        print(msg[18]+msg[19])


        #STATUS_IR = int(msg[4:8].hex(), 16)
        #STATUS_SIMULATION = int(msg[3].hex(), 16)
        #STATUS_HEADLIGHTS = int(msg[3].hex(), 16)
  