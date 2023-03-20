// Copyright (c) 2022 The Bitcoin Republic Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOINREPUBLIC_KERNEL_CHAIN_H
#define BITCOINREPUBLIC_KERNEL_CHAIN_H

class CBlock;
class CBlockIndex;
namespace interfaces {
struct BlockInfo;
} // namespace interfaces

namespace kernel {
//! Return data from block index.
interfaces::BlockInfo MakeBlockInfo(const CBlockIndex* block_index, const CBlock* data = nullptr);
} // namespace kernel

#endif // BITCOINREPUBLIC_KERNEL_CHAIN_H
