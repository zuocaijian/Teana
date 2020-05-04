package com.zcj.teana.util;

import android.util.Log;

import com.zcj.teana.App;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class FileUtils {
    private static final String TAG = FileUtils.class.getSimpleName();

    public static void copyAssetsFile(final String assetsFile, final String dstFilePath) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    InputStream open = App.sContext.getAssets().open(assetsFile);
                    File file = new File(dstFilePath);
                    if (file.exists() && file.isFile()) {
                        file.delete();
                    }
                    boolean success = file.createNewFile();
                    if (success) {
                        int len = 0;
                        byte[] buffer = new byte[1024];
                        FileOutputStream fos = new FileOutputStream(file);
                        while ((len = open.read(buffer, 0, 1024)) != -1) {
                            fos.write(buffer, 0, len);
                        }
                        fos.close();
                        open.close();
                    }
                    Log.i(TAG, assetsFile + " has been copied to " + dstFilePath + " successfully.");
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }).start();
    }
}
