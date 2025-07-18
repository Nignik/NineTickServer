#pragma once

#include <string>
#include <unordered_set>

#include "Asio.h"
#include "messages.pb.h"

class Game
{
public:
    Game(const std::string& id);

    void MessageAllPlayers(NetworkMessage msg);
    [[nodiscard]] uint32_t GetNumberOfPlayers() const;

private:
    std::string mId;
    std::set<std::shared_ptr<websocket::stream<tcp::socket>>> mPlayers;

private:
    friend class GameManager;

    void addPlayer(std::shared_ptr<websocket::stream<tcp::socket>> ws);
    void disconnectPlayer(std::shared_ptr<websocket::stream<tcp::socket>> ws);
};
