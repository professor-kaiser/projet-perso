#!/bin/bash
sudo iptables -t filter -p tcp -m multiport --dports http,https -A INPUT  -j DROP  -s www.joyalestore.com
sudo iptables -t filter -p tcp -m multiport --dports http,https -A INPUT  -j DROP  -s www.youtube.com
