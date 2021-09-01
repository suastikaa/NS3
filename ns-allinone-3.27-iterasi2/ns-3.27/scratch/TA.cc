
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/wifi-module.h"
#include "ns3/energy-module.h"
#include "ns3/internet-module.h"
#include "ns3/olsr-helper.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-list-routing-helper.h"
//#include "ns3/config-store-module.h"
#include "ns3/netanim-module.h"
#include "ns3/animation-interface.h"
#include "ns3/applications-module.h"



//
//#include "crypto++/osrng.h"
//
//#include "crypto++/filters.h"
//using CryptoPP::StringSink;
//using CryptoPP::StringSource;
//using CryptoPP::StreamTransformationFilter;

//#include "crypto++/hex.h"
//
//#include "crypto++/rc5.h"
//
//#include "crypto++/aes.h"
//using CryptoPP::AES;

//#include "crypto++/cryptlib.h"

//#include "crypto++/ccm.h"
//using CryptoPP::CBC_Mode;

#include "assert.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <fstream>
#include <vector>
#include <string>
using std::string;


NS_LOG_COMPONENT_DEFINE ("EnergyExample");

using namespace ns3;
//using namespace CryptoPP;

static inline std::string
PrintReceivedPacket (Address& from)
{
  InetSocketAddress iaddr = InetSocketAddress::ConvertFrom (from);

  std::ostringstream oss;
  oss << "--\nReceived one packet! Socket: " << iaddr.GetIpv4 ()
      << " port: " << iaddr.GetPort ()
      << " at time = " << Simulator::Now ().GetSeconds ()
      << "\n--";

  return oss.str ();
}

double energyConstant = 0;
/**
 * \param socket Pointer to socket.
 *
 * Packet receiving sink.
 */
void
ReceivePacket (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  Address from;
  while ((packet = socket->RecvFrom (from)))
    {
      if (packet->GetSize () > 0)
        {
          NS_LOG_UNCOND (PrintReceivedPacket (from));
        }
    }
}

/**
 * \param socket Pointer to socket.
 * \param pktSize Packet size.
 * \param n Pointer to node.
 * \param pktCount Number of packets to generate.
 * \param pktInterval Packet sending interval.
 *
 * Traffic generator.
 */
static void
GenerateTraffic (Ptr<Socket> socket, uint32_t pktSize, Ptr<Node> n,
                 uint32_t pktCount, Time pktInterval)
{
  if (pktCount > 0)
    {
      socket->Send (Create<Packet> (pktSize));
      Simulator::Schedule (pktInterval, &GenerateTraffic, socket, pktSize, n,
                           pktCount - 1, pktInterval);
    }
  else
    {
      socket->Close ();
    }
}

/// Trace function for remaining energy at node.
void
RemainingEnergy (double oldValue, double remainingEnergy)
{
  NS_LOG_UNCOND (Simulator::Now ().GetSeconds ()
                 << "s Current remaining energy = " << remainingEnergy-(energyConstant*Simulator::Now ().GetSeconds ()) << "J");
}

/// Trace function for total energy consumption at node.
void
TotalEnergy (double oldValue, double totalEnergy)
{
  NS_LOG_UNCOND (Simulator::Now ().GetSeconds ()
                 << "s Total energy consumed by radio = " << totalEnergy+(energyConstant*Simulator::Now ().GetSeconds ()) << "J");
}

