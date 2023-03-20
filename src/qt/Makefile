.PHONY: FORCE
all: FORCE
	$(MAKE) -C .. bitcoinrepublic_qt test_bitcoinrepublic_qt
clean: FORCE
	$(MAKE) -C .. bitcoinrepublic_qt_clean test_bitcoinrepublic_qt_clean
check: FORCE
	$(MAKE) -C .. test_bitcoinrepublic_qt_check
bitcoinrepublic-qt bitcoinrepublic-qt.exe: FORCE
	 $(MAKE) -C .. bitcoinrepublic_qt
apk: FORCE
	$(MAKE) -C .. bitcoinrepublic_qt_apk
