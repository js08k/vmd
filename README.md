# Brief
VMD (Virtual Movie Date) is an application that aims to allow two people to 
watch a DVD synchronously at two remote locations.

# Libraries
The project is built on top of Qt 5. It also utalizes libdvdread/libdvdnav for
DVD navagation and reading and libdvdcss for DVD decryption. The project is 
using GtQt (Google protocol buffers in unison with Qt network) to perform 
network message passing between remote clients.
