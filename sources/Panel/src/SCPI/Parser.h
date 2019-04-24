#pragma once
#include "Utils/String.h"
#include "Utils/List.h"


namespace SCPI
{
    class Parser
    {
    public:
        /// ѕопытка разобрать первую команду из SCPI::Buffer на составл€ющие. ѕосле разора в words хран€тс€ составл€ющие
        static void Parse();
        static void ClearList();
        static List<String> words;
    private:
        static void ParseStar();
        static void ParseColon();
    };
}
