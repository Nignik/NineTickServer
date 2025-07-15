#include "Game.h"

Game::Game(const std::string& id)
    : mId(id)
{}

uint32_t Game::GetNumberOfPlayers() { return 1; }

void Game::disconnectPlayer(const std::shared_ptr<websocket::stream<tcp::socket>>& ws) {}
