#pragma once

#ifdef WIN32

#include "log.h"
#include "structs.h"
#include "Message.h"
#include "Transceiver.h"

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

#include "Interface/InterfaceDevice.h"

#include "Settings/CalibrationSettings.h"

#include "SCPI/SCPI.h"

#include "Utils/Array.h"
#include "Utils/Debug.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"

#include <limits>
#include <math.h>
#include <stdarg.h>
#include <cstring>
#include <cstdlib>

#include "usbh_diskio.h"
#include "usbh_conf.h"

#endif
