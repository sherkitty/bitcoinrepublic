BitcoinRepublic version 0.5.1 is now available for download at:
http://sourceforge.net/projects/bitcoinrepublic/files/BitcoinRepublic/bitcoinrepublic-0.5.1/

This is a bugfix-only release.

This release includes 13 translations, including 5 new translations:
Italian, Hungarian, Ukranian, Portuguese (Brazilian) and Simplified Chinese.
More translations are welcome; join the project at Transifex if you can help:
https://www.transifex.net/projects/p/bitcoinrepublic/

Please report bugs using the issue tracker at github:
https://github.com/sherkitty/bitcoinrepublic/issues

Project source code is hosted at github; we are no longer
distributing .tar.gz files here, you can get them
directly from github:
https://github.com/sherkitty/bitcoinrepublic/tarball/v0.5.1  # .tar.gz
https://github.com/sherkitty/bitcoinrepublic/zipball/v0.5.1  # .zip

For Ubuntu users, there is a new ppa maintained by Matt Corallo which
you can add to your system so that it will automatically keep
bitcoinrepublic up-to-date.  Just type
sudo apt-add-repository ppa:bitcoinrepublic/bitcoinrepublic
in your terminal, then install the bitcoinrepublic-qt package.


BUG FIXES

Re-enable SSL support for the JSON-RPC interface (it was unintentionally
disabled for the 0.5.0 release binaries).

The code that finds peers via "dns seeds" no longer stops bitcoinrepublic startup
if one of the dns seed machines is down.

Tooltips on the transaction list view were rendering incorrectly (as black boxes
or with a transparent background).

Prevent a denial-of-service attack involving flooding a bitcoinrepublic node with
orphan blocks.

The wallet passphrase dialog now warns you if the caps lock key was pressed.

Improved searching in addresses and labels in bitcoinrepublic-qt.
