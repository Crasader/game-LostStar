/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.camellia.starfall;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import org.FXPlugin.FXPluginManager;
import org.FXUtils.JniWrapper;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;



import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.Signature;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.view.KeyEvent;


public class star extends Cocos2dxActivity{
	
//	private FacebookLoginFragment facebookLoginFragment = null;
	
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		
		FXPluginManager.init(this, savedInstanceState);
		
		
//		if (facebookLoginFragment == null) {
//	        // Add the fragment on initial activity setup
//			facebookLoginFragment = new FacebookLoginFragment();
//	        getSupportFragmentManager()
//	        .beginTransaction()
//	        .add(android.R.id.content, facebookLoginFragment)
//	        .commit();
//	    } 
		
		//get hash key
		try {
	        PackageInfo info = getPackageManager().getPackageInfo(
	                "com.camellia.starfall", 
	                PackageManager.GET_SIGNATURES);
	        for (Signature signature : info.signatures) {
	            MessageDigest md = MessageDigest.getInstance("SHA");
	            md.update(signature.toByteArray());
	            Log.d("KeyHash:", Base64.encodeToString(md.digest(), Base64.DEFAULT));
	            }
	    } catch (NameNotFoundException e) {

	    } catch (NoSuchAlgorithmException e) {

	    }
		
	}
    

    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// star should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }

    static {
        System.loadLibrary("cocos2dcpp");
    }
    

    

    
    

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
	    super.onActivityResult(requestCode, resultCode, data);
	    FXPluginManager.onActivityResult(requestCode, resultCode, data);
	}
    
    @Override
	public void onResume() {
	    super.onResume();
	    FXPluginManager.onResume();
	}

	@Override
	public void onPause() {
	    super.onPause();
	    FXPluginManager.onPause();
	}

	@Override
	public void onDestroy() {
	    super.onDestroy();
	    FXPluginManager.onDestroy();
	}

	@Override
	public void onSaveInstanceState(Bundle outState) {
	    super.onSaveInstanceState(outState);
	    FXPluginManager.onSaveInstanceState(outState);
	}
    
	//=====================================================
    //退出按键。
    //=====================================================
    @Override  
    public boolean dispatchKeyEvent(KeyEvent event) {  
        if (event.getKeyCode() == KeyEvent.KEYCODE_BACK && event.getAction() == KeyEvent.ACTION_DOWN && event.getRepeatCount() == 0) {   
            AlertDialog isExit = new AlertDialog.Builder(this).create();  
            
            if (JniWrapper.getCurSettingLanguageType() == JniWrapper.LanguageType.kLanguageGerman.value()) {
            	 isExit.setTitle("Hinweis");  
                 isExit.setMessage("Spiel verlassen?");  
                 isExit.setButton("OK", listener);  
                 isExit.setButton2("Abbrechen", listener);  	
			}
            else {
                isExit.setTitle("Hint");  
                isExit.setMessage("Do you want exit the game?");  
                isExit.setButton("OK", listener);  
                isExit.setButton2("Cancel", listener);  
			}
            isExit.show();
        }  
        return super.dispatchKeyEvent(event);  
    }  
    /**按钮回调*/ 
    DialogInterface.OnClickListener listener = new DialogInterface.OnClickListener()  
    {  
        public void onClick(DialogInterface dialog, int which)  
        {  
            switch (which)  
            {  
            case AlertDialog.BUTTON_POSITIVE://是
                star.this.finish();
                FXPluginManager.onDestroy();
                System.exit(0);
                break;  
            case AlertDialog.BUTTON_NEGATIVE://否
            	//JniWrapper.resumeGame();
                break;  
            default:  
                break;  
            }  
        }  
    }; 
	
	
	
	
	
	
}
