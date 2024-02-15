#pragma once

#include <MatchEngine/core/logger/logger.hpp>
#include <MatchEngine/core/serialize/serialize_stream.hpp>
#include <MatchEngine/core/serialize/deserialize_stream.hpp>
#include <MatchEngine/core/serialize/specific_serialize_trait.hpp>
#include <MatchEngine/core/serialize/serialize_macro.hpp>
#include <MatchEngine/core/reflect/object.hpp>
#include <MatchEngine/core/reflect/reflect_macro.hpp>
#include <MatchEngine/function/user_interface/user_interface.hpp>

#include <MatchEngine/game_framework/scene.hpp>
#include <MatchEngine/game_framework/game_object.hpp>

#include <MatchEngine/game_framework/component/component.hpp>
#include <MatchEngine/game_framework/component/transform/transform_component.hpp>
#include <MatchEngine/game_framework/component/mesh/mesh_component.hpp>
#include <MatchEngine/game_framework/component/camera/camera_component.hpp>
#include <MatchEngine/game_framework/component/camera/orthographic_camera.hpp>
#include <MatchEngine/game_framework/component/camera/perspective_camera_component.hpp>
#include <MatchEngine/game_framework/component/camera/camera_controller_component.hpp>

#include <MatchEngine/engine/engine.hpp>
