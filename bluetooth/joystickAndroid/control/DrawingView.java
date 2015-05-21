package com.dantech.control;

import java.util.Random;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.Path;
import android.graphics.drawable.Drawable;
import android.view.View;

public class DrawingView extends View{

	int touchX;
	int touchY;
	
	int knobXDisp = -10;
	int knobYDisp = -10;
	
	int winWidth;
	int winHeight;
	
	int squareHalfSide;
	int centerCircleX;
	int centerCircleY;

	
	int sliderLineY;
	int sliderLineHeight;
	int sliderLineWidth;
	int sliderX;
	int sliderWidth;
	int ultraAngle = 90;
	int angleIncrease = 2;
	
	boolean autoSwivel = true;
	boolean settingsScreen = false;
	boolean updateAngle = false;
	
	Resources res = getResources();
	pressableButton settingsButton = new pressableButton(0,0,0,0,res.getDrawable(R.drawable.settings));
	pressableButton backButton = new pressableButton(0,0,0,0,res.getDrawable(R.drawable.back));
	pressableButton swivelAutoButton = new pressableButton(0,0,0,0,res.getDrawable(R.drawable.swivelauto));
	pressableButton swivelManualButton = new pressableButton(0,0,0,0,res.getDrawable(R.drawable.swivemanual));
	pressableButton connectButton = new pressableButton(0,0,0,0,res.getDrawable(R.drawable.connect));
	pressableButton unstickButton = new pressableButton(0,0,0,0,res.getDrawable(R.drawable.unstick));
	pressableButton clearButton = new pressableButton(0,0,0,0,res.getDrawable(R.drawable.clear));
	
	
	int distances[] = new int[180];
	int angle = 90;
	int angleChange = angleIncrease;
	Random rand = new Random();
	int distance = 200;
	int swivelViewHeight = 0;
	int swivelViewXCent = 0;
	int swivelViewYCent = 0;
	int swivelBoxDim = 10;
	
	Path sliderPath = new Path();
	Path knobPath = new Path();
	
	Paint paint = new Paint();
	
	public DrawingView(Context context) {
		super(context);
	}
	
	//class for buttons
	public class pressableButton{
		int x1;
		int x2;
		int y1;
		int y2;
		Drawable img;
		
		public pressableButton(int left, int top, int right, int bottom, Drawable i){
			x1 = left;
			x2 = right;
			y1 = top;
			y2 = bottom;
			img = i;
		}
		
		public boolean touchButton(float tX, float tY){
			if(tX >= x1 && tX <= x2 && tY >= y1 && tY <= y2)
				return true;
			return false;
		}
		
		public void setBounds(int left, int top, int right, int bottom){
			img.setBounds(left, top, right, bottom);
			x1 = left;
			x2 = right;
			y1 = top;
			y2 = bottom; 
		}
	}
	
