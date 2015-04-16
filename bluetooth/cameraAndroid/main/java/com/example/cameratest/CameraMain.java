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
    byte[] readBuffer;
    int readBufferPosition;
    int counter;
    volatile boolean stopWorker;
    int restartCount = 0;
    String fullMessage = "";

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

        try {
            findBT();
            if (mmDevice != null) {
                openBT();
                sendData("90,0,0");
            }
        }
        catch(IOException e){}
    }

    @Override 
    public boolean onTouchEvent(MotionEvent e) { 
    	if (e.getAction() == MotionEvent.ACTION_DOWN) {
    		int x = (int) e.getX();
    		int y = (int) e.getY();
    		surfaceView.touchDown(x,y);
		}
    	if(e.getAction() == MotionEvent.ACTION_MOVE){
    		int x = (int) e.getX();
    		int y = (int) e.getY();
    		surfaceView.touchMove(x,y);
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
            for(BluetoothDevice device : pairedDevices)
            {
                if(device.getName().equals("HC-06"))
                {
                    mmDevice = device;
                    break;
                }
            }
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
                            if (surfaceView.hC.cCXPos < (surfaceView.width / 2) - 80)
                                sendData("r");
                            else if (surfaceView.hC.cCXPos > (surfaceView.width / 2) + 80)
                                sendData("l");
                            else
                                sendData("f");
                            surfaceView.sendData = false;
                        }

//                        int bytesAvailable = mmInputStream.available();
//                        if(bytesAvailable > 0){
//                            byte[] packetBytes = new byte[bytesAvailable];
//                            mmInputStream.read(packetBytes);
//                            for(int i=0;i<bytesAvailable;i++){
//                                byte b = packetBytes[i];
//                                if(b != 10 && b != 13){
//                                    byte[] encodedBytes = new byte[1];
//                                    encodedBytes[0] = b;
//                                    String data = new String(encodedBytes, "US-ASCII");
//                                    if(!data.equals("*"))
//                                        fullMessage+=data;
//                                    else{
//                                        int finalVal = (Integer.parseInt(fullMessage,10));
//                                        String displayMes = String.format("%d", finalVal);
//                                        System.out.println("*"+displayMes+"*");
//                                        dv.distance = finalVal;
//                                        dv.updateAngle = true;
//                                        sendData(String.format("%d,%d,%d", dv.angle,dv.knobXDisp,dv.knobYDisp));
//                                        fullMessage = "";
//                                    }
//                                }
//                            }
//                        }

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