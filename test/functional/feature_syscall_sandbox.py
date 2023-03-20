#!/usr/bin/env python3
# Copyright (c) 2021-2022 The Bitcoin Republic Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
"""Test bitcoinrepublicd aborts if a disallowed syscall is used when compiled with the syscall sandbox."""

from test_framework.test_framework import BitcoinRepublicTestFramework, SkipTest


class SyscallSandboxTest(BitcoinRepublicTestFramework):
    def set_test_params(self):
        self.num_nodes = 1

    def skip_test_if_missing_module(self):
        if not self.is_syscall_sandbox_compiled():
            raise SkipTest("bitcoinrepublicd has not been built with syscall sandbox enabled.")
        if self.disable_syscall_sandbox:
            raise SkipTest("--nosandbox passed to test runner.")

    def run_test(self):
        disallowed_syscall_terminated_bitcoinrepublicd = False
        expected_log_entry = 'ERROR: The syscall "getgroups" (syscall number 115) is not allowed by the syscall sandbox'
        with self.nodes[0].assert_debug_log([expected_log_entry]):
            self.log.info("Invoking disallowed syscall")
            try:
                self.nodes[0].invokedisallowedsyscall()
            except ConnectionError:
                disallowed_syscall_terminated_bitcoinrepublicd = True
        assert disallowed_syscall_terminated_bitcoinrepublicd
        self.nodes = []


if __name__ == "__main__":
    SyscallSandboxTest().main()
