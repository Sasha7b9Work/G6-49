#pragma once

#ifdef WIN32

#include "defines.h"
#include "log.h"
#include "structs.h"
#include "Packet.h"
#include "RawMessage.h"
#include "Transceiver.h"

#include "Display/Console.h"
#include "Display/Display.h"
#include "Display/InputWindow.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Display/Text.h"
#include "Display/font/Font.h"

#include "FDrive/FDrivePanel.h"

#include "Generator/Signals.h"

#include "Hardware/CPU.h"
#include "Hardware/LTDC.h"
#include "Hardware/Timer.h"
#include "Hardware/Timer4XX.h"
#include "Hardware/VCP.h"
#include "Hardware/Modules/SPI.h"

#include "Interface/InterfacePanel.h"

#include "Menu/Menu.h"
#include "Menu/Hint.h"
#include "Menu/Pages/AddPageInput.h"
#include "Menu/Pages/PageDebug.h"
#include "Menu/Pages/PageFrequencyCounter.h"
#include "Menu/Pages/PageService.h"
#include "Menu/Pages/Include/PageSignals.h"

#include "Settings/Settings.h"

#include "Utils/BigNumber.h"
#include "Utils/Debug.h"
#include "Utils/Math.h"
#include "Utils/Number.h"
#include "Utils/NumberBuffer.h"
#include "Utils/Statistics.h"
#include "Utils/StringUtils.h"

#include <cstdarg>
#include <cstdio>
#include <string.h>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <limits>

#endif
