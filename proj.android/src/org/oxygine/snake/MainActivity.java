package org.oxygine.snake;

import org.oxygine.lib.OxygineActivity;
import com.google.android.gms.ads.*;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

public class MainActivity extends OxygineActivity
{
	private InterstitialAd mAd;
	private AdRequest.Builder adRequestBuilder;
	private boolean noAds;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		createAd();
	}

	private void createAd() {
		if(R.string.banner_ad_unit_id == "ca-app-pub-xxxxxxxxxxxxxxxxxxxxxxxxxxx") {
			noAds = true;
			return;
		} else {
			noAds = false;
		}
		Log.v("Snake", "Called createAd()");
		mAd = new InterstitialAd(this);
		mAd.setAdUnitId(getString(R.string.banner_ad_unit_id));
		mAd.setAdListener(new AdListener() {
			@Override
			public void onAdLoaded() {
				//Toast.makeText(MainActivity.this, "The ad is loaded", Toast.LENGTH_SHORT).show();
				Log.v("Snake", "The ad is loaded");
			}

		@Override
		public void onAdFailedToLoad(int errorCode) {
			Log.v("Snake", "The ad is failed");
			switch(errorCode) {
			case AdRequest.ERROR_CODE_INTERNAL_ERROR:
				Log.v("Snake", "The ad is failed INTERNAL_ERROR");
				break;
			case AdRequest.ERROR_CODE_INVALID_REQUEST:
				Log.v("Snake", "The ad is failed INVALID_REQUEST");
				break;
			case AdRequest.ERROR_CODE_NETWORK_ERROR:
				Log.v("Snake", "The ad is failed NETWORK ERROR");
				break;
			case AdRequest.ERROR_CODE_NO_FILL:
				Log.v("Snake", "The ad is failed NO_FILL");
				break;
			}
		}
		});
		
		adRequestBuilder = new AdRequest.Builder();
		adRequestBuilder.addTestDevice(AdRequest.DEVICE_ID_EMULATOR);
		adRequestBuilder.addTestDevice("529C587BA3A59B55B1FA99A10A8E6DE8");

	}

	private void loadAd() {
		if (noAds)
			return;
		runOnUiThread(new Runnable() {
			public void run() {
				mAd.loadAd(adRequestBuilder.build());
				Log.v("Snake", "loading ad");
			}
		});
	}

	public void showAd() {
		if (noAds)
			return;
		runOnUiThread(new Runnable() {
			public void run() {
				Log.v("Snake", "showing ad");
				if(mAd.isLoaded())
					mAd.show();
			}
		});
	}

	/*
	@Override
	public void onDestroy() {
		super.onDestroy();
		showAd();
	}
	*/

}
