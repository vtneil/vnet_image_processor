# vnet-image-processor

A C++ bitmap image class for saving on memory and 
for serialization. Currently, this supports COLOR (RGB) and
GRAYSCALE 8 bit/channel only.

No compression algorithm has been implemented yet.

For serializing/deserializing image data from/to 
microcontroller system.

Sending over wireless communication requires very high 
performance wireless modules with high air data rate. 
Recommends more than 200 kbps for effective high-resolution 
transmission. For lower pixel counts, you can choose lower 
rate modules.

**Upcoming features**: supports for 4-bit/channel with 
bit packing techniques and 8-bit Color (RRRGGGBB).