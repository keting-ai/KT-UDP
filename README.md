# My UDP/IP
A UDP/IP protocol stack written in C. Works on linux.  
I wrote this project when I was a Freshman... please forgive a beginner who wasn't familiar with the Makefile. So there's no Makefile... I know it's unprofessional, but if you want to run this, please compile all the files (who tf will run this???)... sorry  
Also, there's no structured directory (unprofessional agian)... sorry  
Overall, this avoids using the protocols embedded in the Operating System. It can send valid UDP packets and process other UDP packets on the Internet. It can also forward packets.  
Not yet implemented:
- IP fragmentation
- A more efficient routetable
- Muti-threaded (this is important... but don't know that when writing this project)
- TCP protocol (too complicated... hope to get it done when I have time)

This is only tested on Ubuntu Linux.
