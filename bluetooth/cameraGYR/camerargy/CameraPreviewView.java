package com.dantech.camerargy;

import java.io.IOException;
import java.util.ArrayList;

import android.annotation.TargetApi;
import android.os.Build;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.SurfaceHolder.Callback;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.ImageFormat;
import android.graphics.Paint;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.Size;

public class CameraPreviewView extends SurfaceView implements Callback, Camera.PreviewCallback{
	private SurfaceHolder mHolder;

    private Camera mCamera;
    private boolean isPreviewRunning = false;
    private int [] rgbints;
    public int[][] pixelColorInts;
    int tolerance = 15;
    int startResolution = 4;

    public int[][] targetColorsGYR = new int[3][3];

    protected final Paint paint = new Paint();
    
    public int width;
    public int height;
    
    NumberSlider toleranceSlider;
    NumberSlider resolutionSlider;
    NumberSlider zoomSlider;
    NumberSlider clearNoiseSlider;
    NumberSlider showSquareSlider;
    NumberSlider showCircleSlider;
    NumberSlider showMenuSlider;
    NumberSlider connectSlider;
    NumberSlider gyrTouchSelect;
    
    ArrayList<HighlightedPixels> hpArray;
    ArrayList<ArrayList<HighlightedPixels>> gyrHighlightArray;
    HighlightedSquare hC;
    int[] maxXY = new int[2];
    int[] minXY = new int[2];
    float clearNoiseToleranceDensity = (float) 1.3;
    int drawResolution = startResolution;
    boolean willClearNoise = false;
    boolean willShowSquares = false;
    boolean willShowCircle = true;

    boolean sendData = false;

    boolean menuView = false;

    int colorAppeared = -1;
    int minArea = 4000;

    int gyrTouched = 0;
    
    public CameraPreviewView(Context context, AttributeSet attrs) {
    	super(context, attrs);
        paint.setARGB(100, 200, 0, 0);
        paint.setStyle(Paint.Style.FILL);
        paint.setStrokeWidth(2);

        mHolder = getHolder();
        mHolder.addCallback(this);

        targetColorsGYR = new int[][]{{0,255,0},{255,255,0},{255,0,0}};
        gyrHighlightArray = new ArrayList<ArrayList<HighlightedPixels>>();
        for(int i = 0; i < 3; i++)
            gyrHighlightArray.add(new ArrayList<HighlightedPixels>());

        hpArray = new ArrayList<HighlightedPixels>();
        hC = new HighlightedSquare(0, 0, 0, 0);
        
        toleranceSlider = new NumberSlider(1, 40, 15, 100, 30, 150, 40, "Tolerance");
        resolutionSlider = new NumberSlider(1, 20, 4, 100, 130, 150, 40, "Resolution");
        zoomSlider = new NumberSlider(0, 30, 0, 100, 230, 150, 40, "Zoom");
        connectSlider = new NumberSlider(0, 1, 0, 100, 330, 150, 40, "Connect");
        clearNoiseSlider = new NumberSlider(0, 1, 0, 300, 30, 150, 40, "Clear Noise");
        showSquareSlider = new NumberSlider(0, 1, 0, 300, 130, 150, 40, "Show Squares");
        showCircleSlider = new NumberSlider(0, 1, 1, 300, 230, 150, 40, "Show Circle");
        showMenuSlider = new NumberSlider(0, 1, 0, 650, 30, 150, 40, "Show Menu");
        gyrTouchSelect = new NumberSlider(0,2,0,300,330,150,40,"GYR Select");
    }

