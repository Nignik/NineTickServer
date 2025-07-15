#pragma once

#include <iostream>
#include <optional>
#include <string>

#include "Asio.h"

namespace utils
{
    inline std::optional<json::object> parse_json(const std::string& message)
    {
        boost::system::error_code ec;
        json::value jv = json::parse(message, ec);
        if (ec)
        {
            std::println(std::cerr, "Error parsing JSON: {}", ec.message());
            return std::nullopt;
        }

        if (!jv.is_object())
        {
            std::println(std::cerr, "Parsed JSON is not an object.");
            return std::nullopt;
        }

        return jv.as_object();
    }
}

