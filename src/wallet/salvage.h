// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2021 The Bitcoin Republic Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOINREPUBLIC_WALLET_SALVAGE_H
#define BITCOINREPUBLIC_WALLET_SALVAGE_H

#include <fs.h>
#include <streams.h>

class ArgsManager;
struct bilingual_str;

namespace wallet {
bool RecoverDatabaseFile(const ArgsManager& args, const fs::path& file_path, bilingual_str& error, std::vector<bilingual_str>& warnings);
} // namespace wallet

#endif // BITCOINREPUBLIC_WALLET_SALVAGE_H
