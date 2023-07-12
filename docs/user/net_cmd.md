Network Commands	{#network_commands}
================

@tableofcontents

@section net_ping ping(for w1 shell)
#### Ping is a network software utility used to test the reachability of a host on an Internet Protocol (IP) network. ####

Usage:
	ping <hostname/ip address>
	[-c count]
	[-s packetsize(byte)] [-t ttl]

	ping stop

#### Here is an example. ####

	ping www.baidu.com -c 5 -s 200

	PING www.baidu.com (180.97.34.96) 200(228) bytes of data.
	ping: recv 180.97.34.96 142 ms
	208 bytes from 180.97.34.96 : icmp_seq=1 ttl=52 time=142 ms
	ping: recv 180.97.34.96 57 ms
	208 bytes from 180.97.34.96 : icmp_seq=2 ttl=52 time=57 ms
	ping: recv 180.97.34.96 91 ms
	208 bytes from 180.97.34.96 : icmp_seq=3 ttl=52 time=91 ms
	ping: recv 180.97.34.96 63 ms
	208 bytes from 180.97.34.96 : icmp_seq=4 ttl=52 time=63 ms
	ping: recv 180.97.34.96 56 ms
	208 bytes from 180.97.34.96 : icmp_seq=5 ttl=52 time=56 ms
	--- www.baidu.com ping statistics ---
	5 packets transmitted, 5 received, 0.00% packet loss

	> ping www.baidu.com

	PING www.baidu.com (180.97.34.96) 32(60) bytes of data.
	ping: recv 180.97.34.96 99 ms
	40 bytes from 180.97.34.96 : icmp_seq=1 ttl=52 time=99 ms
	ping: recv 180.97.34.96 52 ms
	40 bytes from 180.97.34.96 : icmp_seq=2 ttl=52 time=52 ms
	ping: recv 180.97.34.96 61 ms
	40 bytes from 180.97.34.96 : icmp_seq=3 ttl=52 time=61 ms
	ping: recv 180.97.34.96 54 ms
	40 bytes from 180.97.34.96 : icmp_seq=4 ttl=52 time=54 ms
	--- www.baidu.com ping statistics ---
	4 packets transmitted, 4 received, 0.00% packet loss

	> ping stop
	--- www.baidu.com ping statistics ---
	4 packets transmitted, 4 received, 0.00% packet loss

@section net_ping6 ping6(for w1 shell)
#### Ping6 is a network software utility used to test the reachability of a host on an Internet Protocol (IPv6) network. ####

Usage:
	ping6 <hostname/ip address>
	[-c count]
	[-s packetsize(byte)] [-t ttl]

	ping6 stop

#### Here is an example. ####

	> ping6 ipv6.ustb.edu.cn

	PING ipv6.ustb.edu.cn (2001:DA8:208:10::14) 32(80) bytes of data.
	40 bytes from 2001:DA8:208:10::14 : icmp_seq=1  time=154 ms
	40 bytes from 2001:DA8:208:10::14 : icmp_seq=2  time=101 ms
	40 bytes from 2001:DA8:208:10::14 : icmp_seq=3  time=79 ms
	40 bytes from 2001:DA8:208:10::14 : icmp_seq=4  time=120 ms
	--- ipv6.ustb.edu.cn ping statistics ---
	4 packets transmitted, 4 received, 0.00% packet loss

	> ping6 2400:3200::1

	PING 2400:3200::1 (2400:3200::1) 32(80) bytes of data.
	40 bytes from 2400:3200::1 : icmp_seq=1  time=89 ms
	40 bytes from 2400:3200::1 : icmp_seq=2  time=82 ms
	40 bytes from 2400:3200::1 : icmp_seq=3  time=75 ms
	40 bytes from 2400:3200::1 : icmp_seq=4  time=79 ms
	--- 2400:3200::1 ping statistics ---
	4 packets transmitted, 4 received, 0.00% packet loss

@section net_ifconfig ifconfig (for w1 shell)
#### List all interfaces those are up.

	ifconfig

#### Here is an example. ####

	> ifconfig
      loopwl1:  flags=31<UP,BROADCAST,RUNNING>  mtu 1476
      inet 10.98.32.208  netmask 255.255.255.0
      gateway 10.98.32.1  broadcast 10.98.32.255
	  (ethernet)      ether 08:09:0a:58:16:05
	  lo0:  flags=5<UP,LOOPBACK,RUNNING>  mtu 0
      inet 127.0.0.1  netmask 255.0.0.0
      gateway 127.0.0.1
      (loopback)

@section net_ping_broadcom ping(for broadcom shell)

#### Ping is a network software utility used to test the reachability of a host on an Internet Protocol (IP) network. ####

Usage:
	ping <hostname/ip address>
	[-c count]
	[-l packetsize(byte)] [-e terminate]

#### Here is an example. ####

	> ping www.baidu.com -c 10 -l 1300
	PING www.baidu.com(14.215.177.38) 1300(1328) bytes of data.
	1308 bytes from 14.215.177.38: icmp_seq=1 ttl=53 time=76ms
	1308 bytes from 14.215.177.38: icmp_seq=2 ttl=53 time=90ms
	1308 bytes from 14.215.177.38: icmp_seq=3 ttl=53 time=94ms
	1308 bytes from 14.215.177.38: icmp_seq=4 ttl=53 time=97ms
	1308 bytes from 14.215.177.38: icmp_seq=5 ttl=53 time=89ms
	1308 bytes from 14.215.177.38: icmp_seq=6 ttl=53 time=93ms
	1308 bytes from 14.215.177.38: icmp_seq=7 ttl=53 time=96ms
	1308 bytes from 14.215.177.38: icmp_seq=8 ttl=53 time=99ms
	1308 bytes from 14.215.177.38: icmp_seq=9 ttl=53 time=92ms
	1308 bytes from 14.215.177.38: icmp_seq=10 ttl=53 time=95ms

	--- www.baidu.com ping statistics ---
	10 packets transmitted, 10 received, 0% packet loss, time 9978ms

	> ping www.baidu.com

	PING www.baidu.com(14.215.177.39) 64(92) bytes of data.
	72 bytes from 14.215.177.39: icmp_seq=1 ttl=53 time=101ms
	72 bytes from 14.215.177.39: icmp_seq=2 ttl=53 time=94ms
	72 bytes from 14.215.177.39: icmp_seq=3 ttl=53 time=97ms
	72 bytes from 14.215.177.39: icmp_seq=4 ttl=53 time=89ms

	> ping -e
	--- www.baidu.com ping statistics ---
	4 packets transmitted, 4 received, 0% packet loss, time 4408ms

@section net_iperf_broadcom iperf (for broadcom shell)
#### iperf is used to perform speed test.

Usage:
	iperf-tx <hostname/ip address> time(in seconds)
	iperf-rx start/stop

#### Here is an example. ####

	> iperf-tx 192.168.1.223 10
	start iperf client, server ip = 192.168.1.232,time = 10s
	start iperf client finished.
	!!! enter function: lwiperf_tcp_close reason=1 !!!

	> iperf-rx start
	!!! enter function: lwiperf_tcp_accept !!!

	> iperf-rx stop

@section net_ifconfig_broadcom ifconfig (for broadcom shell)
#### List all interfaces those are up.

	ifconfig

#### Here is an example. ####

	> ifconfig
	wi:
	mac=28:ed:e0:64:37:50
	ip=192.168.1.162
	gw=192.168.1.1
	mask=255.255.255.0
