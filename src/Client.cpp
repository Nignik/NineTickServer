#include "Client.h"

#include <messages.pb.h>

#include "utils.h"

Client::Client() = default;

Client::Client(std::shared_ptr<websocket::stream<tcp::socket>> ws)
    : mWs{std::move(ws)}
{}

void Client::HandleSession()
{
    for (;;)
    {
        beast::flat_buffer buffer;
        mWs->read(buffer);
        std::string message = beast::buffers_to_string(buffer.data());
        NetworkMessage msg;
        msg.ParseFromString(message);

        switch (msg.type())
        {
            case PLAYER_JOIN_REQUEST:
                std::println("Player join request received from: {}", msg.player_join_request().player_id());
                break;
            default:
                std::println("Unknown message type received");
        }

        std::cout.flush();
    }
}

bool Client::IsInGame() const { return true; }
std::string Client::GetGameID() const { return "defaultid"; }
