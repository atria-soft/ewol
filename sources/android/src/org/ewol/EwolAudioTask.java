/**
*******************************************************************************
* @file ewol EwolAudioTask.java
* @brief Java Audio interface code.
* @author Edouard DUPIN
* @date 29/06/2012
* @par Project
* ewol
*
* @par Copyright
* Copyright 2011 Edouard DUPIN, all right reserved
*
* This software is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY.
*
* Licence summary : 
*    You can modify and redistribute the sources code and binaries.
*    You can send me the bug-fix
*
* Term of the licence in in the file licence.txt.
*
*******************************************************************************
*/

package org.ewol;

import android.media.AudioTrack;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioRecord;

// import the ewol package :
/* no need in same package... */
//import org.ewol.NativeInterface;

import static org.ewol.Ewol.EWOL;


public class EwolAudioTask implements Runnable, EwolConstants
{
    public static final int SAMPLE_FREQ_44100  = 44100;
    private boolean         m_stopAudioThreads = false;
    private AudioTrack      m_musicTrack       = null;

    // constructor : 
    public EwolAudioTask()
    {
	// nothing to do ...
    }

    public void run()
    {
	if(m_musicTrack != null) {
	    return;
	}
		
	int sampleFreq = SAMPLE_FREQ_44100; //AudioTrack.getNativeOutputSampleRate(AudioManager.STREAM_MUSIC);
	int channelConfig = AudioFormat.CHANNEL_CONFIGURATION_STEREO;
	int audioFormat = AudioFormat.ENCODING_PCM_16BIT;
	int nbChannels = 2;
	// we keep the minimum buffer size, otherwite the delay is too big ...
	int bufferSize = AudioTrack.getMinBufferSize(sampleFreq, channelConfig, audioFormat);
	// Create a streaming AudioTrack for music playback
	short[] streamBuffer = new short[bufferSize];
	m_musicTrack = new AudioTrack(AudioManager.STREAM_MUSIC,
				      SAMPLE_FREQ_44100,
				      AudioFormat.CHANNEL_CONFIGURATION_STEREO,
				      AudioFormat.ENCODING_PCM_16BIT,
				      bufferSize,
				      AudioTrack.MODE_STREAM);
	m_musicTrack.play();
	//m_musicTrack.setPositionNotificationPeriod(2048);
		
	while (!m_stopAudioThreads) {
	    // Fill buffer with PCM data from C++
	    EWOL.audioPlayback(streamBuffer, NATIVE_AUDIO_BUFFER_SIZE, nbChannels);
			
	    // Stream PCM data into the music AudioTrack
	    m_musicTrack.write(streamBuffer, 0, NATIVE_AUDIO_BUFFER_SIZE);
	}
		
	m_musicTrack.flush();
	m_musicTrack.stop();
    }
    public void Pause()
    {
	if(m_musicTrack == null) return;
	m_musicTrack.pause();
    }
    public void Resume()
    {
	if(m_musicTrack == null) return;
	m_musicTrack.play();
    }
    public void AutoStop()
    {
	if(m_musicTrack == null) return;
	m_stopAudioThreads=true;
    }
}/* no ; here !!! */