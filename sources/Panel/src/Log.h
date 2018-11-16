#pragma once   

#if defined(DEBUG) && !defined(WIN32)
#define LOG_WRITE(...)          Log::Write(TypeTrace::Info, __VA_ARGS__)
#define LOG_WRITE_FINALIZE(...) Log::Write(TypeTrace::Info, __VA_ARGS__); Painter::BeginScene(Color::BACK); Console::Draw(); Painter::EndScene();
#define LOG_ERROR(...)          Log::Write(TypeTrace::Error, __VA_ARGS__)
#define LOG_WRITE_TRACE(...)    Log::Trace(TypeTrace::Info, __MODULE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_ERROR_TRACE(...)    Log::Trace(TypeTrace::Error, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
// Когда нужен вспомогательный буфер для преобразования числа в строку, можно пользоваться этой функцией
#define LOG_WRITE_BUF(...)      { char buffer[100]; Log_Write(TypeTrace_Info, __VA_ARGS__); }
#define LOG_FUNC_ENTER          Log::Write(TypeTrace::Info, "%s enter", __FUNCTION__);
#define LOG_FUNC_LEAVE          Log::Write(TypeTrace::Info, "%s leave", __FUNCTION__);
#define LOG_TRACE()             Log::Write(TypeTrace::Info, "%s : %d", __FILE__, __LINE__);
#define ASSEERT(cond, ...)      if(cond)(LOG_ERROR_TRACE(__VA_ARGS__));
#define ASSERT_RET(cond, ...)   if(cond) {LOG_ERROR_TRACE(__VA_ARGS__); return; }
#elif defined(WIN32)
#define ASSERT_RET(cont, ...)
#define LOG_ERROR_TRACE(...)
#define LOG_WRITE(...)          Log::Write(TypeTrace::Info, __VA_ARGS__)
#define LOG_TRACE
#define LOG_ERROR(...)          Log::Write(TypeTrace::Error, __VA_ARGS__)
#define LOG_FUNC_ENTER
#define LOG_WRITE_FINALIZE(...)
#else
#define LOG_WRITE(...)
#define LOG_ERROR(...)
#define LOG_WRITE_TRACE(...)
#define LOG_ERROR_TRACE(...)
#define LOG_WRITE_BUF(...)
#define LOG_FUNC_ENTER
#define LOG_FUNC_LEAVE
#define LOG_TRACE
#define ASSERT(const, ...)
#define ASSERT_RET(cont, ...)
#endif


struct TypeTrace
{
    enum E
    {
        Info,
        Error
    } value;
};

class Log
{
public:
    static void Trace(TypeTrace::E type, const char *module, const char *func, int numLine, char *format, ...);
    static void Write(TypeTrace::E type, char *format, ...);
    static void DisconnectLoggerUSB();
    static void EnableLoggerUSB(bool enable);
    static int GetNumStrings();
};
