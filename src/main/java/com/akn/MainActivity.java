package com.akn;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

/** Crash-fix shell: the original Java activity was not present in the supplied archive. */
public final class MainActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        View openMenu = findViewById(R.id.OpenMenu);
        if (openMenu != null) {
            openMenu.setOnClickListener(v -> {
                Toast.makeText(this,
                        "O ZIP não inclui a lógica Java original do menu.",
                        Toast.LENGTH_LONG).show();
                startService(new Intent(this, MenuService.class));
            });
        }
    }
}
