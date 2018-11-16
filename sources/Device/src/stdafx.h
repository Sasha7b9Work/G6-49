#pragma once

#ifdef WIN32

#include "Log.h"
#include "structs.h"
#include "Packet.h"
#include "Transceiver.h"

#include "FDrive/FDriveDevice.h"
#include "FreqMeter/FreqMeter.h"

#include "Generator/GeneratorDevice.h"
#include "Generator/GeneratroSettings.h"

#include "Hardware/Timer.h"
#include "Hardware/Timer4XX.h"
#include "Hardware/CPU.h"
#include "Hardware/VCP.h"

#include "Interface/InterfaceDevice.h"

#include "SCPI/SCPI.h"

#include "Utils/Array.h"
#include "Utils/Console.h"
#include "Utils/Debug.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"

#include <limits.h>
#include <math.h>
#include <stdarg.h>

#include "usbh_diskio.h"
#include "usbh_conf.h"

#endif