int
main (int argc, char *argv[])
{
  /*
  LogComponentEnable ("EnergySource", LOG_LEVEL_DEBUG);
  LogComponentEnable ("BasicEnergySource", LOG_LEVEL_DEBUG);
  LogComponentEnable ("DeviceEnergyModel", LOG_LEVEL_DEBUG);
  LogComponentEnable ("WifiRadioEnergyModel", LOG_LEVEL_DEBUG);
   */

  std::string phyMode ("DsssRate1Mbps");
  double Prss = -80;            // dBm
  uint32_t PpacketSize = 200;   // bytes
  bool verbose = false;
//  bool tracing = false;

  // simulation parameters
  uint32_t numPackets = 100;  // number of packets to send
  double interval = 1;          // seconds
  double startTime = 0.0;       // seconds
  double distanceToRx = 100.0;  // meters
  /*
   * This is a magic number used to set the transmit power, based on other
   * configuration.
   */
  double offset = 81;

  CommandLine cmd;
  cmd.AddValue ("phyMode", "Wifi Phy mode", phyMode);
  cmd.AddValue ("Prss", "Intended primary RSS (dBm)", Prss);
  cmd.AddValue ("PpacketSize", "size of application packet sent", PpacketSize);
  cmd.AddValue ("numPackets", "Total number of packets to send", numPackets);
  cmd.AddValue ("startTime", "Simulation start time", startTime);
  cmd.AddValue ("distanceToRx", "X-Axis distance between nodes", distanceToRx);
  cmd.AddValue ("verbose", "Turn on all device log components", verbose);
  cmd.Parse (argc, argv);

  // Convert to time object
  Time interPacketInterval = Seconds (interval);

  // disable fragmentation for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold",
                      StringValue ("2200"));
  // turn off RTS/CTS for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold",
                      StringValue ("2200"));
  // Fix non-unicast data rate to be the same as that of unicast
  Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode",
                      StringValue (phyMode));

// code for encryption -----------------------------------------------------------

//  AutoSeededRandomPool prng;
//
//  byte key[RC5::DEFAULT_KEYLENGTH];
//  prng.GenerateBlock(key, sizeof(key));
//
//  byte iv[RC5::BLOCKSIZE];
////  byte iv[4];
//  prng.GenerateBlock(iv, sizeof(iv));

  string plain = "ini pesan asli";
  cout << "pesan: " << plain << endl;
//  string cipher, encoded, recovered;

  /*-------------------encrypt message---------------------*/
//  encoded.clear();
//  StringSource SSkey(key, sizeof(key), true,
//		  new HexEncoder(
//				  new StringSink(encoded)
//		  )
//  );
//  cout << "key: " << encoded << endl;
//
//  encoded.clear();
//  StringSource SSiv(iv, sizeof(iv), true,
//		  new HexEncoder(
//				  new StringSink(encoded)
//		  )
//  );
//  cout << "IV: " << encoded << endl;
//  try{
//	  CBC_Mode< RC5 >::Encryption e;
//	  e.SetKeyWithIV(key, sizeof(key), iv);
//	  StringSource s(plain, true,
//			  new StreamTransformationFilter(e,
//					  new StringSink(cipher)));
//
//#if 0
//	  StreamTransformatinFilter filter(e);
//	  filter.Put((const byte*)plain.data(), plain.size());
//	  filter.MessageEnd();
//
//	  const size_t ret = filter.MaxRetrievable();
//	  cipher.resize(ret);
//	  filter.Get((byte*)cipher.data(), cipher.size());
//#endif
//  }
//  catch(const CryptoPP::Exception& e)
//  {
//	  cerr << e.what() << endl;
//	  exit(1);
//  }
  /********************************************************/

//  encoded.clear();
//  StringSource(cipher, true,
//		  new HexEncoder(
//				  new StringSink(encoded)));
//  cout << "cipher text: " << encoded << endl;

  /*------------------decrypt message---------------------*/
//  try{
//	  CBC_Mode< RC5 >::Decryption d;
//	  d.SetKeyWithIV(key, sizeof(key), iv);
//	  StringSource s(cipher, true,
//			  new StreamTransformationFilter(d,
//					  new StringSink(recovered)));
//#if 0
//	  StreamTransformationFilter filter(d);
//	  filter.Put((const byte*)cipher.data(), cipher.size());
//	  filter.MessageEnd();
//
//	  const size_t ret = filter.MaxRetrievable();
//	  recovered.resize(ret);
//	  filter.Get((byte*)recovered.data(), recovered.size());
//#endif
//	  cout << "recovered text: " << recovered << endl;
//  }
//  catch(const CryptoPP::Exception& e){
//	  cerr << e.what() << endl;
//	  exit(1);
//  }

// end of encryption code --------------------------------------------------------

// message distribution ----------------------------------------------------------
  /*sending content*/
