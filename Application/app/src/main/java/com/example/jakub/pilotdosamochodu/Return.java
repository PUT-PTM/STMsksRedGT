package com.example.jakub.pilotdosamochodu;

/**
 * Created by jakub on 09.06.2016.
 */
public class Return extends Thread {
    MainActivity ma = new MainActivity();
    public void run(){
        try{
            Thread.sleep(1000);
            ma.sendChar("o");

        } catch(Exception ex){}
    }

}
