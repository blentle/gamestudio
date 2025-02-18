// Copyright (C) 2020-2021 Sami Väisänen
// Copyright (C) 2020-2021 Ensisoft http://www.ensisoft.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include "config.h"

#include "warnpush.h"
#  include <glm/vec2.hpp>
#  include <glm/vec3.hpp>
#  include <glm/vec4.hpp>
#include "warnpop.h"

#include <variant>
#include <unordered_map>

#include "base/types.h"
#include "wdk/keys.h"
#include "wdk/bitflag.h"

#include "game/fwd.h"

namespace engine
{
    struct MouseEvent {
        // mouse cursor position in window coordinates.
        glm::vec2 window_coord;
        // mouse cursor position in scene coordinates.
        glm::vec2 scene_coord;
        // true when the mouse cursor is within/over the viewport
        // in the window that renders the visible part of the
        // game/scene.
        bool over_scene = false;
        wdk::MouseButton btn = wdk::MouseButton::None;
        wdk::bitflag<wdk::Keymod> mods;
    };

    using GameEventValue = std::variant<
            bool, int, float,
            std::string,
            glm::vec2, glm::vec3, glm::vec4,
            base::Color4f,
            base::FPoint, base::FSize, base::FRect,
            game::Scene*, game::Entity*>;

    using GameEventObject = std::variant<
            std::string, game::Scene*, game::Entity*>;

    // an event generated by the game. the semantics and meaning
    // of every field is completely up to the game to define.
    // the below comments are just a hint.
    struct GameEvent {
        // identification of the poster
        GameEventObject from;
        // identification of the receiver.
        GameEventObject to;
        // name/action/meaning of the event.
        std::string message;
        // the value (if any)
        std::unordered_map<std::string, GameEventValue> values;
    };

} // namespace

