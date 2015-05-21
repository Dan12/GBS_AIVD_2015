package com.example.cameratest;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;
import android.widget.RelativeLayout;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

public class CameraMain extends Activity
{
	RelativeLayout mRelativeLayout;
	CameraPreviewView surfaceView;

    BluetoothAdapter mBluetoothAdapter;
    BluetoothSocket mmSocket;
    BluetoothDevice mmDevice;
    OutputStream mmOutputStream;
    InputStream mmInputStream;
    Thread workerThread;
    volatile boolean stopWorker;
    static int centerTolerance = 100;

	@Override
    public void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        //Set this APK Full screen
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,  WindowManager.LayoutParams.FLAG_FULLSCREEN);
        //Set app to not sleep
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        //Set this APK no title
        requestWindowFeature(Window.FEATURE_NO_TITLE);  
        
        mRelativeLayout = new RelativeLayout(this);
        surfaceView = new CameraPreviewView(this, null);
        surfaceView.setBackgroundColor(Color.WHITE);
        RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT);
        surfaceView.setLayoutParams(layoutParams);
        mRelativeLayout.addView(surfaceView);
        setContentView(mRelativeLayout);
    }

    @Override 
    public boolean onTouchEvent(MotionEvent e) { 
    	if (e.getAction() == MotionEvent.ACTION_DOWN) {
    		int x = (int) e.getX();
    		int y = (int) e.getY();
    		surfaceView.touchDown(x,y);
            if(surfaceView.menuView && surfaceView.connectSlider.sVal == 1){
                if(mmOutputStream == null)
                    findBT();
                surfaceView.connectSlider.sVal = 0;
            }
		}
    	if(e.getAction() == MotionEvent.ACTION_MOVE){
    		int x = (int) e.getX();
    		int y = (int) e.getY();
    		surfaceView.touchMove(x,y);
            if(surfaceView.menuView && surfaceView.connectSlider.sVal == 1){
                if(mmOutputStream == null)
                    findBT();
                surfaceView.connectSlider.sVal = 0;
            }
    	}
    	return true;
    };

    void findBT(){
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if(mBluetoothAdapter == null)
        {
            System.out.println("No bluetooth adapter available");
        }

        if(!mBluetoothAdapter.isEnabled())
        {
            Intent enableBluetooth = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBluetooth, 0);
        }

        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
        if(pairedDevices.size() > 0)
        {
            boolean success = false;
            searchloop:
            for(BluetoothDevice device : pairedDevices)
            {
                System.out.println(device.getName());
                if(mmOutputStream != null) {
                    success = true;
                    break searchloop;
                }
                mmDevice = device;
                try {
                    openBT();
                    System.out.println("success here");
                    success = true;
                    break searchloop;
                } catch (IOException e) {
                    System.out.println("Failed");
                }
            }
            if(!success)
                Toast.makeText(getApplicationContext(), "Failed to open device",Toast.LENGTH_SHORT).show();
        }
        System.out.println("Bluetooth Device Found");
    }

    void openBT() throws IOException {
        UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb"); //Standard SerialPortService ID
        mmSocket = mmDevice.createRfcommSocketToServiceRecord(uuid);
        mmSocket.connect();
        mmOutputStream = mmSocket.getOutputStream();
        mmInputStream = mmSocket.getInputStream();

        System.out.println("Bluetooth Opened");
        Toast.makeText(getApplicationContext(), "Bluetooth Opened", Toast.LENGTH_SHORT).show();

        beginListenForData();
    }

    void beginListenForData(){
        stopWorker = false;
        workerThread = new Thread(new Runnable(){
            public void run(){
                while(!Thread.currentThread().isInterrupted() && !stopWorker){
                    try {
                        if(surfaceView.sendData == true) {
                            if(surfaceView.hC.cRadius > surfaceView.stopRadiusSize)
                                sendData("s");
                            else if (surfaceView.hC.cCXPos < (surfaceView.width / 2) - (centerTolerance+centerTolerance/2))
                                sendData("l");
                            else if (surfaceView.hC.cCXPos > (surfaceView.width / 2) + (centerTolerance+centerTolerance/2))
                                sendData("r");
                            else if (surfaceView.hC.cCXPos < (surfaceView.width / 2) - centerTolerance/2)
                                sendData("d");
                            else if (surfaceView.hC.cCXPos > (surfaceView.width / 2) + centerTolerance/2)
                                sendData("g");
                            else
                                sendData("f");
                            surfaceView.sendData = false;
                        }

                    }
                    catch (IOException ex) {stopWorker = true;}
                }
            }
        });
        workerThread.start();
    }

    void sendData(String m) throws IOException{
        String msg = m;
        mmOutputStream.write(msg.getBytes());
        System.out.println("Data Sent");
    }

    void closeBT() throws IOException{
        stopWorker = true;
        if(mmOutputStream != null)
            mmOutputStream.close();
        if(mmInputStream != null)
            mmInputStream.close();
        if(mmSocket != null)
            mmSocket.close();
        System.out.println("BlueTooth Closed");
    }

    protected void onDestroy(){
        System.out.println("Destroyed");
        super.onDestroy();
        System.out.println(isFinishing());
        try {
            closeBT();
        } catch (IOException e) {e.printStackTrace();}
        finish();
    }
}