#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/ndnSIM-module.h>
#include <iostream>
#include "../utils/tracers/ndn-cs-tracer-powerful.hpp"

using namespace std;

namespace ns3 {

int main(int argc, char **argv) {

  CommandLine cmd;
  cmd.Parse(argc, argv);

  AnnotatedTopologyReader topologyReader("",25);
  topologyReader.SetFileName("src/ndnSIM/examples/topologies/topo-ibac-4.txt");
  NodeContainer nodes = topologyReader.Read();



  // NodeContainer authorizedcon;
  // NodeContainer unauthorizedcon;

  Ptr<Node> authorizedcon[6] = {Names::Find<Node>("AU1"),Names::Find<Node>("AU2"),Names::Find<Node>("AU3"),
                      Names::Find<Node>("AU4"),Names::Find<Node>("AU5"),Names::Find<Node>("AU6")};
  Ptr<Node> unauthorizedcon[6] = {Names::Find<Node>("UU1"),Names::Find<Node>("UU2"),Names::Find<Node>("UU3"),
                        Names::Find<Node>("UU4"),Names::Find<Node>("UU5"),Names::Find<Node>("UU6")};
  
  Ptr<Node> producer = Names::Find<Node>("pro1");

  // NodeContainer routers;
  // NodeContainer edgerouter;

  Ptr<Node> edgerouter[3] = {Names::Find<Node>("ER1"),Names::Find<Node>("ER2"),Names::Find<Node>("ER3")};
  Ptr<Node> routers = Names::Find<Node>("Rtr");

  
  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.SetOldContentStore("ns3::ndn::cs::Nocache");

  for (int i = 0; i < 6; i++) {
  ndnHelper.Install(authorizedcon[i]);
  ndnHelper.Install(unauthorizedcon[i]);
  ndn::StrategyChoiceHelper::Install(authorizedcon[i], "/xyz.com/au", "/localhost/nfd/strategy/best-route");
  ndn::StrategyChoiceHelper::Install(unauthorizedcon[i], "/xyz.com/unau", "/localhost/nfd/strategy/best-route");
}
  ndnHelper.Install(producer);
  ndn::StrategyChoiceHelper::Install(producer, "/xyz.com/au", "/localhost/nfd/strategy/best-route");
  


  //ndnHelper.SetOldContentStore("ns3::ndn::cs::Lru", "MaxSize", "200000");
  ndnHelper.SetOldContentStore("ns3::ndn::cs::Lru", "MaxSize", "10");
  ndnHelper.Install(routers);
  // Choosing forwarding strategy
  //ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/best-route");  
  ndn::StrategyChoiceHelper::Install(routers, "/xyz.com", "/localhost/nfd/strategy/best-route");
  
  for (int i = 0; i < 3; i++) {
  ndnHelper.Install(edgerouter[i]);
  ndn::StrategyChoiceHelper::Install(edgerouter[i], "/xyz.com", "/localhost/nfd/strategy/cencor-route");
  }


  // Installing global routing interface on all nodes
  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();



  ndn::AppHelper consumerHelper("ns3::ndn::ConsumerZipfMandelbrot");
  consumerHelper.SetAttribute("NumberOfContents", StringValue("100"));
  //consumerHelper.SetAttribute("NumberOfContentItems", StringValue("10"));
  consumerHelper.SetAttribute("Frequency", StringValue("11"));       //10 interests per seconds
  consumerHelper.SetPrefix("/xyz.com/au");
  
  for (int i = 0; i < 6; i++) {  
  consumerHelper.Install(authorizedcon[i]);
  }


//   for (int i = 1; i < 6; i=i+2) {  
//   consumerHelper.Install(unauthorizedcon[i]);
// }

  consumerHelper.SetPrefix("/xyz.com/unau");
  for (int i = 0; i < 6; i++) { 
  consumerHelper.Install(unauthorizedcon[i]);

  }

  ndn::AppHelper producerHelper("ns3::ndn::Producer");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));

  // Register /dst1 prefix with global routing controller and
  // install producer that will satisfy Interests in /dst1 namespace
  ndnGlobalRoutingHelper.AddOrigins("/xyz.com", producer);
  
  producerHelper.SetPrefix("/xyz.com");
  producerHelper.Install(producer);

  // Calculate and install FIBs
  ndn::GlobalRoutingHelper::CalculateRoutes();
  
  ndn::AppDelayTracer::InstallAll("EV_delay.txt");
  // ndn::L3RateTracer::InstallAll("taoyu3.txt");
  // L2RateTracer::InstallAll("taoyu2.txt");
  // ndn::CsTracer::InstallAll("rate-EV-10.txt", Seconds(0.1));
  Simulator::Stop(Seconds(300.0));
  Simulator::Run();
  Simulator::Destroy();
  return 0;
}

} // namespace ns3

int main(int argc, char **argv) {

  return ns3::main(argc, argv);
}
