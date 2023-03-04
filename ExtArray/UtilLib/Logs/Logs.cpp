///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
// Модуль для создания файлов-логов работы программы.
// 
// Файл с исходным кодом модуля.
// 
// Версия: 1.0.1.0
// Дата последнего изменения: 18:11 28.01.2023
// 
// Автор: Маслов А.С. (https://github.com/ArtemMaslov).
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <cstdarg>

#include "../TargetOS.h"
#include "../FilesIO/FilesIO.h"

#include "Logs.h"
#include "logs_config_private.h"

using namespace UtilLib;

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

#undef LogLine
#undef LogFLine
#undef LogAddTrace
#undef LogBeginDump

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

// Выравнивание не важно.
#if defined(MSVC)
    #pragma warning (push)
    #pragma warning (disable: 4820)
#endif

/**
 * @brief Структура файла логов.
*/
struct LogFile
{
    /// Указатель на файл.
    FILE* file;
    /// Сигнатура файла.
    LogSignature sig;
    /// Заголовок.
    const char* caption;
    /// Имя файла.
    const char* fileName;
};

#if defined(MSVC)
    #pragma warning (pop)
#endif

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

/// Размер буфера для форматирования даты и времени сообщения логов.
static const size_t BufferTimeSize  = 40;

/// Количество лог-файлов.
static const size_t LogFilesSize    = (size_t)LogSignature::Reserved;
/// Массив файловых дескрипторов.
static LogFile      LogFiles[LogFilesSize];
/// Был ли вызван конструктор логов.
static bool         LogFilesCreated = false;

/// В конце файла логов находятся закрывающие html-теги. Сообщения в файл пишутся не в конец, а перед этими тегами.
static int          TextOffset      = 0;

/// Номер записанной в файл логов строки.
/// Общий для всех файлов.
/// Так можно отследить порядок сообщений в разных файлах.
static size_t       AbsoluteLogLineIndex = 0;

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

namespace UtilLib
{
    /**
     * @brief  Конструктор отдельного файла логов. Вызывается внутри LogsConstructor().
     * 
     * @param logFileName Имя выходного файла логов (включая путь к нему).
     * @param caption     Текстовый заголовок в начале файла логов.
     * @param sig         Сигнатура файла логов.
     * 
     * @return LogError::FileOpen, если не удалось создать файл. 
     *         LogError::NoErrors, если нет ошибок.
    */
    static LogError LogConstructor(const char* const logFileName, const char* const caption, const LogSignature sig);

    /**
     * @brief  Возвращает указатель на цвет данного уровня логирования.
     * 
     * @param level Уровень логирования.
     * 
     * @return Указатель на цвет текста.
    */
    static const char* const GetLogLevelColor(const LogLevel level);

