// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2021 The Bitcoin Republic Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kernel/chainparams.h>

#include <chainparamsseeds.h>
#include <consensus/amount.h>
#include <consensus/merkle.h>
#include <consensus/params.h>
#include <hash.h>
#include <chainparamsbase.h>
#include <logging.h>
#include <primitives/block.h>
#include <primitives/transaction.h>
#include <script/interpreter.h>
#include <script/script.h>
#include <uint256.h>
#include <util/strencodings.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <type_traits>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 4294967295 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000001b421b, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=3826ba, nTime=1681108888, nBits=1d00ffff, nNonce=2284324887, vtx=1)
 *   CTransaction(hash=3826ba, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c4e5468652054696d65732030392f417072696c2f32303233204c61626f75722061747461636b2061647320746f20626c616d652053756e616b20666f72206372617368696e672065636f6e6f6d792e)
 *     CTxOut(nValue=150.00000000, scriptPubKey=0x4c6574277320476574204269626c69)
 *   vMerkleTree: 3826ba
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "The Times 09/April/2023 Labour attack ads to blame Sunak for crashing economy.";
    const CScript genesisOutputScript = CScript() << ParseHex("4c6574277320476574204269626c6963616c20427920546f7669612053696e6765722050726f7665732043687269737469616e6974792049732041204c6965ba5e") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network on which people trade goods and services.
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = CBaseChainParams::MAIN;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        consensus.nSubsidyHalvingInterval = 630000;
        consensus.script_flag_exceptions.emplace( // BIP16 exception
            uint256S("0x000000001b421b4c6cb20e5de6ed3fbc881b9032f01e3ae48c5a6aa93ff17d15"), SCRIPT_VERIFY_NONE);
        consensus.script_flag_exceptions.emplace( // Taproot exception
            uint256S("0x000000001b421b4c6cb20e5de6ed3fbc881b9032f01e3ae48c5a6aa93ff17d15"), SCRIPT_VERIFY_P2SH | SCRIPT_VERIFY_WITNESS);
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x000000001b421b4c6cb20e5de6ed3fbc881b9032f01e3ae48c5a6aa93ff17d15");
        consensus.BIP65Height = 0; 
        consensus.BIP66Height = 0;
        consensus.CSVHeight = 0;
        consensus.SegwitHeight = 0;
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1815;
        consensus.nMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0; // No activation delay

        // Deployment of Taproot (BIPs 340-342)
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = 1600000000; 
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = 1900000000;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].min_activation_height = 0;

        consensus.nMinimumChainWork = uint256S("0x00000000ffff0000000000000000000000000000000000000000000000000000");
        consensus.defaultAssumeValid = uint256S("0x000000001b421b4c6cb20e5de6ed3fbc881b9032f01e3ae48c5a6aa93ff17d15");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xbe;
        pchMessageStart[1] = 0xef;
        pchMessageStart[2] = 0xba;
        pchMessageStart[3] = 0x5e;
        nDefaultPort = 33731;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 496;
        m_assumed_chain_state_size = 6;

        genesis = CreateGenesisBlock(1681108888, 2284324887, 0x1d00ffff, 1, 150 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000000001b421b4c6cb20e5de6ed3fbc881b9032f01e3ae48c5a6aa93ff17d15"));
        assert(genesis.hashMerkleRoot == uint256S("0x3826ba31cf091792c76040928fa931062d16d881d31550f68698584964210dc0"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as an addrfetch if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.emplace_back("62.171.154.187."); // SeedNode@gobbledygook.me
        vSeeds.emplace_back("62.171.154.187."); // SeedNode@gobbledygook.me
        vSeeds.emplace_back("62.171.154.143."); // SeedNode@gobbledygook.me
        vSeeds.emplace_back("62.171.154.143."); // SeedNode@gobbledygook.me
        vSeeds.emplace_back("62.171.151.63."); // SeedNode@gobbledygook.me
        vSeeds.emplace_back("62.171.151.63."); // SeedNode@gobbledygook.me
        vSeeds.emplace_back("62.171.151.61."); //SeedNode@gobbledygook.me
        vSeeds.emplace_back("62.171.151.61."); // SeedNode@gobbledygook.me
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,60); // "sK" prefix for public key addresses
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5); // Replace with your desired prefix for script addresses
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,190); // "sKT" prefix for secret keys
        base58Prefixes[EXT_PUBLIC_KEY] = {0x63, 0x45, 0x87, 0xCF}; // Replace with your desired prefix for extended public keys
        base58Prefixes[EXT_SECRET_KEY] = {0x63, 0x45, 0x83, 0x94}; // Replace with your desired prefix for extended secret keys
        bech32_hrp = "bc";

        vFixedSeeds = std::vector<uint8_t>(std::begin(chainparams_seed_main), std::end(chainparams_seed_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = false;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                { 0, uint256S("0x000000001b421b4c6cb20e5de6ed3fbc881b9032f01e3ae48c5a6aa93ff17d15")},
                
            }
        };

        m_assumeutxo_data = MapAssumeutxo{
         // TODO to be specified in a future patch.
        };

        chainTxData = ChainTxData{
            // Data from RPC: getchaintxstats 1 000000001b421b4c6cb20e5de6ed3fbc881b9032f01e3ae48c5a6aa93ff17d15
            .nTime    = 1681108888,
            .nTxCount = 1,
            .dTxRate  = 0.0,
        };
    }
};

