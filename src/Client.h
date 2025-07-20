#pragma once

#include "Asio.h"
#include <memory>
#include <vector>
#include <print>

#include "Game.h"
#include "messages.pb.h"

class Client
{
public:
	Client();
	Client(std::shared_ptr<websocket::stream<tcp::socket>> ws);

	void HandleSession();

	bool IsInGame() const;
	std::string GetGameID() const;

private:
	void handlePlayerJoinRequest(const PlayerJoinRequest& msg);
	void handlePlayerMove(NetworkMessage msg);

	std::shared_ptr<websocket::stream<tcp::socket>> mWs{};
	std::shared_ptr<Game> mGame{};
	std::string mId = "anonymous";
};
