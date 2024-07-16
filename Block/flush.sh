#!/bin/bash

sudo iptables -t filter -P INPUT ACCEPT
sudo iptables -t filter -P OUTPUT ACCEPT