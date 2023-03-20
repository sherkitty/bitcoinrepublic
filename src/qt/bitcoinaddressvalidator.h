// Copyright (c) 2011-2020 The Bitcoin Republic Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOINREPUBLIC_QT_BITCOINREPUBLICADDRESSVALIDATOR_H
#define BITCOINREPUBLIC_QT_BITCOINREPUBLICADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class BitcoinRepublicAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BitcoinRepublicAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** BitcoinRepublic address widget validator, checks for a valid bitcoinrepublic address.
 */
class BitcoinRepublicAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BitcoinRepublicAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // BITCOINREPUBLIC_QT_BITCOINREPUBLICADDRESSVALIDATOR_H
