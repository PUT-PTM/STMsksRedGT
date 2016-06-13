package com.example.jakub.pilotdosamochodu;

import android.provider.Telephony;
import android.util.Log;

import java.nio.charset.StandardCharsets;

/**
 * Created by jakub on 06.06.2016.
 */
public class Receiving extends Thread {
    public static byte[] inByte;
    public static String inData;
    MainActivity ma = new MainActivity();
    public void run() {
        Log.i("Odbieranie: ", inData);
        while (true) {
            try {
                Thread.sleep(1000);
                MainActivity.inData.read(inByte);
                inData = new String(inByte, StandardCharsets.UTF_8);


                ma.runOnUiThread(new Runnable(){
                    @Override
                    public void run(){
                        ma.c1.setText(inData);
                    }
                });
               }
        catch(Exception ex) {Log.i("Odbieranie: ", "dupa");}
        }
    }
}