    @Override
    protected void onDraw(Canvas canvas) {
        //Log.w(this.getClass().getName(), "On Draw Called");
        System.out.println("onDraw Called");
        //set highlighted pixels
        setSimilar();
        
        System.out.println("Draw Stuff");
        //System.out.println("Width: "+width+", cx: "+hC.cCXPos);
        
        //draw bitmap of pixels from camera
        canvas.drawBitmap(rgbints, 0, width, 0, 0, width, height, false, null);
        
        //draw highlighted pixels
        if(willShowSquares){
	        paint.setARGB(100, 0, 255, 0);
	        for(int i = 0; i < hpArray.size(); i++){
	        	canvas.drawRect(hpArray.get(i).hXPos, hpArray.get(i).hYPos, hpArray.get(i).hXPos+drawResolution, hpArray.get(i).hYPos+drawResolution,paint);
	        }
        }

        System.out.println("min "+minXY[0]+","+minXY[1]+", max "+maxXY[0]+","+maxXY[1]);
        
        if(willShowCircle)
        	hC.drawSqaure(canvas);

        sendData = true;

        if(menuView) {
            paint.setARGB(150,250,250,250);
            canvas.drawRect(0,0,width,height,paint);
            toleranceSlider.drawSlider(canvas);
            tolerance = toleranceSlider.sVal;
            resolutionSlider.drawSlider(canvas);
            startResolution = resolutionSlider.sVal;
            drawResolution = resolutionSlider.sVal;
            zoomSlider.drawSlider(canvas);
            clearNoiseSlider.drawSlider(canvas);
            gyrTouchSelect.drawSlider(canvas);
            gyrTouched = gyrTouchSelect.sVal;
            if (clearNoiseSlider.sVal > 0)
                willClearNoise = true;
            else
                willClearNoise = false;
            showSquareSlider.drawSlider(canvas);
            if (showSquareSlider.sVal > 0)
                willShowSquares = true;
            else
                willShowSquares = false;
            showCircleSlider.drawSlider(canvas);
            if (showCircleSlider.sVal > 0)
                willShowCircle = true;
            else
                willShowCircle = false;
            connectSlider.drawSlider(canvas);
        }

        showMenuSlider.drawSlider(canvas);

        paint.setColor(Color.BLACK);
        String charSent = "";
        if (colorAppeared == 0)
            charSent = "g";
        else if (colorAppeared == 1)
            charSent = "y";
        else if (colorAppeared == 2)
            charSent = "r";
        else
            charSent = "n";
        paint.setTextSize(24);
        canvas.drawText("Char Sent: "+charSent,650,130,paint);
        
        super.invalidate();
    }
    
