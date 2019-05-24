# AltiPi
Basic Altimeter logger for SF11/C laser range finder.

- Using Raspberry Pi Zero
- Trigger on specified pin interrupt 
- Retrieve Altitude via serial port
- Log Altitudes.


REQUIRES:
---------
- Rasbian Stretch 4.14
- wiringPi library installed system wide. 
- libterraclear: Clone https://github.com/terraclear/libterraclear into the same root folder as this repo

HARDWARE:
---------
A Raspberry Pi Zero, wired up as follows:
Lightware SF11 Laser Altimeter whose RX line is attached to TX, TX line is attached to RX, connected to 5 Volt power and GND.
Camera trigger line connected to pin #21
Red LED connected to #24 and GND.
Green LED connected to #21 and GND.


RASPBERRY DEPLOYMENT NOTES:
-----------------
- SSH: user=pi, password=terrapi123
- Deploy altipi binary to /altimeter
- Create a system.d service for altipi and register service (see /altipi.service in repository.)
- Mount FAT partition on /media/data through /etc/fstab
-     PARTUUID=4955b1f2-03  /media/data     vfat    defaults,umask=007,gid=100 0 0
- In mounted FAT partition, Create folder "altimeter"
-     /media/data/altimeter

DEPLOY SYSTEM.D SERVICE
-----------------------
- deploy altipi.service file to /lib/systemd/system/altipi.service
- Ensure rights are 644:
-     sudo chmod 644 /lib/systemd/system/altipi.service
- configure system.d: 
-     sudo systemctl daemon-reload
-     sudo systemctl enable altipi.service
- reboot

VALIDATE RELEASE
----------------
On startup, expect ```ps -A | grep altipi``` to find a running process.
Expect ```/media/data/altimeter``` to contain ```altimeter.log``` and ```altimeterdebug.log```.
Expect green LED to turn on.
Expect ```altimeterdebug.log``` to contain readings and continuously grow.
