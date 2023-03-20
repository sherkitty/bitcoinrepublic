package org.bitcoinrepubliccore.qt;

import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class BitcoinRepublicQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File bitcoinrepublicDir = new File(getFilesDir().getAbsolutePath() + "/.bitcoinrepublic");
        if (!bitcoinrepublicDir.exists()) {
            bitcoinrepublicDir.mkdir();
        }

        super.onCreate(savedInstanceState);
    }
}