    public void touchDown(int x, int y){
        showMenuSlider.touchSlider(x, y);
        if(showMenuSlider.sVal == 1)
            menuView = true;
        else
            menuView = false;
        if(!menuView) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                int blue = pixelColorInts[x][y] / 1000000;
                int green = (pixelColorInts[x][y] - (blue * 1000000)) / 1000;
                int red = (pixelColorInts[x][y] - ((green * 1000) + (blue * 1000000)));
                targetColorsGYR[gyrTouched][0] = red;
                targetColorsGYR[gyrTouched][1] = green;
                targetColorsGYR[gyrTouched][2] = blue;
                System.out.println(targetColorsGYR[gyrTouched]);
            }
        }
        else {
            toleranceSlider.touchSlider(x, y);
            resolutionSlider.touchSlider(x, y);
            zoomSlider.touchSlider(x, y);
            Camera.Parameters p = mCamera.getParameters();
            p.setZoom(zoomSlider.sVal);
            mCamera.setParameters(p);
            clearNoiseSlider.touchSlider(x, y);
            showSquareSlider.touchSlider(x, y);
            showCircleSlider.touchSlider(x, y);
            connectSlider.touchSlider(x, y);
            gyrTouchSelect.touchSlider(x, y);
        }
    }
    
    public void touchMove(int x, int y){
        if(menuView) {
            toleranceSlider.touchSlider(x, y);
            resolutionSlider.touchSlider(x, y);
            zoomSlider.touchSlider(x, y);
            Camera.Parameters p = mCamera.getParameters();
            p.setZoom(zoomSlider.sVal);
            mCamera.setParameters(p);
            clearNoiseSlider.touchSlider(x, y);
            showSquareSlider.touchSlider(x, y);
            showCircleSlider.touchSlider(x, y);
            connectSlider.touchSlider(x, y);
            gyrTouchSelect.touchSlider(x, y);
        }
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
    }

    @TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH)
    public void surfaceCreated(SurfaceHolder holder) {
        synchronized (this) {
        	if (isPreviewRunning)
                return;
            this.setWillNotDraw(false); // This allows us to make our own draw
                                    // calls to this canvas
            
            mCamera = Camera.open();

            Camera.Parameters params = mCamera.getParameters();
            String sParams = params.flatten();
            System.out.println("camera flatten: "+sParams);
            Parameters mCameraParameters = mCamera.getParameters();
            System.out.println("Supported Exposure Modes:" + mCameraParameters.get("exposure-mode-values"));
            System.out.println("Supported White Balance Modes:" + mCameraParameters.get("whitebalance-values"));
            String supportedIsoValues = params.get("iso-values");
            System.out.println("ISO Vals " + supportedIsoValues);

            isPreviewRunning = true;
            Camera.Parameters p = mCamera.getParameters();
            Size size = p.getPreviewSize();
            width = size.width;
            height = size.height;
            System.out.println(width+","+height);
            p.setPreviewFormat(ImageFormat.NV21);
            int maxZoom = p.getMaxZoom(); 
            int zoom = 0;
            p.set("iso", "100");
            p.setWhiteBalance(p.WHITE_BALANCE_CLOUDY_DAYLIGHT);
            p.setSceneMode(p.SCENE_MODE_ACTION);
            p.setExposureCompensation(0);
            if(p.isAutoExposureLockSupported())
            	p.setAutoExposureLock(true);
            if(p.isAutoWhiteBalanceLockSupported())
            	p.setAutoWhiteBalanceLock(true);
            if (p.isZoomSupported()) {
               if (zoom >=0 && zoom < maxZoom) {
                 p.setZoom(zoom);
                 System.out.println("Zoom Supported");
               } else {
                 System.out.println("Error Zoom");
               }
            }
            mCamera.setParameters(p);

            rgbints = new int[width * height];
            pixelColorInts = new int[width][height];
            
            System.out.println("New");
            
            //comment out if you don't want live preview
            try { mCamera.setPreviewDisplay(holder); } catch (IOException e)
            { Log.e("Camera", "mCamera.setPreviewDisplay(holder);"); }

            mCamera.startPreview();
            mCamera.setPreviewCallback(this);

        }
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
        synchronized (this) {
        	try {
                if (mCamera != null) {
                    //mHolder.removeCallback(this);
                    mCamera.setPreviewCallback(null);
                    mCamera.stopPreview();
                    isPreviewRunning  = false;
                    mCamera.release();
                }
            } catch (Exception e) {
                Log.e("Camera", e.getMessage());
            }
        }
    }

    public void onPreviewFrame(byte[] data, Camera camera) {
    	System.out.println("Got Preview Data");
        if (!isPreviewRunning)
            return;
        
        Canvas canvas = null;

        if(mHolder == null){
            return;
        }

        try {
            synchronized (mHolder) {
            	System.out.println("Start Decode");
                decodeYUV(data, width, height);
                System.out.println("Finished Decode");

            }
        }  catch (Exception e){
            e.printStackTrace();
        } finally {
            // do this in a finally so that if an exception is thrown
            // during the above, we don't leave the Surface in an
            // inconsistent state
            if (canvas != null) {
                mHolder.unlockCanvasAndPost(canvas);
            }
        }
    }
    
    public void setSimilar(){
    	hpArray.clear();
        for(int i = 0; i < 3; i++)
            gyrHighlightArray.get(i).clear();
        
        minXY[0] = 0;
        minXY[1] = 0;
        maxXY[0] = 0;
        maxXY[1] = 0;
        
    	//System.out.println((pixelColorInts[200][200]/1000000)+","+((pixelColorInts[200][200]-((pixelColorInts[200][200]/1000000)*1000000))/1000)+","+(pixelColorInts[200][200]-((((pixelColorInts[200][200]-((pixelColorInts[200][200]/1000000)*1000000))/1000)*1000)+((pixelColorInts[200][200]/1000000)*1000000))));
        for(int i = 0; i < pixelColorInts.length/startResolution; i++){
            for(int k = 0; k < pixelColorInts[0].length/startResolution; k++){
            	int blue = pixelColorInts[i*startResolution][k*startResolution]/1000000;
            	int green = (pixelColorInts[i*startResolution][k*startResolution]-(blue*1000000))/1000;
            	int red = (pixelColorInts[i*startResolution][k*startResolution]-((green*1000)+(blue*1000000)));
                for(int j = 0; j < 3; j++) {
                    if (Math.abs(targetColorsGYR[j][0] - red) <= tolerance && Math.abs(targetColorsGYR[j][1] - green) <= tolerance && Math.abs(targetColorsGYR[j][2] - blue) <= tolerance) {
                        gyrHighlightArray.get(j).add(new HighlightedPixels(i * startResolution, k * startResolution));
                    }
                }
            }
        }

        colorAppeared = 0;
        if(gyrHighlightArray.get(0).size() > gyrHighlightArray.get(1).size()){
            if(gyrHighlightArray.get(0).size() > gyrHighlightArray.get(2).size())
                colorAppeared = 0;
            else
                colorAppeared = 2;
        }
        else{
            if(gyrHighlightArray.get(1).size() > gyrHighlightArray.get(2).size())
                colorAppeared = 1;
            else
                colorAppeared = 2;
        }

        hpArray = gyrHighlightArray.get(colorAppeared);
        if(!hpArray.isEmpty()) {
            minXY[0] = hpArray.get(0).hXPos;
            minXY[1] = hpArray.get(0).hYPos;
            maxXY[0] = hpArray.get(hpArray.size() - 1).hXPos;
            maxXY[1] = hpArray.get(hpArray.size() - 1).hYPos;
        }
        for(HighlightedPixels hp : hpArray){
            if(hp.hYPos > maxXY[1])
                maxXY[1] = hp.hYPos;
            if(hp.hYPos < minXY[1])
                minXY[1] = hp.hYPos;
        }
        
        if(hpArray.size() > 1){
            hC.setVals(minXY[0], minXY[1], maxXY[0]-minXY[0], maxXY[1]-minXY[1]);
	        
	        if(willClearNoise)
	            clearNoise();
        }
        else{
        	hC.setVals(0,0,0,0);
        }
        if(hC.getArea() < minArea)
            colorAppeared = -1;
    }
    
    public void clearNoise(){
        int clearNoiseRes = startResolution+startResolution;
        ArrayList<HighlightedPixels> temp = new ArrayList<HighlightedPixels>();
        for(int j = 0; j < hpArray.size(); j++){
            temp.add(hpArray.get(j));
        }
        while(hpArray.size() > 1){
            if(hC.getArea()/clearNoiseToleranceDensity > (hpArray.size()*clearNoiseRes*clearNoiseRes)){
                minXY[0] = 0;
                minXY[1] = height;
                maxXY[0] = 0;
                maxXY[1] = 0;
                hpArray.clear();
                
                for(int i = 0; i < temp.size(); i++){
                    if(temp.get(i).hXPos%clearNoiseRes == 0 && temp.get(i).hYPos%clearNoiseRes == 0 && temp.get(i).hN){
                        hpArray.add(temp.get(i));
                    }
                }

                for(int i = 0; i < hpArray.size(); i++){
                    int tempX = hpArray.get(i).hXPos;
                    int tempY = hpArray.get(i).hYPos;
                    boolean hasNeighbor = false;
                    innerloop:
                    for(int k = 0; k < hpArray.size(); k++){
                        if((hpArray.get(k).hXPos == tempX+clearNoiseRes && hpArray.get(k).hYPos == tempY) || (hpArray.get(k).hXPos == tempX-clearNoiseRes && hpArray.get(k).hYPos == tempY) || (hpArray.get(k).hYPos == tempY+clearNoiseRes && hpArray.get(k).hXPos == tempX) || (hpArray.get(k).hYPos == tempY-clearNoiseRes && hpArray.get(k).hXPos == tempX)){
                            hasNeighbor = true;
                            break innerloop;
                        }
                    }
                    if(!hasNeighbor){
                        hpArray.get(i).hN = false;
                        hpArray.remove(i);
                        i--;
                    }
                    else{
                        if(hpArray.get(i).hYPos > maxXY[1])
                            maxXY[1] = hpArray.get(i).hYPos+clearNoiseRes;
                        if(hpArray.get(i).hYPos < minXY[1])
                            minXY[1] = hpArray.get(i).hYPos;
                    }
                }

                if(!hpArray.isEmpty()) {
                    minXY[0] = hpArray.get(0).hXPos;
                    maxXY[0] = hpArray.get(hpArray.size() - 1).hXPos+clearNoiseRes;
                }
                
                if(hpArray.size() > 1)
                	hC.setVals(minXY[0], minXY[1], maxXY[0]-minXY[0], maxXY[1]-minXY[1]);
                else
                	hC.setVals(0, 0, 0, 0);
                
                clearNoiseRes+=startResolution;
            }
            else
                break;
        }
        
        drawResolution = clearNoiseRes-startResolution;
    }
    

