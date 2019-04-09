# AltiPi
Basic Altimeter logger for SF11/C laser range finder.

- Using Raspberry Pi Zero
- Trigger on specified pin interrupt 
- Retrieve Altitude via serial port
- Log Altitudes.


REQUIRES:
---------
- libterraclear: Clone https://github.com/terraclear/libterraclear into the same root folder as this repo


RASPBERRY DEPLOYMENT NOTES:
-----------------
- SSH: user=pi, password=terrapi123
- Deploy altipi binary to /altimeter
- Create a system.d service for altipi and register service

DEPLOY SYSTEM.D SERVICE
-----------------------
- deploy altipi.service file to /lib/systemd/system/altipi.service
- Ensure rights are 644:
-                     sudo chmod 644 /lib/systemd/system/altipi.service
- configure system.d: 
-                     sudo systemctl daemon-reload
-                     sudo systemctl enable sample.service
- reboot
