# IBAC-EV
Interest-Based Access Control via Edge Verification in Named Data Networking
## Statement: The code works with the version 2.3 of ndnSIM.
The given three cpp files, ENC, IBAC, EV reproduce three scenarios in the baselines and our schemes, respectively, in terms of network traffic.
## step1：Preparation
### To run these three scenarios, you have to first save our given files to the corresponding path
### save the file ENC.cpp, IBAC.cpp, EV.cpp to the path ns3/ndnSIM/examples
### save the file topo-ibac-4.txt to the path ns3/ndnSIM/examples/topologies
### save the file forwarder.cpp, cencor-route-strategy.cpp, cencor-route-strategy.hpp to the path ns3/ndnSIM/NFD/daemon/fw
## step2：Normally, you can run scenarios directly and observe the packet forwarding through visualizer.
'cd ndnSIM/ns-3/src ./waf --run <scnario> --vis'