/**
 * Testnet (v3): public test network which is reset from time to time.
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = CBaseChainParams::TESTNET;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        consensus.nSubsidyHalvingInterval = 630000;
        consensus.script_flag_exceptions.emplace( // BIP16 exception
            uint256S("0x00000000135510e627bfe59a45cf2ecf25002a31fe7f8905add4498c6ddd92e5"), SCRIPT_VERIFY_NONE);
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x00000000135510e627bfe59a45cf2ecf25002a31fe7f8905add4498c6ddd92e5");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.CSVHeight = 0;
        consensus.SegwitHeight = 0;
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 0;
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0; // No activation delay

        // Deployment of Taproot (BIPs 340-342)
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = 1600000000; 
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = 1900000000;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].min_activation_height = 0;

        consensus.nMinimumChainWork = uint256S("0x00000000ffff0000000000000000000000000000000000000000000000000000");
        consensus.defaultAssumeValid = uint256S("0x00000000135510e627bfe59a45cf2ecf25002a31fe7f8905add4498c6ddd92e5"); // 0

        pchMessageStart[0] = 0x0b;
        pchMessageStart[1] = 0xad;
        pchMessageStart[2] = 0xbe;
        pchMessageStart[3] = 0xef;
        nDefaultPort = 33700;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 42;
        m_assumed_chain_state_size = 2;

        genesis = CreateGenesisBlock(1681108778, 981970217, 0x1d00ffff, 1, 150 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000000135510e627bfe59a45cf2ecf25002a31fe7f8905add4498c6ddd92e5"));
        assert(genesis.hashMerkleRoot == uint256S("0x3826ba31cf091792c76040928fa931062d16d881d31550f68698584964210dc0"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("109.123.229.142");
        vSeeds.emplace_back("109.123.229.140");
        vSeeds.emplace_back("109.123.229.139");
        vSeeds.emplace_back("109.123.229.138"); // Just a static list of stable node(s), only supports x9

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x63, 0x45, 0x88, 0xFF};
        base58Prefixes[EXT_SECRET_KEY] = {0x63, 0x45, 0x99, 0xDD};

        bech32_hrp = "tb";

        vFixedSeeds = std::vector<uint8_t>(std::begin(chainparams_seed_test), std::end(chainparams_seed_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        m_is_test_chain = true;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                {0, uint256S("0x00000000135510e627bfe59a45cf2ecf25002a31fe7f8905add4498c6ddd92e5")},
            }
        };

        m_assumeutxo_data = MapAssumeutxo{
            // TODO to be specified in a future patch.
        };

        chainTxData = ChainTxData{
            // Data from RPC: getchaintxstats 4096 0000000000000004877fa2d36316398528de4f347df2f8a96f76613a298ce060
            .nTime    = 1681108778,
            .nTxCount = 1,
            .dTxRate  = 0.0,
        };
    }
};

/**
 * Signet: test network with an additional consensus parameter (see BIP325).
 */