	@Override
    public void onDraw(Canvas canvas) {
		long startTime = System.nanoTime();
		
		updateTouch();
		
		//big square
		paint.setColor(Color.BLACK);
		canvas.drawRect(centerCircleX - squareHalfSide, centerCircleY - squareHalfSide, centerCircleX + squareHalfSide, centerCircleY + squareHalfSide, paint);
		paint.setColor(Color.WHITE);
		canvas.drawRect(centerCircleX + 4 - squareHalfSide, centerCircleY + 4 - squareHalfSide, centerCircleX + squareHalfSide - 4, centerCircleY + squareHalfSide - 4, paint);
		//small circle
		paint.setColor(Color.BLACK);
		canvas.drawCircle(centerCircleX, centerCircleY, (float) (squareHalfSide *0.2), paint);
		//knob circle
		paint.setColor(Color.BLACK);
		canvas.drawCircle(centerCircleX +knobXDisp, centerCircleY +knobYDisp, (float) (squareHalfSide *0.15), paint);
		//knob connector line
		double knobAngle = Math.atan2(knobXDisp,-knobYDisp) * (180/Math.PI) + 180;
		knobAngle = Math.toRadians(knobAngle);
		knobPath.reset();
		knobPath.moveTo((float) (centerCircleX +(squareHalfSide *0.2*Math.cos(knobAngle))), (float) (centerCircleY +(squareHalfSide *0.2*Math.sin(knobAngle))));
		knobPath.lineTo((float) (centerCircleX +knobXDisp), centerCircleY +knobYDisp);
		//knobPath.lineTo((float) (centerCircleX+knobXDisp+squareHalfSide*0.15), (centerCircleY)+knobYDisp);
		knobPath.lineTo((float) (centerCircleX +(squareHalfSide *0.2*Math.cos(knobAngle+Math.PI))), (float) (centerCircleY +(squareHalfSide *0.2*Math.sin(knobAngle+Math.PI))));
		canvas.drawPath(knobPath, paint);
		//knob
		paint.setColor(Color.BLUE);
		canvas.drawCircle(centerCircleX +knobXDisp, centerCircleY +knobYDisp, (float) (squareHalfSide *0.2)-4, paint);
		
		//slider
		paint.setColor(Color.argb(255, 210, 210, 210));
		canvas.drawRect(20, sliderLineY-sliderLineHeight, winWidth-20, sliderLineY, paint);
		//slider line
		if(!autoSwivel)
			paint.setColor(Color.BLACK);
		else
			paint.setColor(Color.GRAY);
		paint.setStrokeWidth(4);
		canvas.drawLine(30, sliderLineY-(sliderLineHeight/2), winWidth-30, sliderLineY-(sliderLineHeight/2), paint);
		//slider ticks
		for(int i = 1; i <= 9; i++){
			canvas.drawLine(30+(sliderLineWidth/10)*i, sliderLineY-sliderLineHeight+10, 30+(sliderLineWidth/10)*i, sliderLineY-10, paint);
		}
		//slider
		if(!autoSwivel)
			paint.setColor(Color.GRAY);
		else
			paint.setColor(Color.LTGRAY);
		sliderPath.reset();
		sliderPath.moveTo(sliderX-sliderWidth, sliderLineY-sliderLineHeight+5);
		sliderPath.lineTo(sliderX+sliderWidth, sliderLineY-sliderLineHeight+5);
		sliderPath.lineTo(sliderX, sliderLineY-5);
		canvas.drawPath(sliderPath, paint);
		
		//angle and dist
		if(autoSwivel && updateAngle)
			angle+=angleChange;
		else if(!autoSwivel)
			angle = (int) map(sliderX,30,winWidth-30,179,0);
		if(angle >= 179-angleIncrease)
			angleChange = -1*angleIncrease;
		if(angle <= 0+angleIncrease)
			angleChange = angleIncrease;
		if(updateAngle){
			distances[angle] = distance;
			updateAngle = false;
		}
		//swivel view
		paint.setColor(Color.RED);
		for(int i = 0; i < distances.length; i++){
			if(distances[i] > 0 && distances[i] <= swivelViewHeight){
				canvas.drawRect(swivelViewXCent+(float)(distances[i]*Math.cos(Math.toRadians(i))-swivelBoxDim/2), swivelViewYCent-(float)(distances[i]*Math.sin(Math.toRadians(i))+swivelBoxDim), swivelViewXCent+(float)(distances[i]*Math.cos(Math.toRadians(i))+swivelBoxDim/2), (swivelViewYCent-(float)(distances[i]*Math.sin(Math.toRadians(i)))), paint);
			}
		}
		paint.setColor(Color.BLACK);
		canvas.drawCircle(swivelViewXCent, swivelViewYCent, swivelBoxDim, paint);
		canvas.drawLine(swivelViewXCent, swivelViewYCent, (float)(swivelViewXCent+Math.cos(Math.toRadians(angle))*swivelViewHeight), (float) (swivelViewYCent-Math.sin(Math.toRadians(angle))*swivelViewHeight), paint);
		if(angle>20)
			canvas.drawLine(swivelViewXCent, swivelViewYCent, (float)(swivelViewXCent+Math.cos(Math.toRadians(angle-20))*swivelViewHeight), (float) (swivelViewYCent-Math.sin(Math.toRadians(angle-20))*swivelViewHeight), paint);
		if(angle<160)
			canvas.drawLine(swivelViewXCent, swivelViewYCent, (float)(swivelViewXCent+Math.cos(Math.toRadians(angle+20))*swivelViewHeight), (float) (swivelViewYCent-Math.sin(Math.toRadians(angle+20))*swivelViewHeight), paint);
		paint.setStyle(Style.STROKE);
		paint.setStrokeWidth(20);
		paint.setColor(Color.WHITE);
		canvas.drawRect(10, 10, winWidth-10, swivelViewYCent+10, paint);
		paint.setColor(Color.GRAY);
		paint.setStrokeWidth(4);
		canvas.drawRect(20, 20, winWidth-20, swivelViewYCent, paint);
		paint.setStyle(Style.FILL);
		
		//button
		settingsButton.img.draw(canvas);
		if(settingsScreen){
			paint.setARGB(100, 100, 100, 100);
			canvas.drawRect(0, 0, winWidth, winHeight, paint);
			if(autoSwivel)
				swivelAutoButton.img.draw(canvas);
			else
				swivelManualButton.img.draw(canvas);
			backButton.img.draw(canvas);
			connectButton.img.draw(canvas);
			unstickButton.img.draw(canvas);
			clearButton.img.draw(canvas);
		}
		
		//account for system lag and keep at updating every 20 mill
    	long endTime = System.nanoTime();
    	long duration = (endTime-startTime)/1000000;
    	
	    //System.out.println(duration);   
    	
    	if (duration<20){
    		//try {Thread.sleep(20-duration);} catch (InterruptedException e) { }
    	}
    	
    	//call repaint
        invalidate();
	}
	
