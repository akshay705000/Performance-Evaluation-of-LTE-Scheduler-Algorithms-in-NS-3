#include "ns3/applications-module.h"
#include "ns3/config-store-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/lte-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/mobility-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"
#include "ns3/position-allocator.h"
#include "ns3/network-module.h"
#include "ns3/radio-bearer-stats-calculator.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor-helper.h"
#include <iomanip>
#include <string>

using namespace ns3;

/**
 * Sample simulation script for LTE+EPC. It instantiates 4 eNodeBs,
 * attaches five UE per eNodeB starts a flow for each UE to and from a remote host.
 */

NS_LOG_COMPONENT_DEFINE("LenaSimpleEpc");

void printStats(FlowMonitorHelper &flowmon_helper, bool Info, std::string schedInfo, double Interval, double speed)
{
    // Obtain the classifier and monitor from the FlowMonitorHelper
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon_helper.GetClassifier());
    std::string proto;
    Ptr<FlowMonitor> monitor = flowmon_helper.GetMonitor ();
    
    // Initialize variables to hold aggregate statistics
    std::map < FlowId, FlowMonitor::FlowStats > stats = monitor->GetFlowStats();
    double total_Time_Receiving;
    uint64_t total_Packets_Received, total_Packets_Dropped, total_Bytes_Received;

    total_Bytes_Received = 0, total_Packets_Dropped = 0, total_Packets_Received = 0, total_Time_Receiving = 0;
    double total_throughput=0;
    int n=0;
    for (std::map< FlowId, FlowMonitor::FlowStats>::iterator flow = stats.begin(); flow != stats.end(); flow++)
      { n++;
        int arj=1;
        while(arj--){
          Ipv4FlowClassifier::FiveTuple  t = classifier->FindFlow(flow->first); // Extract the 5-tuple from the flow ID
          switch(t.protocol)
           {
           case(6):
               proto = "TCP";
               break;
           case(17):
               proto = "UDP";
               std::cout << "UDP" << std::endl;
               break;
           default:
               exit(1);
           }

	   // Aggregate flow-level statistics
           total_Bytes_Received += (double) flow->second.rxBytes * 8;
           total_Time_Receiving += flow->second.timeLastRxPacket.GetSeconds ();
           total_Packets_Received += flow->second.rxPackets;
           total_Packets_Dropped += flow->second.txPackets - flow->second.rxPackets;

	   // Print per-flow information if requested
           if (Info) {
             std::cout << "FlowID: " << flow->first << " (" << proto << " "
                       << t.sourceAddress << " / " << t.sourcePort << " --> "
                       << t.destinationAddress << " / " << t.destinationPort << ")" << std::endl;
             std::cout << "  Tx Bytes: " << flow->second.txBytes << std::endl;
             std::cout << "  Rx Bytes: " << flow->second.rxBytes << std::endl;
             std::cout << "  Tx Packets: " << flow->second.txPackets << std::endl;
             std::cout << "  Rx Packets: " << flow->second.rxPackets << std::endl;
             std::cout << "  Time LastRxPacket: " << flow->second.timeLastRxPacket.GetSeconds () << "s" << std::endl;
             std::cout << "  Lost Packets: " << flow->second.lostPackets << std::endl;
             std::cout << "  Pkt Lost Ratio: " << ((double)flow->second.txPackets-(double)flow->second.rxPackets)/(double)flow->second.txPackets << std::endl;
             std::cout << "  Throughput: " << ( ((double)flow->second.rxBytes*8) / (flow->second.timeLastRxPacket.GetSeconds ()) ) << "bps" << std::endl;
             total_throughput+= ( ((double)flow->second.rxBytes*8) / (flow->second.timeLastRxPacket.GetSeconds ()) );
             std::cout << "  Mean{Delay}: " << (flow->second.delaySum.GetSeconds()/flow->second.rxPackets) << std::endl;
             std::cout << "  Mean{Jitter}: " << (flow->second.jitterSum.GetSeconds()/(flow->second.rxPackets)) << std::endl;
           }
       }

     }

     // Print aggregate statistics
     std::cout << speed << "\t" << Interval << "\t" << schedInfo << "\t"  << std::endl;
    
     std::cout<<"Average throughput= "<<total_throughput/n<<" bps\n";
}

