#include <enet/enet.h>

#include "log.h"

#define MAX_CLIENTS 32

int main(void) {
  if (enet_initialize()) {
    ERROR("Could not initialize ENet\n");
    return 1;
  }

  ENetAddress address = {
    .host = ENET_HOST_ANY,
    .port = 6666,
  };
  ENetHost *server = enet_host_create(&address,
                            MAX_CLIENTS,
                            2, 0, 0);
  if (!server) {
    ERROR("Could not create an ENet server\n");
    return 1;
  }

  ENetEvent event;

  for (;;) {
    if (enet_host_service(server, &event, 0) <= 0)
      continue;

    switch (event.type) {
    case ENET_EVENT_TYPE_CONNECT: {
      INFO("A new client connected from %x:%u\n",
           event.peer->address.host,
           event.peer->address.port);
      event.peer->data = "Client information";
    } break;

    case ENET_EVENT_TYPE_RECEIVE: {
      INFO("Packet received: %u:%s from `%s`\n",
           (unsigned int) event.packet->dataLength,
           (char *) event.packet->data,
           (char *) event.peer->data);
      enet_host_broadcast(server, 0, event.packet);
    } break;

    case ENET_EVENT_TYPE_DISCONNECT: {
      printf ("Client `%s` disconnected\n", (char *) event.peer->data);
      event.peer->data = NULL;
    } break;

    case ENET_EVENT_TYPE_NONE: {} break;
    }

    sleep(0.05);
  }

  enet_host_destroy(server);
  enet_deinitialize();
  return 0;
}
