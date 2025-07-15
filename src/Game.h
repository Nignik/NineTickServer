#pragma once

#include <string>
#include "Asio.h"

class Game
{
public:
    Game(const std::string& id);

    [[nodiscard]] uint32_t GetNumberOfPlayers();

private:
    std::string mId;

private:
    friend class GameManager;

    void disconnectPlayer(const std::shared_ptr<websocket::stream<tcp::socket>>& ws);
};
