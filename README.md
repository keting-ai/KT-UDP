# My UDP/IP
A UDP/IP protocol stack written in C. Works on linux.  
I wrote this project when I was a beginner of C... not familiar with the Makefile. So there's no Makefile... I know it's not professional, but if you want to run this, please compile all the files (who tf will run this???)... sorry  
Overall, this avoids using the protocols embedded in the Operating System. It can send valid UDP packets and process other UDP packets on the Internet. It can also forward packets.  
Not yet implemented:
- IP fragmentation
- A more efficient routetable
- Muti-threaded (this is important... but don't know that when writing this project)
- TCP protocol (too complicated... hope to get it done when I have time)
This is only tested on Ubuntu Linux.
