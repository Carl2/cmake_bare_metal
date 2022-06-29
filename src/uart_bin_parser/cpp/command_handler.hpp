#pragma once
#include "command_parser.hpp"

namespace msg
{

namespace
{
// auto callback = [](msg::OptArgs args, auto start_iter, auto end_iter) {
//     static_cast<void>(args);
//     static_cast<void>(start_iter);
//     static_cast<void>(end_iter);
// };

}  // namespace

// constexpr auto cmds =
//     make_Guppi_protocol(make_cmd_item<GuppiCmd::CMD_ENABLE_ADDRESS_SETUP>(callback),
//                         make_cmd_item<GuppiCmd::CMD_DISABLE_ADDRESS_SETUP>(callback),
//                         make_cmd_item<GuppiCmd::CMD_SET_PRINTHEAD_ADDRESS>(callback));

}  // namespace msg
