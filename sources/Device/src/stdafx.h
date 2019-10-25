#pragma once

#ifdef WIN32

#include "log.h"
#include "structs.h"
#include "Message.h"
#include "Transceiver.h"

#include "libs/FatFS/FatFS.h"

#include "FDrive/FDriveDevice.h"
#include "FreqMeter/FreqMeter.h"

#include "Generator/GeneratorDevice.h"
#include "Generator/GeneratroSettings.h"

#include "Hardware/Timer.h"
#include "Hardware/Timer4XX.h"
#include "Hardware/CPU.h"
#include "Hardware/VCP.h"
#include "Hardware/Modules/EEPROM.h"
#include "Hardware/Modules/SPI.h"

#include "Interface/HandlersDevice.h"
#include "Interface/InterfaceDevice.h"

#include "Settings/CalibrationSettings.h"
#include "Settings/Settings.h"

#include "Utils/Buffer.h"
#include "Utils/Debug.h"
#include "Utils/Math.h"
#include "Utils/Memory.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"
#include "Utils/Queue.h"

#include <limits>
#include <cmath>
#include <cstdarg>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "usbh_diskio.h"
#include "usbh_conf.h"

#include <usbd_cdc.h>

#endif
