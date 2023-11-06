# UDP SERVER
  <p>The udp server listens on port 8055 by default. It uses batches with a similar functionality to tcp acks. This facillates udp lost packet detection and adds the ability to automatically resend lost packets</p>
  this model automatically serves a certain portion of the mp3 file max.mp3. The portion is specified using <b>./zionplay no_of_fragments</b>.
  The no_of_fragments is multiplied by 1065 to get the portion size of the mp3 file to serve.
  
## COMPILING:
  compile using ./cyzion.sh. cyzion contains the commands to build and pack the executable.

### USE CASES:
could be used for TCP over UDP.
