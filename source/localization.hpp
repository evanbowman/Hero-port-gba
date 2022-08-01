#pragma once


#include "bulkAllocator.hpp"
#include "localeString.hpp"
#include "string.hpp"


void locale_set_language(int language_id);
void locale_set_language_english();
int locale_get_language();


using LocalizedStrBuffer = StringBuffer<1187>;
using LocalizedText = DynamicMemory<LocalizedStrBuffer>;
LocalizedText locale_string(Platform& pfrm, LocaleString ls);


// In most cases, you do not want to call this function directly, better to call
// the localized version, locale_num2str. Only call english__to_string for
// logging purposes, where the language is assumed to be english.
void english__to_string(int num, char* buffer, int base);


void locale_num2str(int num, char* buffer, int base);
