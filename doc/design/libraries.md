# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libbitcoinrepublic_cli*         | RPC client functionality used by *bitcoinrepublic-cli* executable |
| *libbitcoinrepublic_common*      | Home for common functionality shared by different executables and libraries. Similar to *libbitcoinrepublic_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libbitcoinrepublic_consensus*   | Stable, backwards-compatible consensus functionality used by *libbitcoinrepublic_node* and *libbitcoinrepublic_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *libbitcoinrepublicconsensus*    | Shared library build of static *libbitcoinrepublic_consensus* library |
| *libbitcoinrepublic_kernel*      | Consensus engine and support library used for validation by *libbitcoinrepublic_node* and also exposed as a [shared library](../shared-libraries.md). |
| *libbitcoinrepublicqt*           | GUI functionality used by *bitcoinrepublic-qt* and *bitcoinrepublic-gui* executables |
| *libbitcoinrepublic_ipc*         | IPC functionality used by *bitcoinrepublic-node*, *bitcoinrepublic-wallet*, *bitcoinrepublic-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *libbitcoinrepublic_node*        | P2P and RPC server functionality used by *bitcoinrepublicd* and *bitcoinrepublic-qt* executables. |
| *libbitcoinrepublic_util*        | Home for common functionality shared by different executables and libraries. Similar to *libbitcoinrepublic_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libbitcoinrepublic_wallet*      | Wallet functionality used by *bitcoinrepublicd* and *bitcoinrepublic-wallet* executables. |
| *libbitcoinrepublic_wallet_tool* | Lower-level wallet functionality used by *bitcoinrepublic-wallet* executable. |
| *libbitcoinrepublic_zmq*         | [ZeroMQ](../zmq.md) functionality used by *bitcoinrepublicd* and *bitcoinrepublic-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *libbitcoinrepublic_consensus* and *libbitcoinrepublic_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`libbitcoinrepublic_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libbitcoinrepublic_node* code lives in `src/node/` in the `node::` namespace
  - *libbitcoinrepublic_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libbitcoinrepublic_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libbitcoinrepublic_util* code lives in `src/util/` in the `util::` namespace
  - *libbitcoinrepublic_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "basis" }}}%%

graph TD;

bitcoinrepublic-cli[bitcoinrepublic-cli]-->libbitcoinrepublic_cli;

bitcoinrepublicd[bitcoinrepublicd]-->libbitcoinrepublic_node;
bitcoinrepublicd[bitcoinrepublicd]-->libbitcoinrepublic_wallet;

bitcoinrepublic-qt[bitcoinrepublic-qt]-->libbitcoinrepublic_node;
bitcoinrepublic-qt[bitcoinrepublic-qt]-->libbitcoinrepublicqt;
bitcoinrepublic-qt[bitcoinrepublic-qt]-->libbitcoinrepublic_wallet;

bitcoinrepublic-wallet[bitcoinrepublic-wallet]-->libbitcoinrepublic_wallet;
bitcoinrepublic-wallet[bitcoinrepublic-wallet]-->libbitcoinrepublic_wallet_tool;

libbitcoinrepublic_cli-->libbitcoinrepublic_util;
libbitcoinrepublic_cli-->libbitcoinrepublic_common;

libbitcoinrepublic_common-->libbitcoinrepublic_consensus;
libbitcoinrepublic_common-->libbitcoinrepublic_util;

libbitcoinrepublic_kernel-->libbitcoinrepublic_consensus;
libbitcoinrepublic_kernel-->libbitcoinrepublic_util;

libbitcoinrepublic_node-->libbitcoinrepublic_consensus;
libbitcoinrepublic_node-->libbitcoinrepublic_kernel;
libbitcoinrepublic_node-->libbitcoinrepublic_common;
libbitcoinrepublic_node-->libbitcoinrepublic_util;

libbitcoinrepublicqt-->libbitcoinrepublic_common;
libbitcoinrepublicqt-->libbitcoinrepublic_util;

libbitcoinrepublic_wallet-->libbitcoinrepublic_common;
libbitcoinrepublic_wallet-->libbitcoinrepublic_util;

libbitcoinrepublic_wallet_tool-->libbitcoinrepublic_wallet;
libbitcoinrepublic_wallet_tool-->libbitcoinrepublic_util;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class bitcoinrepublic-qt,bitcoinrepublicd,bitcoinrepublic-cli,bitcoinrepublic-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libbitcoinrepublic_wallet* and *libbitcoinrepublic_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libbitcoinrepublic_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libbitcoinrepublic_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *libbitcoinrepublic_common* should serve a similar function as *libbitcoinrepublic_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *libbitcoinrepublic_util* and *libbitcoinrepublic_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for bitcoinrepublic-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *libbitcoinrepublic_kernel* is not supposed to depend on *libbitcoinrepublic_common*, only *libbitcoinrepublic_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *libbitcoinrepublic_kernel* should only depend on *libbitcoinrepublic_util* and *libbitcoinrepublic_consensus*.

- The only thing that should depend on *libbitcoinrepublic_kernel* internally should be *libbitcoinrepublic_node*. GUI and wallet libraries *libbitcoinrepublicqt* and *libbitcoinrepublic_wallet* in particular should not depend on *libbitcoinrepublic_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *libbitcoinrepublic_consensus*, *libbitcoinrepublic_common*, and *libbitcoinrepublic_util*, instead of *libbitcoinrepublic_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libbitcoinrepublicqt*, *libbitcoinrepublic_node*, *libbitcoinrepublic_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *libbitcoinrepublic_node* to *libbitcoinrepublic_kernel* as part of [The libbitcoinrepublickernel Project #24303](https://github.com/sherkitty/bitcoinrepublic/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/sherkitty/bitcoinrepublic/issues/15732)
