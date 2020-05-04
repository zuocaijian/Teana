package com.zcj.teana;

import android.app.Application;
import android.content.Context;

public class App extends Application {

    public static Context sContext;
    public static Application sApplication;

    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
    }

    @Override
    public void onCreate() {
        super.onCreate();
        sApplication = this;
        sContext = getApplicationContext();
    }
}
