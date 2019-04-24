#pragma once
#include "Utils/String.h"
#include "Utils/List.h"


namespace SCPI
{
    class Parser
    {
    public:
        /// Попытка разобрать первую команду из SCPI::Buffer на составляющие. После разора в words хранятся составляющие
        static void Parse();
        static void ClearList();
        static List<String> words;
    private:
        static void ParseStar();
        static void ParseColon();
        /// Создаёт строку, где first - индекс первого символа, а last - индекс последнего символа и возвращает указатель на неё
        static String *CreateString(uint first, uint last);
    };
}
