package com.example.control;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.MotionEvent;
import android.widget.Toast;

public class MainActivity extends Activity {
	
	int width;
    int height;
    
    DrawingView dv;
    
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
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		dv = new DrawingView(this);
		
		dv.setBackgroundColor(Color.WHITE);
        
        DisplayMetrics dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);
        width=dm.widthPixels;
        height=dm.heightPixels;
        System.out.println(width + " , "+height);
        dv.setDim(width, height);
        
    	setContentView(dv);
    	
//		try{
//		  findBT();
//		  openBT();
//		  sendData("90,0,0");
//		}
//		catch (IOException ex) { }
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// get masked (not specific to a pointer) action
        
        switch (event.getAction()) {
        	case MotionEvent.ACTION_DOWN:
        		float tX = event.getX();
        		float tY = event.getY();
        		System.out.println("touch at "+tX+","+tY);
        		dv.touchX = (int)tX;
        		dv.touchY = (int)tY;
        		if(dv.settingsButton.touchButton(tX, tY) && !dv.settingsScreen){
        			dv.settingsScreen = true;
        		}
        		if(dv.settingsScreen){
        			if(dv.backButton.touchButton(tX, tY))
        				dv.settingsScreen = false;
        			if(!dv.autoSwivel && dv.swivelAutoButton.touchButton(tX, tY))
        				dv.autoSwivel = true;
        			else if(dv.autoSwivel && dv.swivelManualButton.touchButton(tX,tY))
        				dv.autoSwivel = false;
        			if(dv.connectButton.touchButton(tX, tY)){
        				try{
    					  findBT();
                          if(mmOutputStream != null) {
                              sendData("90,0,0");
                          }
    					}
    					catch (IOException ex) { }
        			}
        			if(dv.unstickButton.touchButton(tX, tY)){
        				try{
    					  sendData("90,0,0");
    					}
    					catch (IOException ex) { }
        			}
        			if(dv.clearButton.touchButton(tX, tY)){
        				for(int i = 0; i < dv.distances.length; i++){
        					dv.distances[i] = -1;
        				}
        			}
        		}
        		break;
        	case MotionEvent.ACTION_UP:
        		System.out.println("no touch");
        		dv.touchX = -10;
        		dv.touchY = -10;
        		break;
        	case MotionEvent.ACTION_MOVE:
        		//touch
        		float sX = event.getX();
        		float sY = event.getY();
        		dv.touchX = (int)sX;
        		dv.touchY = (int)sY;
        		break;
        }
        
        return super.onTouchEvent(event);
	}
	
	
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
                System.out.println(device.getName());
                mmDevice = device;
                try {
                    openBT();
                    System.out.println("success here");
                    break;
                } catch (IOException e) {
                    Toast.makeText(getApplicationContext(), "Failed to open device",Toast.LENGTH_LONG).show();
                }
            }
        }
        System.out.println("Bluetooth Device Found");
    }
    
    void openBT() throws IOException{
        UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb"); //Standard SerialPortService ID
        mmSocket = mmDevice.createRfcommSocketToServiceRecord(uuid);
        mmSocket.connect();
        mmOutputStream = mmSocket.getOutputStream();
        mmInputStream = mmSocket.getInputStream();
        
        beginListenForData();
        
        System.out.println("Bluetooth Opened");
        Toast.makeText(getApplicationContext(), "Bluetooth Opened", Toast.LENGTH_SHORT).show();
    }
    
    void beginListenForData(){
        stopWorker = false;
        workerThread = new Thread(new Runnable(){
            public void run(){                
               while(!Thread.currentThread().isInterrupted() && !stopWorker){
                    try {
                        int bytesAvailable = mmInputStream.available(); 
                        if(bytesAvailable > 0){
                            byte[] packetBytes = new byte[bytesAvailable];
                            mmInputStream.read(packetBytes);
                            for(int i=0;i<bytesAvailable;i++){
                                byte b = packetBytes[i];
                                if(b != 10 && b != 13){
                                    byte[] encodedBytes = new byte[1];
                                    encodedBytes[0] = b;
                                    String data = new String(encodedBytes, "US-ASCII");
                                    if(!data.equals("*"))
                                    	fullMessage+=data;
                                    else{
	                                    int finalVal = (Integer.parseInt(fullMessage,10));
	                                    String displayMes = String.format("%d", finalVal);
	                                    System.out.println("*"+displayMes+"*");
	                                    dv.distance = finalVal;
	                                    dv.updateAngle = true;
	                                    sendData(String.format("%d,%d,%d", dv.angle,dv.knobXDisp,dv.knobYDisp));
	                                    fullMessage = "";
                                    }
                                }
                            }
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
