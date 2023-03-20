// Copyright (c) 2022 The Bitcoin Republic Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOINREPUBLIC_NODE_CHAINSTATEMANAGER_ARGS_H
#define BITCOINREPUBLIC_NODE_CHAINSTATEMANAGER_ARGS_H

#include <validation.h>

#include <optional>

class ArgsManager;
struct bilingual_str;

namespace node {
std::optional<bilingual_str> ApplyArgsManOptions(const ArgsManager& args, ChainstateManager::Options& opts);
} // namespace node

#endif // BITCOINREPUBLIC_NODE_CHAINSTATEMANAGER_ARGS_H
