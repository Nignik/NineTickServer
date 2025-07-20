#include "Client.h"

#include <messages.pb.h>

#include "GameManager.h"
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
                handlePlayerJoinRequest(msg.player_join_request());
                break;
            case PLAYER_MOVE:
                handlePlayerMove(msg);
                break;

            default:
                std::println("Unknown message type received");
        }

        std::cout.flush();
    }
}

bool Client::IsInGame() const { return mGame != nullptr; }
std::string Client::GetGameID() const { return mGame->GetId(); }

void Client::handlePlayerJoinRequest(const PlayerJoinRequest& msg)
{
    std::println("Player join request received from: {}", msg.player_id());

    // Join game
    GameManager& gameManager = GameManager::GetInstance();
    if (!gameManager.DoesGameExist(msg.game_id()))
        gameManager.CreateGame(msg.game_id());

    mGame = gameManager.JoinGame(mWs, msg.game_id());

    // Send join request approval
    NetworkMessage approvalMsg;
    approvalMsg.set_type(PLAYER_JOIN_APPROVED);

    PlayerJoinApproved* payload = approvalMsg.mutable_player_join_approved();
    payload->set_player_id(mId);
    payload->set_player_color(mGame->GetNumberOfPlayers() == 1 ? WHITE : BLACK); // If there is one player, then that's us

    std::string serialized;
    approvalMsg.SerializeToString(&serialized);
    mWs->write(asio::buffer(serialized));
}

void Client::handlePlayerMove(NetworkMessage msg)
{
    PlayerMove move = msg.player_move();
    std::println("Player {} made a move to (row: {}, column: {})", move.player_id(), move.tile_row(), move.tile_row());
    mGame->MessageAllPlayers(msg);
}