//  std::ostringstream msg;
//  msg << encoded << '\0';
//  Ptr<Packet> packet = Create<Packet>((uint8_t*) msg.str().c_str(), msg.str().length());
//
//  /*read content*/
//  uint8_t *buffer = new uint8_t[packet->GetSize()];
//  packet->CopyData(buffer, packet->GetSize());
//  std::string s = std::string((char*) buffer);
// end of message distribution --------------------------------------------------

  NodeContainer c;
  c.Create (15);     // create 2 nodes
  NodeContainer networkNodes;
  networkNodes.Add (c.Get (0));
  networkNodes.Add (c.Get (1));
  networkNodes.Add (c.Get (2));
  networkNodes.Add (c.Get (3));
  networkNodes.Add (c.Get (4));
  networkNodes.Add (c.Get (5));
  networkNodes.Add (c.Get (6));
  networkNodes.Add (c.Get (7));
  networkNodes.Add (c.Get (8));
  networkNodes.Add (c.Get (9));
  networkNodes.Add (c.Get (10));
  networkNodes.Add (c.Get (11));
  networkNodes.Add (c.Get (12));
  networkNodes.Add (c.Get (13));
  networkNodes.Add (c.Get (14));

  // The below set of helpers will help us to put together the wifi NICs we want
  WifiHelper wifi;
  if (verbose)
    {
      wifi.EnableLogComponents ();
    }
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);

  /** Wifi PHY **/
  /***************************************************************************/
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  wifiPhy.Set ("RxGain", DoubleValue (-10));
//  wifiPhy.Set ("TxGain", DoubleValue (offset + Prss));
  wifiPhy.Set ("TxGain", DoubleValue (offset + Prss-15.0));
  wifiPhy.Set ("CcaMode1Threshold", DoubleValue (0.0));
  /***************************************************************************/

  /** wifi channel **/
  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss ("ns3::FriisPropagationLossModel");
  // create wifi channel
  Ptr<YansWifiChannel> wifiChannelPtr = wifiChannel.Create ();
  wifiPhy.SetChannel (wifiChannelPtr);

  /** MAC layer **/
  // Add a non-QoS upper MAC, and disable rate control
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode",
                                StringValue (phyMode), "ControlMode",
                                StringValue (phyMode));
  // Set it to ad-hoc mode
  wifiMac.SetType ("ns3::AdhocWifiMac");

  /** install PHY + MAC **/
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, networkNodes);

  /** mobility **/
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (60.0, 345.0, 0.0));  //0
  positionAlloc->Add (Vector (138.0, 276.0, 0.0)); //1
  positionAlloc->Add (Vector (177.0, 381.0, 0.0)); //2
  positionAlloc->Add (Vector (247.0, 278.0, 0.0)); //3
  positionAlloc->Add (Vector (257.0, 197.0, 0.0)); //4
  positionAlloc->Add (Vector (338.0, 222.0, 0.0)); //5
  positionAlloc->Add (Vector (361.0, 177.0, 0.0)); //6
  positionAlloc->Add (Vector (441.0, 209.0, 0.0)); //7
  positionAlloc->Add (Vector (268.0, 421.0, 0.0)); //8
  positionAlloc->Add (Vector (330.0, 467.0, 0.0)); //9
  positionAlloc->Add (Vector (407.0, 381.0, 0.0)); //10
  positionAlloc->Add (Vector (460.0, 364.0, 0.0)); //11
  positionAlloc->Add (Vector (500.0, 376.0, 0.0)); //12
  positionAlloc->Add (Vector (487.0, 419.0, 0.0)); //13
  positionAlloc->Add (Vector (401.0, 287.0, 0.0)); //14

  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (c);

  // Enable OLSR
  OlsrHelper olsr;
  Ipv4StaticRoutingHelper staticRouting;

  Ipv4ListRoutingHelper list;
    list.Add (staticRouting, 0);
    list.Add (olsr, 10);

    InternetStackHelper internet;
    internet.SetRoutingHelper (list);
    internet.Install (networkNodes);

  /** Energy Model **/
  /***************************************************************************/
  /* energy source */
  BasicEnergySourceHelper basicSourceHelper;
  // configure energy source
  basicSourceHelper.Set ("BasicEnergySourceInitialEnergyJ", DoubleValue (15.0));
  // install source
  EnergySourceContainer sources = basicSourceHelper.Install (c);
  /* device energy model */
  WifiRadioEnergyModelHelper radioEnergyHelper;
  // configure radio energy model
  radioEnergyHelper.Set ("TxCurrentA", DoubleValue (0.0174));
  // install device model
  DeviceEnergyModelContainer deviceModels = radioEnergyHelper.Install (devices, sources);
  /***************************************************************************/

  /** Internet stack **/