//     * Decodes YUV frame to a buffer which can be use to create a bitmap. use
//     * this for OS < FROYO which has a native YUV decoder decode Y, U, and V
//     * values on the YUV 420 buffer described as YCbCr_422_SP by Android
//     *
//     * @param rgb
//     *            the outgoing array of RGB bytes
//     * @param fg
//     *            the incoming frame bytes
//     * @param width
//     *            of source frame
//     * @param height
//     *            of source frame
//     * @throws NullPointerException
//     * @throws IllegalArgumentException

    public void decodeYUV(byte[] fg, int width, int height) {
        int sz = width * height;
        int row = 0;
        int col = 0;
        int i, j;
        int Y, Cr = 0, Cb = 0;
        for (j = 0; j < height; j++) {
            int pixPtr = j * width;
            final int jDiv2 = j >> 1;
	        for (i = 0; i < width; i++) {
	            Y = fg[pixPtr];
	            if (Y < 0)
	                Y += 255;
	            if ((i & 0x1) != 1) {
	                final int cOff = sz + jDiv2 * width + (i >> 1) * 2;
	                Cb = fg[cOff];
	                if (Cb < 0)
	                    Cb += 127;
	                else
	                    Cb -= 128;
	                Cr = fg[cOff + 1];
	                if (Cr < 0)
	                    Cr += 127;
	                else
	                    Cr -= 128;
	            }
	            int R = Y + Cr + (Cr >> 2) + (Cr >> 3) + (Cr >> 5);
	            if (R < 0)
	                R = 0;
	            else if (R > 255)
	                R = 255;
	            int G = Y - (Cb >> 2) + (Cb >> 4) + (Cb >> 5) - (Cr >> 1) + (Cr >> 3) + (Cr >> 4) + (Cr >> 5);
	            if (G < 0)
	                G = 0;
	            else if (G > 255)
	                G = 255;
	            int B = Y + Cb + (Cb >> 1) + (Cb >> 2) + (Cb >> 6);
	            if (B < 0)
	                B = 0;
	            else if (B > 255)
	                B = 255;
	            rgbints[pixPtr++] = 0xff000000 + (B << 16) + (G << 8) + R;
	            pixelColorInts[col][row] = B+G*1000+R*1000000;
	            col++;
	            if (col == width) {
	               col = 0;
	               row++;
	            }
	        }
        }
    }
    
    public int distSqrd(int x1, int y1, int x2, int y2){
        return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
    }
    
    public class HighlightedPixels{
        int hXPos;
        int hYPos;
        boolean hN;
        
        public HighlightedPixels(int x, int y){
            hXPos = x;
            hYPos = y;
            hN = true;
        }
    }
    
    public class HighlightedSquare {
        int cCXPos;
        int cCYPos;
        int cWidth;
        int cHeight;
        Paint cPaint = new Paint();
        
        public HighlightedSquare(int x, int y, int w, int h){
            cCXPos = x;
            cCYPos = y;
            cWidth = w;
            cHeight = h;
        }
        
        public void setVals(int x, int y, int w, int h){
            cCXPos = x;
            cCYPos = y;
            cWidth = w;
            cHeight = h;
        }
        
        public void drawSqaure(Canvas canvas){
            cPaint.setARGB(100,0,255,0);
            canvas.drawRect(cCXPos, cCYPos, cCXPos+cWidth, cCYPos+cHeight, cPaint);
        }

        public int getArea(){
            return cWidth*cHeight;
        }

        public int getCenterX(){
            return cCXPos+width/2;
        }

        public int getCenterY() {
            return cCYPos + height / 2;
        }
    }
}
