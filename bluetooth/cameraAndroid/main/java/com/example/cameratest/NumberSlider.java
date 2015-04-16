package com.example.cameratest;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;

public class NumberSlider {
	int sVal;
	int sMin;
	int sMax;
	int sXPos;
	int sYPos;
	int sWidth;
	int sHeight;
	int sXVal;
	String sTitle;
	
	int sPadding = 4;
	int sTextSize = 20;
	
	Paint paint = new Paint();
	
	public NumberSlider(int mi, int ma, int d, int x, int y, int w, int h, String t){
		sMin = mi;
		sMax = ma+1;
		if(d < sMin || d > sMax-1)
			sVal = (sMin+sMax)/2;
		else
			sVal = d;
		sXPos = x;
		sYPos = y;
		sWidth = w;
		sHeight = h;
		sXVal = (int) map(d,sMin,sMax,sXPos,sXPos+sWidth);
		sTitle = t;
	}
	
	public void drawSlider(Canvas canvas){
		paint.setColor(Color.LTGRAY);
		canvas.drawRect(sXPos-sPadding, sYPos-sPadding, sXPos+sWidth+sPadding, sYPos+sHeight+sPadding, paint);
		
		//slider arrow
		paint.setColor(Color.GRAY);
		Path sliderPath = new Path();
		sliderPath.moveTo(sXVal-sHeight/2, sYPos);
		sliderPath.lineTo(sXVal+sHeight/2, sYPos);
		sliderPath.lineTo(sXVal, sYPos+sHeight);
		canvas.drawPath(sliderPath, paint);
		
		paint.setColor(Color.BLACK);
		paint.setTextSize(sTextSize/2);
		canvas.drawText(sTitle, sXPos, sYPos-sPadding*2, paint);
		paint.setTextSize(sTextSize);
		canvas.drawText(""+sVal, sXPos+sWidth/2, sYPos+sHeight+sTextSize, paint);
	}
	
	public void touchSlider(int x, int y){
		if(x >= sXPos && x <= sXPos+sWidth && y >= sYPos && y <= sYPos+sHeight){
			sVal = (int) map(x,sXPos,sXPos+sWidth,sMin,sMax);
			sXVal = x;
		}
	}
	
	public double map(double x, double in_min, double in_max, double out_min, double out_max){
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}
}
