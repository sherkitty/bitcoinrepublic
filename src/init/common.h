// Copyright (c) 2021-2022 The Bitcoin Republic Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! @file
//! @brief Common init functions shared by bitcoinrepublic-node, bitcoinrepublic-wallet, etc.

#ifndef BITCOINREPUBLIC_INIT_COMMON_H
#define BITCOINREPUBLIC_INIT_COMMON_H

class ArgsManager;

namespace init {
void AddLoggingArgs(ArgsManager& args);
void SetLoggingOptions(const ArgsManager& args);
void SetLoggingCategories(const ArgsManager& args);
void SetLoggingLevel(const ArgsManager& args);
bool StartLogging(const ArgsManager& args);
void LogPackageVersion();
} // namespace init

#endif // BITCOINREPUBLIC_INIT_COMMON_H
