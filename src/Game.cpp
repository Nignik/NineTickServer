#include "Game.h"

Game::Game(const std::string& id)
    : mId(id)
{}

void Game::MessageAllPlayers(NetworkMessage msg)
{
    std::string serialized;
    msg.SerializeToString(&serialized);
    for (auto& ws : mPlayers)
        ws->write(asio::buffer(serialized));
}


uint32_t Game::GetNumberOfPlayers() const { return mPlayers.size(); }

void Game::addPlayer(std::shared_ptr<websocket::stream<tcp::socket>> ws)
{
    mPlayers.insert(ws);
}

void Game::disconnectPlayer(std::shared_ptr<websocket::stream<tcp::socket>> ws) {}
