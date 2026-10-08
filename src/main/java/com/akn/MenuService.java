package com.akn;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.widget.Toast;

/** Minimal compatibility shell; the original service implementation was absent from the ZIP. */
public final class MenuService extends Service {
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Toast.makeText(this,
                "MenuService original não disponível neste pacote.",
                Toast.LENGTH_LONG).show();
        stopSelf(startId);
        return START_NOT_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }
}
