package com.example.jakub.pilotdosamochodu;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.media.MediaPlayer;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.Random;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {
    public UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb");
    public static TextView t1, c1;
    public static ImageButton i0, i1, i2, i3, i4, i5, i6, i7, i8, i9;
    public static ToggleButton tg1;
    public static Context context;
    public static boolean adapter = false;
    public static boolean running = false;
    public static BluetoothAdapter badapter;
    public BluetoothDevice device = null;
    public BluetoothSocket socket = null;
    public static DataOutputStream outData = null;
    public static DataInputStream inData = null;
    public static boolean _isChecked = false;
    public MediaPlayer mediaPlayer1;
    public MediaPlayer mediaPlayer2;
    public MediaPlayer mediaPlayer3;
    public MediaPlayer mediaPlayer4;
    public MediaPlayer mediaPlayer5;
    public MediaPlayer mediaPlayer6;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        context = getApplicationContext();
        t1 = (TextView)findViewById(R.id.textView);
        i0 = (ImageButton)findViewById(R.id.buttonBlue);
        i1 = (ImageButton)findViewById(R.id.i1);
        i2 = (ImageButton)findViewById(R.id.i2);
        i3 = (ImageButton)findViewById(R.id.i3);
        i4 = (ImageButton)findViewById(R.id.i4);
        i5 = (ImageButton)findViewById(R.id.i5);
        i6 = (ImageButton)findViewById(R.id.i6);
        i7 = (ImageButton)findViewById(R.id.i7);
        i8 = (ImageButton)findViewById(R.id.i8);
        i9 = (ImageButton)findViewById(R.id.i9);
        c1 = (TextView)findViewById(R.id.stmData);
        tg1 = (ToggleButton) findViewById(R.id.switcher);
        mediaPlayer1 = MediaPlayer.create(this, R.raw.a);
        mediaPlayer2 = MediaPlayer.create(this, R.raw.b);
        mediaPlayer3 = MediaPlayer.create(this, R.raw.c);
        mediaPlayer4 = MediaPlayer.create(this, R.raw.d);
        mediaPlayer5 = MediaPlayer.create(this, R.raw.e);
        mediaPlayer6 = MediaPlayer.create(this, R.raw.f);
        setControlls(0);

       tg1.setOnClickListener(new View.OnClickListener() {
           @Override
           public void onClick(View v) {
               _isChecked = !_isChecked;
               if (_isChecked) sendChar("b");
               else sendChar("f");
               Log.i("Switcher: ", "zmieniony");
           }
       });


        i0.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(running == false) {
                    getAdapter();
                    Bluetooth();
                }
            }
        });
        i1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendChar("q");
                playSoundHor();
                playSoundVer();
                new Return().start();
            }
        });
        i2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendChar("w");
                playSoundVer();
            }
        });
        i3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendChar("e");
                playSoundHor();
                playSoundVer();
                new Return().start();
            }
        });
        i4.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendChar("a");
                playSoundVer();
                new Return().start();
            }
        });
        i5.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendChar("s");
            }
        });
        i6.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendChar("d");
                playSoundVer();
                new Return().start();
            }
        });
        i7.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendChar("z");
                playSoundHor();
                playSoundVer();
                new Return().start();
            }
        });
        i8.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendChar("x");
            }
        });
        i9.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendChar("c");
                playSoundHor();
                playSoundVer();
                new Return().start();
            }
        });
    }

    public void playSoundVer(){
        Random r = new Random();
        switch (r.nextInt(4-1)+1)
        {
            case 1: mediaPlayer1.start();
                break;
            case 2: mediaPlayer2.start();
                break;
            case 3: mediaPlayer3.start();
                break;
        }
    }

    public void playSoundHor(){
        Random r = new Random();
        switch (r.nextInt(7-4)+4)
        {
            case 4: mediaPlayer4.start();
                break;
            case 5: mediaPlayer5.start();
                break;
            case 6: mediaPlayer6.start();
                break;
        }
    }

    public void changeConsole(String x){
        Log.i("Ustawianie ", "niby ok");
        /*runOnUiThread(new Runnable(){
            @Override
            public void run(){
                c1.setText();
            }
        });*/

    }
    @Override
    public void onStop() {
        super.onStop();
        try{
        badapter.disable();}
        catch(Exception ex){}
        Log.i("Adaper BT: ", "wyłączony!");
    }

    public void getAdapter(){
        badapter = BluetoothAdapter.getDefaultAdapter();
        if(badapter == null) {
            MainActivity.t1.setText("Adapter nie obsługiwany.");
            adapter = false;
            Log.i("Adaper BT: ", "nieobsługiwany!");
        }
        adapter = true;
    }

    public boolean enableBluetooth(){
        Log.i("URUCHAMIAM BT", "");
        if(!badapter.isEnabled()){
            try{
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, 1);

            }catch (Exception ex){
                return false;
            }
            //t1.setText("Adapter włączony!");
            Log.i("Adaper BT: ", "włączony!");
            return true;
        }
        return false;
    }

    public BluetoothDevice findSTM() {
        Log.i("SZUKAM BT", "a");
        Set<BluetoothDevice> pairedDevices = badapter.getBondedDevices();
        if (pairedDevices.size() > 0) {
            for (BluetoothDevice device : pairedDevices) {
                if (device.getAddress().equals("30:14:08:18:09:10")) {
                    Log.i("Urzadzenie BT: ", "odnalezione!");
                    return badapter.getRemoteDevice(device.getAddress());
                }
            }
        }
        //t1.setText("Nie znaleziono BT!");
        Log.i("Urządzenia: ", "nie odnaleziono!");
        return null;
    }

    public boolean connectSTM(){
        boolean status = false;
        if(socket != null && socket.isConnected()) status = true;
        else{
            try{
                socket = device.createInsecureRfcommSocketToServiceRecord(MY_UUID);
                outData = new DataOutputStream(socket.getOutputStream());
                inData = new DataInputStream(socket.getInputStream());
                socket.connect();

                if(socket.isConnected()){
                   status = true;
                    Log.i("STM", " połączono!");
                }}catch(IOException e){
            }
        }
        return status;
    }

    public void Bluetooth(){

        if(adapter==true){
            if(enableBluetooth())
            while(device==null)
            device = findSTM();
            if(device != null && connectSTM())
            {
                Log.i("STM", " WORK!!!!");
                i0.setBackgroundResource(R.drawable.bluon);
                t1.setEnabled(false);
                t1.setVisibility(View.GONE);
                running = true;
                setControlls(1);
                c1.setText("GÓWNO");
            }
        }
    }

    public void sendChar(String data){
        try{
            outData.writeBytes(data);
        } catch(Exception ex) {}
    }

    public void setControlls(int i){
        if (i == 1){
            i1.setEnabled(true);
            i1.setVisibility(View.VISIBLE);
            i2.setEnabled(true);
            i2.setVisibility(View.VISIBLE);
            i3.setEnabled(true);
            i3.setVisibility(View.VISIBLE);
            i4.setEnabled(true);
            i4.setVisibility(View.VISIBLE);
            i5.setEnabled(true);
            i5.setVisibility(View.VISIBLE);
            i6.setEnabled(true);
            i6.setVisibility(View.VISIBLE);
            i7.setEnabled(true);
            i7.setVisibility(View.VISIBLE);
            i8.setEnabled(true);
            i8.setVisibility(View.VISIBLE);
            i9.setEnabled(true);
            i9.setVisibility(View.VISIBLE);
        }
        else
        {
            i1.setEnabled(false);
            i1.setVisibility(View.INVISIBLE);
            i2.setEnabled(false);
            i2.setVisibility(View.INVISIBLE);
            i3.setEnabled(false);
            i3.setVisibility(View.INVISIBLE);
            i4.setEnabled(false);
            i4.setVisibility(View.INVISIBLE);
            i5.setEnabled(false);
            i5.setVisibility(View.INVISIBLE);
            i6.setEnabled(false);
            i6.setVisibility(View.INVISIBLE);
            i7.setEnabled(false);
            i7.setVisibility(View.INVISIBLE);
            i8.setEnabled(false);
            i8.setVisibility(View.INVISIBLE);
            i9.setEnabled(false);
            i9.setVisibility(View.INVISIBLE);
        }
    }


}
