# AltiPi
Basic Altimeter logger for SF11/C laser range finder.

- Using Raspberry Pi Zero
- Trigger on specified pin interrupt 
- Retrieve Altitude via serial port
- Log Altitudes.


REQUIRES:
---------
- libterraclear: Clone https://github.com/terraclear/libterraclear into the same root folder as this repo


DEPLOYMENT NOTES:
-----------------
- user: pi
- password: terrapi123
- Deploy binary to any folder on SD card, default is /altimeter
- Create a system.d service for altimeter binary and register service

DEPLOY TO SYSTEM.D
------------------
- copy altipi.service file to /lib/systemd/system/
- Ensure rights are 644, i.e.: sudo chmod 644 /lib/systemd/system/altipi.service
- configure system.d: 
-                     sudo systemctl daemon-reload
-                     sudo systemctl enable sample.service
- reboot