    /**
     * @brief  Выводит в буфер отформатированное текущее время.
     * 
     * @param buffer Выходное отформатированное время. Размер буфера должен быть не меньше BufferTimeSize.
    */
    static void FormatCurrentTime(char* const buffer);
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

LogError UtilLib::LogsConstructor()
{
    LogFilesCreated = true;

    FilesIoError directoryState = DirectoryExist("./" LOGS_FOLDER);
    if (directoryState == FilesIoError::Exist)
    {
    #if defined(WINDOWS)
        system("rd " LOGS_FOLDER "/s /q");
    #elif defined(LINUX)
        int status = 0;
        
        status = system("rm -r ./" LOGS_FOLDER);
        if (status == -1 || status == 127)
        {
            puts("Не удалось удалить старые файлы логов.");
            return LogError::FileOpen;
        }
    #endif
        if (DirectoryCreate("./" LOGS_FOLDER) == FilesIoError::DirectoryCreate)
        {
            puts("Не удалось создать папку с логами.");
            return LogError::FileOpen;
        }
    }
    else if (directoryState == FilesIoError::NotExist)
    {
        if (DirectoryCreate("./" LOGS_FOLDER) == FilesIoError::DirectoryCreate)
        {
            puts("Не удалось создать папку с логами.");
            return LogError::FileOpen;
        }
    }

    LogError logError = LogError::NoErrors;

    logError = LogConstructor(LOGS_FOLDER "log_general.html", "Общий лог программы.", LogSignature::General);
    if (logError != LogError::NoErrors)
    {
        return logError;
    }

    ///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
    // Добавить ещё один файл логов здесь. В случае ошибки не забыть закрыть уже открытые файлы с логами.
    ///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

    // В деструкторе есть проверка на повторный вызов.
    atexit(LogsDestructor);

    return logError;
}

static LogError UtilLib::LogConstructor(const char* const logFileName, const char* const caption, const LogSignature sig)
{
    assert(logFileName);
    assert(caption);

    const size_t logIndex = (size_t)sig;
    assert(logIndex < LogFilesSize);

    ///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

    FILE* const file = fopen(logFileName, "w");
    if (!file)
    {
        return LogError::FileOpen;
    }

    // Делаем вывод небуферизованным.
    setbuf(file, nullptr);

    ///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
    
    LogFiles[logIndex].file     = file;
    LogFiles[logIndex].fileName = logFileName;
    LogFiles[logIndex].caption  = caption;
    LogFiles[logIndex].sig      = sig;

    char buffer[BufferSize]         = "";
    char bufferTime[BufferTimeSize] = "";
    FormatCurrentTime(bufferTime);

    ///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

    snprintf(buffer, BufferSize, 
             "<html>\n"
             "<head>"
             "<meta charset=\"" LOG_CODEPAGE "\">"
             "<title>%s</title>"
             "<style>font {line-height: " LOG_LINE_HEIGHT ";} "
             "body {background-color: " LOG_BACKGROUND_COLOR ";  font-size: " LOG_TEXT_SIZE "} "
             "head {background-color: " LOG_BACKGROUND_COLOR ";}"
             "</style>"
             "</head>\n"
             "<body>\n"
             "<h1><font color=\"" LOG_HEADER_COLOR "\">%s %s.</font></h1>\n", caption, caption, bufferTime);
    
    fputs(buffer, file);

    ///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

    TextOffset = ftell(file);

    fputs("</body>\n"
          "</html>\n", file);

    TextOffset -= ftell(file);
    
    return LogError::NoErrors;
}

void UtilLib::LogsDestructor() noexcept
{
    assert(LogFilesCreated);

    static bool LogFilesClosed = false;
    if (LogFilesClosed)
        return;

    for (size_t st = 0; st < LogFilesSize; st++)
    {
        LogFile logFile = LogFiles[st];
        if (logFile.file)
            fclose(logFile.file);
    }

    LogFilesClosed = true;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

void UtilLib::LogLine(const char* const message, const LogLevel logLevel, const LogSignature sig, const bool dublicateToConsole,
                const char* const funcName, const char* const fileName, const int logLine)
{
    assert(message);
    assert(funcName);
    assert(fileName);

    assert(LogFilesCreated);

    const size_t logIndex = (size_t)sig;
    assert(logIndex < LogFilesSize);

    ///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
    
    char  buffer[BufferSize]         = "";
    char  bufferTime[BufferTimeSize] = "";
    FormatCurrentTime(bufferTime);

    const char* const logLevelColor  = GetLogLevelColor(logLevel);
    
    ///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

    fseek(LogFiles[logIndex].file, TextOffset, SEEK_END);

    snprintf(buffer, BufferSize, "<pre>"
            "<font color=\"%s\">%20zd> %-.8s [%-.8s] > </font>"
             "<font color = \"" LOG_FUNCT_COLOR "\">%s</font>\n"
             "<font color = \"" LOG_FILE_COLOR "\"> in %s</font>\n"
             "<font color = \"" LOG_LINE_COLOR "\"> at %d line</font>\n"
             "<font color = \"" LOG_MESSAGE_COLOR "\">\t %s</font>"
            "</pre>\n",
            logLevelColor, AbsoluteLogLineIndex++, bufferTime, LogLevelMessages[(size_t)logLevel],
            funcName,
            fileName,
            logLine,
            message);

    fputs(buffer, LogFiles[logIndex].file);

    ///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

    if (dublicateToConsole)
    {
        fputs(message, stderr);
        fputc('\n', stderr);
    }

    fputs("</body>\n"
          "</html>\n", LogFiles[logIndex].file);
}

void UtilLib::LogFLine(const LogLevel logLevel, const LogSignature sig, const bool dublicateToConsole,
                const char* const funcName, const char* const fileName, const int logLine, const char* const format, ...)
{
    assert(format);
    assert(funcName);
    assert(fileName);

    assert(LogFilesCreated);

    const size_t logIndex = (size_t)sig;
    assert(logIndex < LogFilesSize);

    ///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
    
    char  buffer[BufferSize]         = "";
    char  bufferTime[BufferTimeSize] = "";
    FormatCurrentTime(bufferTime);

    const char* const logLevelColor  = GetLogLevelColor(logLevel);
    
    ///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
    
    fseek(LogFiles[logIndex].file, TextOffset, SEEK_END);

    snprintf(buffer, BufferSize, "<pre>"
            "<font color=\"%s\">%20zd %-.8s [%-.8s] > </font>"
            "<font color = \"" LOG_FUNCT_COLOR "\">%s</font>\n"
            "<font color = \"" LOG_FILE_COLOR "\"> in %s</font>\n"
            "<font color = \"" LOG_LINE_COLOR "\"> at %d line</font>\n"
            "<font color = \"" LOG_MESSAGE_COLOR "\">",
            logLevelColor, AbsoluteLogLineIndex++, bufferTime, LogLevelMessages[(size_t)logLevel],
            funcName,
            fileName,
            logLine);
    
    fputs(buffer, LogFiles[logIndex].file);

    ///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

    va_list args;
	va_start(args, format);
    vsnprintf(buffer, BufferSize, format, args);
    va_end(args);

    ///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
    
    if (dublicateToConsole)
    {
        fputs(buffer, stderr);
        fputc('\n', stderr);
    }
    
    fputs(buffer, LogFiles[logIndex].file);
    
    fputs("</font></pre>\n"
          "</body>\n"
          "</html>\n", LogFiles[logIndex].file);
}

void UtilLib::LogAddTrace(const LogSignature sig, const LogTrace trace,
                const char* const funcName, const char* const fileName, const int logLine)
{
    assert(funcName);
    assert(fileName);
    assert(LogFilesCreated);

    ///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
        
    const size_t logIndex = (size_t)sig;
    assert(logIndex < LogFilesSize);

    char  buffer[BufferSize]         = "";
    char  bufferTime[BufferTimeSize] = "";
    FormatCurrentTime(bufferTime);

    const char* logColor             = LOG_TRACE_COLOR;
    if (trace == LogTrace::Error)
        logColor = LOG_ERR_TRACE_COLOR;
    
    ///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

    fseek(LogFiles[logIndex].file, TextOffset, SEEK_END);

    snprintf(buffer, BufferSize, 
             "<pre>\n"
                "\t<font color=\"%s\">\n"
             "%20zd %-.8s [%-.8s%d] > %s\n"
             "\t\tin %s\n"
             "\t\tat %d line\n"
                "\t</font>\n"
             "</pre>\n",
             logColor, AbsoluteLogLineIndex++, bufferTime, LogLevelMessages[(size_t)LogLevel::Trace], (int)trace,
             funcName,
             fileName,
             logLine);

    fputs(buffer, LogFiles[logIndex].file);

    fputs("</body>\n"
          "</html>\n", LogFiles[logIndex].file);
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

FILE* UtilLib::LogBeginDump(const LogSignature sig, const LogLevel logLevel,
             const char* const funcName, const char* const fileName, const int logLine)
{
    assert(funcName);
    assert(fileName);

    assert(LogFilesCreated);

    const size_t logIndex = (size_t)sig;
    assert(logIndex < LogFilesSize);

    ///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

    char  buffer[BufferSize]         = "";
    char  bufferTime[BufferTimeSize] = "";
    FormatCurrentTime(bufferTime);

    ///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

    fseek(LogFiles[logIndex].file, TextOffset, SEEK_END);

    snprintf(buffer, BufferSize,
             "<pre>\n"
             "\t<font color=\"" LOG_DUMP_COLOR "\">\n"
             "%20zd %-.8s [%-.8s] > %s\n"
             "\t\tin %s\n"
             "\t\tat %d line\n",
             AbsoluteLogLineIndex++, bufferTime, LogLevelMessages[(size_t)logLevel],
             funcName,
             fileName,
             logLine);

    fputs(buffer, LogFiles[logIndex].file);

    return LogFiles[logIndex].file;
}

void UtilLib::LogEndDump(const LogSignature sig)
{
    const size_t logIndex = (size_t)sig;
    assert(logIndex < LogFilesSize);

    fputs("\t</font>\n"
          "</pre>\n"
          "</body>\n"
          "</html>\n", LogFiles[logIndex].file);
}

void UtilLib::LogAddImage(const LogSignature sig, const char* imagePath)
{
    assert(imagePath);

    const size_t logIndex = (size_t)sig;
    assert(logIndex < LogFilesSize);

    fseek(LogFiles[logIndex].file, TextOffset, SEEK_END);

    fprintf(LogFiles[logIndex].file, "<img src = \"%s\" height = 600>\n", imagePath);

    fputs("</body>\n"
          "</html>\n", LogFiles[logIndex].file);
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

static const char* const UtilLib::GetLogLevelColor(const LogLevel level)
{
    switch (level)
    {
        case LogLevel::Debug:
            return LOG_DEBUG_COLOR;
        case LogLevel::Warning:
            return LOG_WARNING_COLOR;
        case LogLevel::Error:
            return LOG_ERROR_COLOR;
        case LogLevel::Info:
            return LOG_INFO_COLOR;

        case LogLevel::Trace:
            // Функция не используется для LogLevel::Trace.
        default:
            assert(!"Not implemented");
            // Устанавливаем цвет ошибки, чтобы бросалось в глаза в Release, если assert не срабатывал в Debug.
            return LOG_ERROR_COLOR;
    }
}

static void UtilLib::FormatCurrentTime(char* const buffer)
{
    const time_t    rawTime = time(nullptr);
    const tm* const curTime = localtime(&rawTime);
    strftime(buffer, BufferTimeSize, "%H:%M:%S", curTime);
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///