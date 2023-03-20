// Copyright (c) 2016-2021 The Bitcoin Republic Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOINREPUBLIC_WALLET_RPC_WALLET_H
#define BITCOINREPUBLIC_WALLET_RPC_WALLET_H

#include <span.h>

class CRPCCommand;

namespace wallet {
Span<const CRPCCommand> GetWalletRPCCommands();
} // namespace wallet

#endif // BITCOINREPUBLIC_WALLET_RPC_WALLET_H