class SigNetParams : public CChainParams {
public:
    explicit SigNetParams(const SigNetOptions& options)
    {
        std::vector<uint8_t> bin;
        vSeeds.clear();

        if (!options.challenge) {
            bin = ParseHex("546f204461766964284e4f54204a43293a796f7572206b696e67646f6d207368616c6c2062652065737461626c697368656420666f7265766572206265666f726520796f752e20");
            vSeeds.emplace_back("seed.signet.bitcoin.sprovoost.nl.");

            // Hardcoded nodes can be removed once there are more DNS seeds
            vSeeds.emplace_back("62.171.154.143");
            vSeeds.emplace_back("v7ajjeirttkbnt32wpy3c6w3emwnfr3fkla7hpxcfokr3ysd3kqtzmqd.onion:38333");

            consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000000001291fc22898");
            consensus.defaultAssumeValid = uint256S("0x000000d1a0e224fa4679d2fb2187ba55431c284fa1b74cbc8cfda866fd4d2c09"); // 105495
            m_assumed_blockchain_size = 1;
            m_assumed_chain_state_size = 0;
            chainTxData = ChainTxData{
                // Data from RPC: getchaintxstats 4096 000000d1a0e224fa4679d2fb2187ba55431c284fa1b74cbc8cfda866fd4d2c09
                .nTime    = 1661702566,
                .nTxCount = 1903567,
                .dTxRate  = 0.02336701143027275,
            };
        } else {
            bin = *options.challenge;
            consensus.nMinimumChainWork = uint256{};
            consensus.defaultAssumeValid = uint256{};
            m_assumed_blockchain_size = 0;
            m_assumed_chain_state_size = 0;
            chainTxData = ChainTxData{
                0,
                0,
                0,
            };
            LogPrintf("Signet with challenge %s\n", HexStr(bin));
        }

        if (options.seeds) {
            vSeeds = *options.seeds;
        }

        strNetworkID = CBaseChainParams::SIGNET;
        consensus.signet_blocks = true;
        consensus.signet_challenge.assign(bin.begin(), bin.end());
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256{};
        consensus.BIP65Height = 1;
        consensus.BIP66Height = 1;
        consensus.CSVHeight = 1;
        consensus.SegwitHeight = 1;
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1815; // 90% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256S("00000377ae000000000000000000000000000000000000000000000000000000");
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0; // No activation delay

        // Activation of Taproot (BIPs 340-342)
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].min_activation_height = 0; // No activation delay

        // message start is defined as the first 4 bytes of the sha256d of the block script
        HashWriter h{};
        h << consensus.signet_challenge;
        uint256 hash = h.GetHash();
        memcpy(pchMessageStart, hash.begin(), 4);

        nDefaultPort = 38333;
        nPruneAfterHeight = 1000;
                                   
        genesis = CreateGenesisBlock(241556880, 52613770, 0x1e0377ae, 1, 150 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xddc768b19bd63c2824e7d46fe6f2ea638cca39e1e928cc83ab6ce162dea137d8"));
        assert(genesis.hashMerkleRoot == uint256S("0x2a109db2d5c10c5d8bc04b7620923a5046da69a86258fc9cdbd083734cb43316"));

        vFixedSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x63, 0x45, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x63, 0x45, 0x83, 0x94};

