#include <enet/enet.h>

#include "log.h"

int main(void) {
  ENetHost *client = enet_host_create(NULL, 1, 2, 0, 0);
  if (!client) {
    ERROR("Could not create an ENet client\n");
    return 1;
  }

  ENetAddress server_address = { .port = 6666 };
  enet_address_set_host (&server_address, "localhost");

  ENetPeer *peer = enet_host_connect(client,
                                     &server_address,
                                     2, 0);
  if (!peer) {
    ERROR("No peers available");
    return 1;
  }

  ENetEvent event;

  if (enet_host_service(client, &event, 2000) <= 0 ||
      event.type != ENET_EVENT_TYPE_CONNECT) {
    enet_peer_reset(peer);
    ERROR("Connection to the server failed\n");
    return 1;
  }

  INFO("Connection to the server succeeded\n");

  char buffer[11] = {};
  if (fgets(buffer, sizeof(buffer), stdin)) {
    ENetPacket *packet = enet_packet_create(buffer, 11,
                                            ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    enet_host_flush(client);

    INFO("Succesfully sent packet\n");
  }

  for (;;) {
    if (enet_host_service(client, &event, 0) <= 0)
      continue;

    switch (event.type) {
    case ENET_EVENT_TYPE_CONNECT: break;

    case ENET_EVENT_TYPE_RECEIVE: {
      INFO("%s\n", (char *) event.packet->data);
      enet_packet_destroy(event.packet);
      goto end;
    } break;

    case ENET_EVENT_TYPE_DISCONNECT: break;

    case ENET_EVENT_TYPE_NONE: break;
    }

    sleep(0.05);
  }

 end:
  enet_host_destroy(client);
  enet_deinitialize();
  return 0;
}
