
/*
 * Ekiga -- A VoIP and Video-Conferencing application
 * Copyright (C) 2000-2009 Damien Sandras <dsandras@seconix.com>

 * This program is free software; you can  redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version. This program is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Ekiga is licensed under the GPL license and as a special exception, you
 * have permission to link or otherwise combine this program with the
 * programs OPAL, OpenH323 and PWLIB, and distribute the combination, without
 * applying the requirements of the GNU GPL to the OPAL, OpenH323 and PWLIB
 * programs, as long as you do follow the requirements of the GNU GPL for all
 * the rest of the software thus combined.
 */


/*
 *                         audioutput-info.h  -  description
 *                         ------------------------------------------
 *   begin                : written in 2008 by Matthias Schneider
 *   copyright            : (c) 2008 by Matthias Schneider
 *   description          : Declaration of structs and classes used for communication
 *                          with the AudioOutputManagers
 *
 */

#ifndef __AUDIOOUTPUT_INFO_H__
#define __AUDIOOUTPUT_INFO_H__

#include "device-def.h"

namespace Ekiga
{
  class AudioOutputDevice : public Device
  {
public:
    AudioOutputDevice () : Device () {}
    AudioOutputDevice (const std::string & t,
                      const std::string & s,
                      const std::string & n) : Device (t, s, n) {};
  };

  typedef struct AudioOutputSettings {
    unsigned volume;
    bool modifyable;
  } AudioOutputSettings;

  enum AudioOutputErrorCodes {
    AO_ERROR_NONE = 0,
    AO_ERROR_DEVICE,
    AO_ERROR_WRITE
  };

  enum AudioOutputPS {
    primary = 0,
    secondary
  };

};

#endif