int
main(int argc, char* argv[])
{
    // Defining variables.
    int RngRun1 = 22;
    double speed = 0.0;
    uint16_t numEnbNode = 4;
    uint16_t numUEnode = 20;
    double Enbdistance = 1000.0;
    double cellRadius = 500.0;
    bool disableDl = false;
    bool disableUl = true;
    bool disablePl = true;
    bool udpFullBuffer = true;
    Time simTime = Seconds(10.0);
    std::string schedType = "ns3::PfFfMacScheduler";
    Time interPacketInterval = MilliSeconds(10.0);
    char spd[100];
    sprintf(spd,"ns3::ConstantRandomVariable[Constant=%f]",speed);
 
    // Command line arguments
    CommandLine cmd(__FILE__);
    cmd.AddValue("schedType","Type of Scheduler",schedType);
    cmd.AddValue("speed","UE speed",speed);
    cmd.AddValue("RngRun1","Random seed",RngRun1);
    cmd.AddValue ("udpFullBuffer", "Whether to set the full buffer traffic or not.", udpFullBuffer);
    cmd.AddValue("numEnbNode", "Number of eNodeBs", numEnbNode);
    cmd.AddValue("numUEnode", "Number of UEs per eNodeBs", numUEnode);
    cmd.AddValue("distance", "Distance between eNBs [m]", Enbdistance);
    cmd.AddValue("simTime", "Total duration of the simulation [s])", simTime);
    cmd.Parse(argc, argv);

    ConfigStore inputConfig;
    inputConfig.ConfigureDefaults();
    
    // Setting seed value
    RngSeedManager::SetSeed(RngRun1);
    
    // Initializing variable for full buffer and half buffer case base on cmd value.
    if(udpFullBuffer){
     interPacketInterval = MilliSeconds(1.0);
    }
    else{
    interPacketInterval = MilliSeconds(10.0);
    }
    
    // Creating lteHelper and PointToPointEpcHelper object
    Ptr<LteHelper> lteHelper = CreateObject<LteHelper>();
    Ptr<PointToPointEpcHelper> epcHelper = CreateObject<PointToPointEpcHelper>();
    
    // Setting resource block value
    lteHelper->SetEnbDeviceAttribute ("DlBandwidth", UintegerValue (50));
    lteHelper->SetEnbDeviceAttribute ("UlBandwidth", UintegerValue (50));
    
    // setting epchelper
    lteHelper->SetEpcHelper(epcHelper);

    // setting the type of schedular based on input from cmd
    lteHelper->SetSchedulerType(schedType);
    
    // Create a single RemoteHost
    Ptr<Node> pgw = epcHelper->GetPgwNode();
    NodeContainer remoteHostContainer;
    remoteHostContainer.Create(1);
    Ptr<Node> remoteHost = remoteHostContainer.Get(0);

    // Creating the Internet
    InternetStackHelper internet;
    internet.Install(remoteHostContainer);

    //Creating a PointToPointHelper and setting its attributes
    PointToPointHelper p2ph;
    p2ph.SetDeviceAttribute("DataRate", DataRateValue(DataRate("1Gb/s")));
    p2ph.SetDeviceAttribute("Mtu", UintegerValue(1500));
    p2ph.SetChannelAttribute("Delay", TimeValue(MilliSeconds(10)));

    // Assigning IP address
    NetDeviceContainer internetDevices = p2ph.Install(pgw, remoteHost);
    Ipv4AddressHelper ipv4h;
    ipv4h.SetBase("1.0.0.0", "255.0.0.0");
    Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign(internetDevices);
    Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress(1);

    // Setting up routing
    Ipv4StaticRoutingHelper ipv4RoutingHelper;
    Ptr<Ipv4StaticRouting> remoteHostStaticRouting =
        ipv4RoutingHelper.GetStaticRouting(remoteHost->GetObject<Ipv4>());
    remoteHostStaticRouting->AddNetworkRouteTo(Ipv4Address("7.0.0.0"), Ipv4Mask("255.0.0.0"), 1);

    // Creating UEs and enodeBs
    NodeContainer ueNodes1;
    NodeContainer ueNodes2;
    NodeContainer ueNodes3;
    NodeContainer ueNodes4;
    NodeContainer enbNodes;
    enbNodes.Create(numEnbNode);
    ueNodes1.Create(numUEnode/4);
    ueNodes2.Create(numUEnode/4);
    ueNodes3.Create(numUEnode/4);
    ueNodes4.Create(numUEnode/4);

    // Setting eNodeBs network architecture.
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
    positionAlloc->Add(Vector(0, 0, 0)); // First eNodeB at origin
    positionAlloc->Add(Vector(Enbdistance, 0, 0));         // Second eNodeB
    positionAlloc->Add(Vector(0, Enbdistance, 0));          // Third eNodeB
    positionAlloc->Add(Vector(Enbdistance, Enbdistance, 0)); // Fourth eNodeB
    
    // Setting the mobility of eNodeBs
    MobilityHelper enbMobility;
    enbMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    enbMobility.SetPositionAllocator(positionAlloc);
    enbMobility.Install(enbNodes);

    // setting tx power of eNodeBs
    Config::SetDefault("ns3::LteEnbPhy::TxPower", DoubleValue(30.0));
    
    // Mobility helper for UEs
    MobilityHelper uemobility;

    // Setting mobility of UEs
    uemobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Mode", StringValue ("Time"),
                             "Time", StringValue ("2s"),
                             "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=0.0]"),
                             "Bounds", StringValue ("-500|1500|-500|1500"));
    
    // Setting position of UEs attached to eNB 1
    uemobility.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                 "X", DoubleValue(0.0),
                                 "Y", DoubleValue(0.0));
    Ptr<RandomVariableStream> theta1 = CreateObject<ConstantRandomVariable>();
    theta1->SetAttribute("Constant", DoubleValue(0.0)); 
    Ptr<RandomVariableStream> rho1 = CreateObject<ConstantRandomVariable>();
    rho1->SetAttribute("Constant", DoubleValue(cellRadius));
    uemobility.Install(ueNodes1);
    
    // Setting position of UEs attached to eNB 2
    uemobility.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                     "X",
                                     DoubleValue(Enbdistance),
                                     "Y",
                                     DoubleValue(0.0));                   
    Ptr<RandomVariableStream> theta2 = CreateObject<ConstantRandomVariable>();
    theta2->SetAttribute("Constant", DoubleValue(0.0));
    Ptr<RandomVariableStream> rho2 = CreateObject<ConstantRandomVariable>();
    rho2->SetAttribute("Constant", DoubleValue(cellRadius));    
    uemobility.Install(ueNodes2);
    
    // Setting position of UEs attached to eNB 3
    uemobility.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                     "X",
                                     DoubleValue(0.0),
                                     "Y",
                                     DoubleValue(Enbdistance));
    Ptr<RandomVariableStream> theta3 = CreateObject<ConstantRandomVariable>();
    theta3->SetAttribute("Constant", DoubleValue(0.0));
    Ptr<RandomVariableStream> rho3 = CreateObject<ConstantRandomVariable>();
    rho3->SetAttribute("Constant", DoubleValue(cellRadius)); 
    uemobility.Install(ueNodes3);
    
    // Setting position of UEs attached to eNB 4
    uemobility.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                     "X",
                                     DoubleValue(Enbdistance),
                                     "Y",
                                     DoubleValue(Enbdistance));    
    Ptr<RandomVariableStream> theta4 = CreateObject<ConstantRandomVariable>();
    theta4->SetAttribute("Constant", DoubleValue(0.0));    
    Ptr<RandomVariableStream> rho4 = CreateObject<ConstantRandomVariable>();
    rho4->SetAttribute("Constant", DoubleValue(cellRadius));     
    uemobility.Install(ueNodes4);
    
    // Install LTE Devices to the UEs and eNodeBs
    NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice(enbNodes);
    NetDeviceContainer ueLteDevs1 = lteHelper->InstallUeDevice(ueNodes1);
    NetDeviceContainer ueLteDevs2 = lteHelper->InstallUeDevice(ueNodes2);
    NetDeviceContainer ueLteDevs3 = lteHelper->InstallUeDevice(ueNodes3);
    NetDeviceContainer ueLteDevs4 = lteHelper->InstallUeDevice(ueNodes4);
    
    // Adding X2 interface for handovers.
    lteHelper->AddX2Interface(enbNodes);

    // Installing the IP stack on the UEs
    internet.Install(ueNodes1);
    Ipv4InterfaceContainer ueIpIface1;
    ueIpIface1 = epcHelper->AssignUeIpv4Address(NetDeviceContainer(ueLteDevs1));
    
    internet.Install(ueNodes2);
    Ipv4InterfaceContainer ueIpIface2;
    ueIpIface2 = epcHelper->AssignUeIpv4Address(NetDeviceContainer(ueLteDevs2));
    
    internet.Install(ueNodes3);
    Ipv4InterfaceContainer ueIpIface3;
    ueIpIface3 = epcHelper->AssignUeIpv4Address(NetDeviceContainer(ueLteDevs3));
    
    internet.Install(ueNodes4);
    Ipv4InterfaceContainer ueIpIface4;
    ueIpIface4 = epcHelper->AssignUeIpv4Address(NetDeviceContainer(ueLteDevs4));
    
    // Assigning IP address to UEs, and installing applications
    for (uint32_t u = 0; u < ueNodes1.GetN(); ++u)
    {
        Ptr<Node> ueNode = ueNodes1.Get(u);
        // Set the default gateway for the UE
        Ptr<Ipv4StaticRouting> ueStaticRouting =
            ipv4RoutingHelper.GetStaticRouting(ueNode->GetObject<Ipv4>());
        ueStaticRouting->SetDefaultRoute(epcHelper->GetUeDefaultGatewayAddress(), 1);
    }
    
    for (uint32_t u = 0; u < ueNodes2.GetN(); ++u)
    {
        Ptr<Node> ueNode = ueNodes2.Get(u);
        // Set the default gateway for the UE
        Ptr<Ipv4StaticRouting> ueStaticRouting =
            ipv4RoutingHelper.GetStaticRouting(ueNode->GetObject<Ipv4>());
        ueStaticRouting->SetDefaultRoute(epcHelper->GetUeDefaultGatewayAddress(), 1);
    }
    
    for (uint32_t u = 0; u < ueNodes3.GetN(); ++u)
    {
        Ptr<Node> ueNode = ueNodes3.Get(u);
        // Set the default gateway for the UE
        Ptr<Ipv4StaticRouting> ueStaticRouting =
            ipv4RoutingHelper.GetStaticRouting(ueNode->GetObject<Ipv4>());
        ueStaticRouting->SetDefaultRoute(epcHelper->GetUeDefaultGatewayAddress(), 1);
    }
    
    for (uint32_t u = 0; u < ueNodes4.GetN(); ++u)
    {
        Ptr<Node> ueNode = ueNodes4.Get(u);
        // Set the default gateway for the UE
        Ptr<Ipv4StaticRouting> ueStaticRouting =
            ipv4RoutingHelper.GetStaticRouting(ueNode->GetObject<Ipv4>());
        ueStaticRouting->SetDefaultRoute(epcHelper->GetUeDefaultGatewayAddress(), 1);
    }
    
    // Attaching UEs to eNodeBs
    lteHelper->AttachToClosestEnb(ueLteDevs1, enbLteDevs);
    lteHelper->AttachToClosestEnb(ueLteDevs2, enbLteDevs);
    lteHelper->AttachToClosestEnb(ueLteDevs3, enbLteDevs);
    lteHelper->AttachToClosestEnb(ueLteDevs4, enbLteDevs);
    
    // Installing and starting applications on UEs and remote hosts.
    uint16_t dlPort = 1100;
    uint16_t ulPort = 2000;
    uint16_t otherPort = 3000;
    ApplicationContainer clientApps;
    ApplicationContainer serverApps;

    for (uint32_t u = 0; u < ueNodes1.GetN(); ++u)
    {   
        // If downlink is enabled
        if (!disableDl)
        {
            PacketSinkHelper dlPacketSinkHelper("ns3::UdpSocketFactory",
                                                InetSocketAddress(Ipv4Address::GetAny(), dlPort));
            serverApps.Add(dlPacketSinkHelper.Install(ueNodes1.Get(u)));

            UdpClientHelper dlClient(ueIpIface1.GetAddress(u), dlPort);
            dlClient.SetAttribute("Interval", TimeValue(interPacketInterval));
            dlClient.SetAttribute("MaxPackets", UintegerValue(1000));
            dlClient.SetAttribute("PacketSize",UintegerValue(1500));
            clientApps.Add(dlClient.Install(remoteHost));
        }
        
        // If uplink is enabled
        if (!disableUl)
        {
            ++ulPort;
            PacketSinkHelper ulPacketSinkHelper("ns3::UdpSocketFactory",
                                                InetSocketAddress(Ipv4Address::GetAny(), ulPort));
            serverApps.Add(ulPacketSinkHelper.Install(remoteHost));

            UdpClientHelper ulClient(remoteHostAddr, ulPort);
            ulClient.SetAttribute("Interval", TimeValue(interPacketInterval));
            ulClient.SetAttribute("MaxPackets", UintegerValue(1000));
            ulClient.SetAttribute("PacketSize",UintegerValue(1500));
            clientApps.Add(ulClient.Install(ueNodes1.Get(u)));
        }
        
        // If number of enode is more than 1 and other communications required 
        if (!disablePl && numEnbNode > 1)
        {
            ++otherPort;
            PacketSinkHelper packetSinkHelper("ns3::UdpSocketFactory",
                                              InetSocketAddress(Ipv4Address::GetAny(), otherPort));
            serverApps.Add(packetSinkHelper.Install(ueNodes1.Get(u)));

            UdpClientHelper client(ueIpIface1.GetAddress(u), otherPort);
            client.SetAttribute("Interval", TimeValue(interPacketInterval));
            client.SetAttribute("MaxPackets", UintegerValue(1000));
            client.SetAttribute("PacketSize",UintegerValue(1500));
            clientApps.Add(client.Install(ueNodes1.Get((u + 1) % numEnbNode)));
        }
    }
    
    for (uint32_t u = 0; u < ueNodes2.GetN(); ++u)
    {
        // If downlink is enabled
        if (!disableDl)
        {
            PacketSinkHelper dlPacketSinkHelper("ns3::UdpSocketFactory",
                                                InetSocketAddress(Ipv4Address::GetAny(), dlPort));
            serverApps.Add(dlPacketSinkHelper.Install(ueNodes2.Get(u)));

            UdpClientHelper dlClient(ueIpIface2.GetAddress(u), dlPort);
            dlClient.SetAttribute("Interval", TimeValue(interPacketInterval));
            dlClient.SetAttribute("MaxPackets", UintegerValue(1000));
            dlClient.SetAttribute("PacketSize",UintegerValue(1500));
            clientApps.Add(dlClient.Install(remoteHost));
        }

        // If uplink is enabled
        if (!disableUl)
        {
            ++ulPort;
            PacketSinkHelper ulPacketSinkHelper("ns3::UdpSocketFactory",
                                                InetSocketAddress(Ipv4Address::GetAny(), ulPort));
            serverApps.Add(ulPacketSinkHelper.Install(remoteHost));

            UdpClientHelper ulClient(remoteHostAddr, ulPort);
            ulClient.SetAttribute("Interval", TimeValue(interPacketInterval));
            ulClient.SetAttribute("MaxPackets", UintegerValue(1000));
            ulClient.SetAttribute("PacketSize",UintegerValue(1500));
            clientApps.Add(ulClient.Install(ueNodes2.Get(u)));
        }

        // If number of enode is more than 1 and other communications required
        if (!disablePl && numEnbNode > 1)
        {
            ++otherPort;
            PacketSinkHelper packetSinkHelper("ns3::UdpSocketFactory",
                                              InetSocketAddress(Ipv4Address::GetAny(), otherPort));
            serverApps.Add(packetSinkHelper.Install(ueNodes2.Get(u)));

            UdpClientHelper client(ueIpIface2.GetAddress(u), otherPort);
            client.SetAttribute("Interval", TimeValue(interPacketInterval));
            client.SetAttribute ("MaxPackets", UintegerValue(1000));
            client.SetAttribute("PacketSize",UintegerValue(1500));
            clientApps.Add(client.Install(ueNodes2.Get((u + 1) % numEnbNode)));
        }
    }
    
    for (uint32_t u = 0; u < ueNodes3.GetN(); ++u)
    {
        // If downlink is enabled
        if (!disableDl)
        {
            PacketSinkHelper dlPacketSinkHelper("ns3::UdpSocketFactory",
                                                InetSocketAddress(Ipv4Address::GetAny(), dlPort));
            serverApps.Add(dlPacketSinkHelper.Install(ueNodes3.Get(u)));

            UdpClientHelper dlClient(ueIpIface3.GetAddress(u), dlPort);
            dlClient.SetAttribute("Interval", TimeValue(interPacketInterval));
            dlClient.SetAttribute("MaxPackets", UintegerValue(1000));
            dlClient.SetAttribute("PacketSize",UintegerValue(1500));
            clientApps.Add(dlClient.Install(remoteHost));
        }

        // If uplink is enabled
        if (!disableUl)
        {
            ++ulPort;
            PacketSinkHelper ulPacketSinkHelper("ns3::UdpSocketFactory",
                                                InetSocketAddress(Ipv4Address::GetAny(), ulPort));
            serverApps.Add(ulPacketSinkHelper.Install(remoteHost));

            UdpClientHelper ulClient(remoteHostAddr, ulPort);
            ulClient.SetAttribute("Interval", TimeValue(interPacketInterval));
            ulClient.SetAttribute("MaxPackets", UintegerValue(1000));
            ulClient.SetAttribute("PacketSize",UintegerValue(1500));
            clientApps.Add(ulClient.Install(ueNodes3.Get(u)));
        }

        // If number of enode is more than 1 and other communications required
        if (!disablePl && numEnbNode > 1)
        {
            ++otherPort;
            PacketSinkHelper packetSinkHelper("ns3::UdpSocketFactory",
                                              InetSocketAddress(Ipv4Address::GetAny(), otherPort));
            serverApps.Add(packetSinkHelper.Install(ueNodes3.Get(u)));

            UdpClientHelper client(ueIpIface4.GetAddress(u), otherPort);
            client.SetAttribute("Interval", TimeValue(interPacketInterval));
            client.SetAttribute ("MaxPackets", UintegerValue(1000));
            client.SetAttribute("PacketSize",UintegerValue(1500));
            clientApps.Add(client.Install(ueNodes3.Get((u + 1) % numEnbNode)));
        }
    }
    
    for (uint32_t u = 0; u < ueNodes4.GetN(); ++u)
    {
        // If downlink is enabled
        if (!disableDl)
        {
            PacketSinkHelper dlPacketSinkHelper("ns3::UdpSocketFactory",
                                                InetSocketAddress(Ipv4Address::GetAny(), dlPort));
            serverApps.Add(dlPacketSinkHelper.Install(ueNodes4.Get(u)));

            UdpClientHelper dlClient(ueIpIface4.GetAddress(u), dlPort);
            dlClient.SetAttribute("Interval", TimeValue(interPacketInterval));
            dlClient.SetAttribute("MaxPackets", UintegerValue(1000));
            dlClient.SetAttribute("PacketSize",UintegerValue(1500));
            clientApps.Add(dlClient.Install(remoteHost));
        }

        // If uplink is enabled
        if (!disableUl)
        {
            ++ulPort;
            PacketSinkHelper ulPacketSinkHelper("ns3::UdpSocketFactory",
                                                InetSocketAddress(Ipv4Address::GetAny(), ulPort));
            serverApps.Add(ulPacketSinkHelper.Install(remoteHost));

            UdpClientHelper ulClient(remoteHostAddr, ulPort);
            ulClient.SetAttribute("Interval", TimeValue(interPacketInterval));
            ulClient.SetAttribute("MaxPackets", UintegerValue(1000));
            ulClient.SetAttribute("PacketSize",UintegerValue(1500));
            clientApps.Add(ulClient.Install(ueNodes4.Get(u)));
        }
        
        // If number of enode is more than 1 and other communications required
        if (!disablePl && numEnbNode > 1)
        {
            ++otherPort;
            PacketSinkHelper packetSinkHelper("ns3::UdpSocketFactory",
                                              InetSocketAddress(Ipv4Address::GetAny(), otherPort));
            serverApps.Add(packetSinkHelper.Install(ueNodes4.Get(u)));

            UdpClientHelper client(ueIpIface4.GetAddress(u), otherPort);
            client.SetAttribute("Interval", TimeValue(interPacketInterval));
            client.SetAttribute ("MaxPackets", UintegerValue(1000));
            client.SetAttribute("PacketSize",UintegerValue(1500));
            clientApps.Add(client.Install(ueNodes4.Get((u + 1) % numEnbNode)));
        }
    }

    // Starting applications
    serverApps.Start(MilliSeconds(500));
    clientApps.Start(MilliSeconds(500));
    lteHelper->EnableTraces();
    
    //creating a flowmonitorhelper object to get the track of flows.
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> Monitor;

    // installing devices
    Monitor = flowmon.Install(ueNodes1);
    Monitor = flowmon.Install(ueNodes2);
    Monitor = flowmon.Install(ueNodes3);
    Monitor = flowmon.Install(ueNodes4);
    Monitor = flowmon.Install(remoteHostContainer);

    Simulator::Stop(simTime);
    
    //* remove this comments for graph 1 i.e., getting rems2.out
    
    // creating RadioEnvironmentMapHelper object to get the details of REM
    Ptr<RadioEnvironmentMapHelper> remHelper = CreateObject<RadioEnvironmentMapHelper> ();
    remHelper->SetAttribute("Channel", PointerValue(lteHelper->GetDownlinkSpectrumChannel()));
    remHelper->SetAttribute ("OutputFile", StringValue ("rems2.out"));
    remHelper->SetAttribute ("XMin", DoubleValue (-500.0));
    remHelper->SetAttribute ("XMax", DoubleValue (1500.0));
    remHelper->SetAttribute ("XRes", UintegerValue (100));
    remHelper->SetAttribute ("YMin", DoubleValue (-500.0));
    remHelper->SetAttribute ("YMax", DoubleValue (1500.0));
    remHelper->SetAttribute ("YRes", UintegerValue (75));
    remHelper->SetAttribute ("Z", DoubleValue (0.0));
    //remHelper->SetAttribute ("UseDataChannel", BooleanValue (true));
    remHelper->SetAttribute ("RbId", IntegerValue (10));
    remHelper->Install ();
    
    //Above comment is done till here for other graphs
  
    // Running simulation
    Simulator::Run();
    
    // Printing stats
    //double Interval = interPacketInterval.GetSeconds();  //Comment this line for graph1
    //printStats(flowmon, true, schedType, Interval, speed);  //Comment this line for graph1

    Simulator::Destroy();
    return 0;
}
