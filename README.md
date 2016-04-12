# Arduino MySQL Temperature/Humidity Logger
## Get Extra Libraries
### Download/Install Arduino MySQL Connector
1. Download the .zip file from https://launchpad.net/mysql-arduino/release-1.0.4-ga/release-1.0.4-ga.
2. Un-zip the downloaded file and copy the two folders into ```Documents\Arduino\libraries```.

## Set Up Database
Import .sql file into MySQL

## Networking
By default, your Arduino will use DHCP to get an IP address. If you wish, you can give your Arduino a static IP address but it is only required on special networks.

## Misc.
### Logging Interval
You can modify the log interval by changing the following value found in the ```loop()``` near the bottom of the file:
```arduino
delay(284000);
```
> Note: This value is in seconds (the default value of 284000 converts to 5 minutes).