//  InternetStackHelper internet;
//  internet.Install (networkNodes);

  Ipv4AddressHelper ipv4;
  NS_LOG_INFO ("Assign IP Addresses.");
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = ipv4.Assign (devices);
  UdpEchoServerHelper echoServer(90);

  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  Ptr<Socket> recvSink = Socket::CreateSocket (networkNodes.Get (14), tid);  // node 14, receiver
  InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), 80);
  recvSink->Bind (local);
  recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));

  Ptr<Socket> source = Socket::CreateSocket (networkNodes.Get (0), tid);    // node 0, sender
  InetSocketAddress remote = InetSocketAddress (i.GetAddress(14,0), 80);
  source->SetAllowBroadcast (true);
  source->Connect (remote);

  //-------------------------------------------------------------------------
  ApplicationContainer serverApps =  echoServer.Install(c.Get(14));
  serverApps.Start(Seconds(1.0));
  serverApps.Stop(Seconds(1500.0));

  UdpEchoClientHelper echoClient(i.GetAddress(14), 90);
  echoClient.SetAttribute("MaxPackets", UintegerValue(1000));
  echoClient.SetAttribute("Interval", TimeValue(Seconds(2.0)));

  ApplicationContainer client_app_0 = echoClient.Install(c.Get(0));
  client_app_0.Start(Seconds(2.0));
  client_app_0.Stop(Seconds(1500.0));

  ApplicationContainer client_app_1 = echoClient.Install(c.Get(1));
  client_app_1.Start(Seconds(2.0));
  client_app_1.Stop(Seconds(1500.0));

  ApplicationContainer client_app_2 = echoClient.Install(c.Get(2));
  client_app_2.Start(Seconds(2.0));
  client_app_2.Stop(Seconds(1500.0));

  ApplicationContainer client_app_3 = echoClient.Install(c.Get(3));
  client_app_3.Start(Seconds(2.0));
  client_app_3.Stop(Seconds(1500.0));

  ApplicationContainer client_app_4 = echoClient.Install(c.Get(4));
  client_app_4.Start(Seconds(2.0));
  client_app_4.Stop(Seconds(1500.0));

  ApplicationContainer client_app_5 = echoClient.Install(c.Get(5));
  client_app_5.Start(Seconds(2.0));
  client_app_5.Stop(Seconds(1500.0));

  ApplicationContainer client_app_6 = echoClient.Install(c.Get(6));
  client_app_6.Start(Seconds(2.0));
  client_app_6.Stop(Seconds(1500.0));

  ApplicationContainer client_app_7 = echoClient.Install(c.Get(7));
  client_app_7.Start(Seconds(2.0));
  client_app_7.Stop(Seconds(1500.0));

  ApplicationContainer client_app_8 = echoClient.Install(c.Get(8));
  client_app_8.Start(Seconds(2.0));
  client_app_8.Stop(Seconds(1500.0));

  ApplicationContainer client_app_9 = echoClient.Install(c.Get(9));
  client_app_9.Start(Seconds(2.0));
  client_app_9.Stop(Seconds(1500.0));

  ApplicationContainer client_app_10 = echoClient.Install(c.Get(10));
  client_app_10.Start(Seconds(2.0));
  client_app_10.Stop(Seconds(1500.0));

  ApplicationContainer client_app_11 = echoClient.Install(c.Get(11));
  client_app_11.Start(Seconds(2.0));
  client_app_11.Stop(Seconds(1500.0));

  ApplicationContainer client_app_12 = echoClient.Install(c.Get(12));
  client_app_12.Start(Seconds(2.0));
  client_app_12.Stop(Seconds(1500.0));

  ApplicationContainer client_app_13 = echoClient.Install(c.Get(13));
  client_app_13.Start(Seconds(2.0));
  client_app_13.Stop(Seconds(1500.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  //-implement encryption-
  echoClient.SetFill(client_app_0.Get(0), plain);
  echoClient.SetFill(client_app_1.Get(0), plain);
  echoClient.SetFill(client_app_2.Get(0), plain);
  echoClient.SetFill(client_app_3.Get(0), plain);
  echoClient.SetFill(client_app_4.Get(0), plain);
  echoClient.SetFill(client_app_5.Get(0), plain);
  echoClient.SetFill(client_app_6.Get(0), plain);
  echoClient.SetFill(client_app_7.Get(0), plain);
  echoClient.SetFill(client_app_8.Get(0), plain);
  echoClient.SetFill(client_app_9.Get(0), plain);
  echoClient.SetFill(client_app_10.Get(0), plain);
  echoClient.SetFill(client_app_11.Get(0), plain);
  echoClient.SetFill(client_app_12.Get(0), plain);
  echoClient.SetFill(client_app_13.Get(0), plain);
  /*-----------------------------------------------*/



  /** connect trace sources **/
  /***************************************************************************/
  // all sources are connected to node (1)
  // energy source
  Ptr<BasicEnergySource> basicSourcePtr = DynamicCast<BasicEnergySource> (sources.Get (1));
  basicSourcePtr->TraceConnectWithoutContext ("RemainingEnergy", MakeCallback (&RemainingEnergy));

  // device energy model
  Ptr<DeviceEnergyModel> basicRadioModelPtr =
  basicSourcePtr->FindDeviceEnergyModels ("ns3::WifiRadioEnergyModel").Get (0);
  NS_ASSERT (basicRadioModelPtr != NULL);
  basicRadioModelPtr->TraceConnectWithoutContext ("TotalEnergyConsumption", MakeCallback (&TotalEnergy));

  /***************************************************************************/

  //Output
  AsciiTraceHelper ascii;
  wifiPhy.EnableAsciiAll(ascii.CreateFileStream("wireless_no_encrypt.tr"));
  wifiPhy.EnablePcap("Pcap_no_encrypt", devices);

  Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper>("rute", std::ios::out);
  olsr.PrintRoutingTableAllEvery(Seconds(2.0),routingStream);

  /*Animation setup*/
  AnimationInterface anm("mo_encrypt.xml");

  anm.SetConstantPosition(c.Get(0),60.0, 345.0);
  anm.SetConstantPosition(c.Get(1),138.0, 276.0);
  anm.SetConstantPosition(c.Get(2),177.0, 381.0);
  anm.SetConstantPosition(c.Get(3),247.0, 278.0);
  anm.SetConstantPosition(c.Get(4),257.0, 197.0);
  anm.SetConstantPosition(c.Get(5),338.0, 222.0);
  anm.SetConstantPosition(c.Get(6),361.0, 177.0);
  anm.SetConstantPosition(c.Get(7),441.0, 209.0);
  anm.SetConstantPosition(c.Get(8),268.0, 421.0);
  anm.SetConstantPosition(c.Get(9),330.0, 467.0);
  anm.SetConstantPosition(c.Get(10),407.0, 381.0);
  anm.SetConstantPosition(c.Get(11),460.0, 364.0);
  anm.SetConstantPosition(c.Get(12),500.0, 376.0);
  anm.SetConstantPosition(c.Get(13),487.0, 419.0);
  anm.SetConstantPosition(c.Get(14),401.0, 287.0);
  //-------------------------------------------------

  /** simulation setup **/
  // start traffic
  Simulator::Schedule (Seconds (startTime), &GenerateTraffic, source, PpacketSize,
                       networkNodes.Get (0), numPackets, interPacketInterval);

  Simulator::Stop (Seconds (1500.0));
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
