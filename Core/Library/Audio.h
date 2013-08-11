/*
 *  Audio.h
 *
 *  QSound implemenation using ALSA. Original implemenation by EB
 *  found at http://lists.trolltech.com/qt-interest/2007-01/thread00147-0.html
 *  with minor changes and support for compiling on Windows/OSX by Dan Krusi.
 *
 *  ===========================================================================
 *
 *	Copyright 2006 by EB (yfh2@xxxxxxxxxxx)
 *  Copyright 2008-2009 Daniel Kruesi (Dan Krusi) and David Grob
 *
 *  This file is part of the emms framework.
 *
 *  The emms framework  is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The emms framework is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <QSound>

#ifdef Q_OS_LINUX
#include <alsa/asoundlib.h> // If this is missing, just "sudo apt-get install libasound2-dev". Don't forget to link to -lasound!
#endif

#ifdef Q_OS_LINUX
typedef  struct
{       u_int32_t  dwSize ;
        u_int16_t  wFormatTag ;
        u_int16_t  wChannels ;
        u_int32_t  dwSamplesPerSec ;
        u_int32_t  dwAvgBytesPerSec ;
        u_int16_t  wBlockAlign ;
        u_int16_t  wBitsPerSample ;
} WAVEFORMAT ;
#endif

class QAlsaSound : public QSound
{
Q_OBJECT
public :
        QAlsaSound( const QString& filename, QObject* parent=0) ;
        ~QAlsaSound() {};

        QString Path ;
        bool initialise() ;
        bool isAvailable() { return is_available ; }
        bool is_available;
        void play(bool drain = true);

#ifdef Q_OS_LINUX
private:
        /* ALSA parameters */
        snd_pcm_t               *handle;
        snd_pcm_sframes_t       frames;
        char                    *device ;                        /* playback device */
        snd_pcm_uframes_t chunk_size, buffer_size;
        size_t bits_per_sample, bits_per_frame, chunk_bytes;

        /* File parser */
        int     fd;                             /* Open file descriptor or -1 */
        char* findchunk(char* pstart, char* fourcc, size_t n);
        WAVEFORMAT waveformat ;
        u_long samples, datastart;
#endif
};



#endif // _AUDIO_H_



