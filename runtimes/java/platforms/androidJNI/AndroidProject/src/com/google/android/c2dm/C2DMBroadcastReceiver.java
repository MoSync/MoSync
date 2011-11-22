package com.google.android.c2dm;
/*
 * Copyright 2010 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

/**
 * Helper class to handle BroadcastReciver behavior.
 * - can only run for a limited amount of time - it must start a real service
 * for longer activity
 * - must get the power lock, must make sure it's released when all done.
 *
 */
public class C2DMBroadcastReceiver extends BroadcastReceiver {

    @Override
    public final void onReceive(Context context, Intent intent) {
        C2DMBaseReceiver.runIntentInService(context, intent);
        setResult(Activity.RESULT_OK, null /* data */, null /* extra */);
    }
}
