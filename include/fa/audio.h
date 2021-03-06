
#ifndef _FA_AUDIO
#define _FA_AUDIO

#include <fa/audio/session.h>
#include <fa/audio/device.h>
#include <fa/audio/stream.h>

/** @addtogroup FaAudio

    Provides real-time audio.
    
    These device run processors on the input and output
    the underlying system, typically physical audio interfaces. A running audio
    computation is represented by a *stream*. Access to the current device setups
    is provided by *sessions*.

    @par Implements
    - fa_equal_t
    - fa_destroy_t (sessions and streams)
    - fa_string_show_t

    @see
    - @ref Devices
 
    @defgroup Fa Fa
    @{
    @defgroup FaAudio Audio
    @{
    */



/** @}
    @}
    */

#endif // _FA_AUDIO