        bech32_hrp = "tb";

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = true;
        m_is_mockable_chain = false;
    }
};

/**
 * Regression test: intended for private networks only. Has minimal difficulty to ensure that
 * blocks can be found instantly.
 */
class CRegTestParams : public CChainParams
{
public:
    explicit CRegTestParams(const RegTestOptions& opts)
    {
        strNetworkID =  CBaseChainParams::REGTEST;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP34Height = 1; // Always active unless overridden
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1;  // Always active unless overridden
        consensus.BIP66Height = 1;  // Always active unless overridden
        consensus.CSVHeight = 1;    // Always active unless overridden
        consensus.SegwitHeight = 0; // Always active unless overridden
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0; // No activation delay

        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].min_activation_height = 0; // No activation delay

        consensus.nMinimumChainWork = uint256{};
        consensus.defaultAssumeValid = uint256{};

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 18444;
        nPruneAfterHeight = opts.fastprune ? 100 : 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        for (const auto& [dep, height] : opts.activation_heights) {
            switch (dep) {
            case Consensus::BuriedDeployment::DEPLOYMENT_SEGWIT:
                consensus.SegwitHeight = int{height};
                break;
            case Consensus::BuriedDeployment::DEPLOYMENT_HEIGHTINCB:
                consensus.BIP34Height = int{height};
                break;
            case Consensus::BuriedDeployment::DEPLOYMENT_DERSIG:
                consensus.BIP66Height = int{height};
                break;
            case Consensus::BuriedDeployment::DEPLOYMENT_CLTV:
                consensus.BIP65Height = int{height};
                break;
            case Consensus::BuriedDeployment::DEPLOYMENT_CSV:
                consensus.CSVHeight = int{height};
                break;
            }
        }

        for (const auto& [deployment_pos, version_bits_params] : opts.version_bits_parameters) {
            consensus.vDeployments[deployment_pos].nStartTime = version_bits_params.start_time;
            consensus.vDeployments[deployment_pos].nTimeout = version_bits_params.timeout;
            consensus.vDeployments[deployment_pos].min_activation_height = version_bits_params.min_activation_height;
        }

        genesis = CreateGenesisBlock(1296688602, 2, 0x207fffff, 1, 150 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xfff467308aae830c9fdcdaa2da7eef0ea1c5ea31103c670da0e753c202862369"));
        assert(genesis.hashMerkleRoot == uint256S("0x2a109db2d5c10c5d8bc04b7620923a5046da69a86258fc9cdbd083734cb43316"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();
        vSeeds.emplace_back("dummySeed.invalid.");

        fDefaultConsistencyChecks = true;
        fRequireStandard = true;
        m_is_test_chain = true;
        m_is_mockable_chain = true;

        checkpointData = {
            {
                {0, uint256S("0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206")},
            }
        };

        m_assumeutxo_data = MapAssumeutxo{
            {
                110,
                {AssumeutxoHash{uint256S("0x1ebbf5850204c0bdb15bf030f47c7fe91d45c44c712697e4509ba67adb01c618")}, 110},
            },
            {
                200,
                {AssumeutxoHash{uint256S("0x51c8d11d8b5c1de51543c579736e786aa2736206d1e11e627568029ce092cf62")}, 200},
            },
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x63, 0x45, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x63, 0x45, 0x83, 0x94};

        bech32_hrp = "bcrt";
    }
};

std::unique_ptr<const CChainParams> CChainParams::SigNet(const SigNetOptions& options)
{
    return std::make_unique<const SigNetParams>(options);
}

std::unique_ptr<const CChainParams> CChainParams::RegTest(const RegTestOptions& options)
{
    return std::make_unique<const CRegTestParams>(options);
}

std::unique_ptr<const CChainParams> CChainParams::Main()
{
    return std::make_unique<const CMainParams>();
}

std::unique_ptr<const CChainParams> CChainParams::TestNet()
{
    return std::make_unique<const CTestNetParams>();
}