	public void updateTouch(){
		if(!settingsScreen){
			if(touchY > winHeight-winWidth){
				if(touchX < squareHalfSide*0.15)
					touchX = (int) (squareHalfSide*0.15);
				if(touchX > winWidth-squareHalfSide*0.15)
					touchX = (int) (winWidth-squareHalfSide*0.15);
				if(touchY < (winHeight-winWidth)+squareHalfSide*0.15)
					touchY = (int) ((winHeight-winWidth)+squareHalfSide*0.15);
				if(touchY > winHeight-squareHalfSide*0.15)
					touchY = (int) (winHeight-squareHalfSide*0.15);
				knobXDisp = touchX - centerCircleX;
				knobYDisp = touchY - centerCircleY;
			}
			else{
				knobXDisp = 0;
				knobYDisp = 0;
			}
			if(touchX <= winWidth-30 && touchX >= 30 && touchY <= sliderLineY && touchY >= sliderLineY-sliderLineHeight && !autoSwivel){
				sliderX = touchX;
			}
		}
	}

	public void setDim(int width, int height) {
		winWidth = width;
		winHeight = height;
		squareHalfSide = (winWidth - 40)/2;
		centerCircleX = 20+squareHalfSide;
		centerCircleY = winHeight-20-squareHalfSide;
		sliderLineY = winHeight-40- squareHalfSide *2;
		sliderLineHeight = (int) (squareHalfSide *0.2);
		sliderLineWidth = winWidth - 60;
		sliderX = winWidth/2;
		sliderWidth = (sliderLineHeight-10)/2;
		swivelViewYCent = sliderLineY-(sliderLineHeight+20);
		swivelViewXCent = winWidth/2;
		swivelViewHeight = swivelViewYCent - 20;
		for(int i = 0; i < distances.length; i++){
			distances[i] = -1;
		}
		int buttonWidth = (int) (winWidth*0.4);
		int buttonHeight = (buttonWidth*2)/5;
		settingsButton.setBounds(10, winHeight-(sliderLineHeight+10), 10+sliderLineHeight, winHeight-10);
		backButton.setBounds(winWidth/2-buttonWidth/2, (int) (winHeight-buttonHeight*1.5), winWidth/2+buttonWidth/2, (int) (winHeight-buttonHeight*0.5));
		swivelAutoButton.setBounds(winWidth/2-buttonWidth/2, (int) (buttonHeight*0.5), winWidth/2+buttonWidth/2, (int) (buttonHeight*1.5));
		swivelManualButton.setBounds(winWidth/2-buttonWidth/2, (int) (buttonHeight*0.5), winWidth/2+buttonWidth/2, (int) (buttonHeight*1.5));
		connectButton.setBounds(winWidth/2-buttonWidth/2, (int) (buttonHeight*2), winWidth/2+buttonWidth/2, (int) (buttonHeight*3));
		unstickButton.setBounds(winWidth/2-buttonWidth/2, (int) (buttonHeight*3.5), winWidth/2+buttonWidth/2, (int) (buttonHeight*4.5));
		clearButton.setBounds(winWidth/2-buttonWidth/2, (int) (buttonHeight*5), winWidth/2+buttonWidth/2, (int) (buttonHeight*6));
		//swivelBoxDim = angleIncrease*2;
	}
	
	public double map(double x, double in_min, double in_max, double out_min, double out_max){
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}
}
