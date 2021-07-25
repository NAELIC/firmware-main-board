#include "com.h"

nRF24L01P Device1(COM_MOSI, COM_MISO, COM_CLK, COM_CS1);
nRF24L01P Device2(COM_MOSI, COM_MISO, COM_CLK, COM_CS2);
nRF24L01P Device3(COM_MOSI, COM_MISO, COM_CLK, COM_CS3);

nRF24L01P_PTX PTX(Device1, COM_CE1);
nRF24L01P_PRX PRX(Device2, COM_CE2);
nRF24L01P_PRX PRX2(Device3, COM_CE3);
static naelic::SWO swo;

int send(nRF24L01P_PTX PTX)
{ 
  struct packet_master packet;
  
  packet.x_speed = 4;
  swo.println("Transmit");
  int r = PTX.TransmitPacket((char *) &packet, sizeof(struct packet_master));
  swo.println(r);

  swo.println(Device1.num_lost_packets());
  swo.println(Device2.num_lost_packets());
  return r;
}

SHELL_COMMAND(send, "send"){
  shell_println(send(PTX));
}

int receive(nRF24L01P_PRX PRX)
{
  int size_packet = -1;
  if (PRX.IsPacketReady())
  {
    
    struct packet_robot receive;
    size_packet = PRX.ReadPacket((char *) &receive);

    swo.print("Read: ");
    swo.print(size_packet);
    swo.print(" ");
    swo.println(receive.id);
    swo.println(receive.xpos);
  }
  else
  {
    swo.println("Got nothing");
  }

  swo.println((int)Device2.debug_read());

  wait_us(1000);
  return size_packet;
}

SHELL_COMMAND(receive, "receive"){
  shell_println(receive(PRX));
}

void test_radio()
{
  send(PTX);
  // struct packet_master packet;
  
  // packet.x_speed = 4;
  // swo.println("Transmit");
  // int r = PTX.TransmitPacket((char *) &packet, sizeof(struct packet_master));
  // swo.println(r);

  // swo.println(Device1.num_lost_packets());
  // swo.println(Device2.num_lost_packets());

  // if (PRX.IsPacketReady())
  // {
  //   struct packet_robot receive;
  //   int r = PRX.ReadPacket((char *) &receive);

  //   swo.print("Read: ");
  //   swo.print(r);
  //   swo.print(" ");
  //   swo.println(receive.id);
  //   swo.println(receive.xpos);
  // }
  // else
  // {
  //   swo.println("Got nothing");
  // }

  // swo.println((int)Device2.debug_read());

  // wait_us(1000);
}

SHELL_COMMAND(test, "")
{
  // char c = 'a';
  // shell_println("Transmit");
  // int r = PTX.TransmitPacket(&c, 1);
  // shell_println(r);

  // shell_println(Device1.num_lost_packets());
  // shell_println(Device2.num_lost_packets());

  // if (PRX.IsPacketReady())
  // {
  //   char d;
  //   int r = PRX.ReadPacket(&d);
  //   shell_print("Read: ");
  //   shell_print(r);
  //   shell_print(" ");
  //   shell_println(d);
  // }
  // else
  // {
  //   shell_println("Got nothing");
  // }

  // shell_println((int)Device2.debug_read());

  // wait_us(1000);
}

void com_init()
{

  PTX.initialize();
  PTX.set_channel(0);
  PTX.set_data_rate(2000);
  PTX.power_up();

  PRX.initialize();
  PRX.set_channel(0);
  PRX.set_data_rate(2000);
  PRX.set_payload_size(sizeof(struct packet_robot));
  PRX.power_up();
  PRX.start_receive();

  PRX2.initialize();
  PRX2.set_channel(0);
  PRX2.set_data_rate(2000);
  PRX2.set_payload_size(sizeof(struct packet_robot));
  PRX2.power_up();
  PRX2.start_receive();
}