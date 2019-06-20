#pragma once

#ifdef WIN32

#pragma warning(push)

#pragma warning(disable:4514)

#include "defines.h"
#include "log.h"
#include "structs.h"
#include "Message.h"
#include "Transceiver.h"
#include "CommonTypes.h"

#include "Display/Console.h"
#include "Display/Display.h"
#include "Display/InputWindow.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Display/Text.h"
#include "Display/font/Font.h"

#include "FDrive/FDrivePanel.h"
#include "FDrive/File.h"
#include "FDrive/Items.h"

#include "Generator/Signals.h"

#include "Hardware/CPU.h"
#include "Hardware/LTDC.h"
#include "Hardware/Timer.h"
#include "Hardware/Timer4XX.h"
#include "Hardware/Modules/EEPROM.h"
#include "Hardware/Modules/SPI.h"

#include "Interface/HandlersPanel.h"
#include "Interface/InterfacePanel.h"

#include "Menu/Menu.h"
#include "Menu/Hint.h"
#include "Menu/Pages/Addition/PageInput.h"
#include "Menu/Pages/Addition/PageLoadForm.h"
#include "Menu/Pages/PageDebug.h"
#include "Menu/Pages/PageFrequencyCounter.h"
#include "Menu/Pages/PageService.h"
#include "Menu/Pages/Include/PageSignals.h"

#include "SCPI/SCPI.h"
#include "SCPI/VCP.h"
#include "Settings/Settings.h"
#include "Settings/CalibrationSettings.h"

#include "Utils/BigNumber.h"
#include "Utils/Buffer.h"
#include "Utils/Debug.h"
#include "Utils/List.h"
#include "Utils/Math.h"
#include "Utils/Number.h"
#include "Utils/NumberBuffer.h"
#include "Utils/Queue.h"
#include "Utils/Statistics.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"

#pragma warning(disable:5039)

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <limits>

#pragma warning(pop)

#endif
