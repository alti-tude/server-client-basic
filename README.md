server-client-basic
======================

# to run
1. run ```make``` in server directory
2. run ```g++ -o client client.cpp``` in client directory

# socket setup on server
1. setup a socket
2. bind the socket to a port and in_addr
3. listen on the port and accept connections
4. accepting gives a brand newe file descriptor, which is used to send and recieve stuff fron that particular client

#setup server listen socket(steps 1, 2)
```c++
socket_fd = socket(AF_INET, SOCK_STREAM, 0);
```
* *domain*   
    * AF_INET = IPV4
    * kinda like specifies the addressing family
* *connection type*  
    **SOCK_STREAM**: Provides sequenced, reliable, two-way, connection-based byte  streams.  An out-of-band data transmission mechanism may be supported.

    **SOCK_DGRAM**: Supports datagrams (connectionless, unreliable messages of a fixed maximum length).

    **SOCK_SEQPACKET**: Provides  a  sequenced,  reliable,  two-way connection-based data transmission path  for  datagrams  of  fixed maximum  length;  a  consumer  is  required  to read an entire packet with each input system call.

    **SOCK_RAW**: Provides raw network protocol access.

    **SOCK_RDM**: Provides a reliable datagram layer that does not  guarantee ordering.
* *protocol*
    * if multiple protocols exist within a protocol family<b>(i guess something like tcp/udp are different protocols?)</b>
    * if 0, protocol is automatically chosen.
