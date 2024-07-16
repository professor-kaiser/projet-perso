#!/bin/bash
sudo iptables -t filter -p tcp -m multiport --dports http,https -A OUTPUT  -j DROP  -d www.joyalestore.com
sudo iptables -t filter -p tcp -m multiport --dports http,https -A OUTPUT  -j DROP  -d www.youtube.com
