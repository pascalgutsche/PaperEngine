#include "Engine.h"
#include "ImGuiUtils.h"



namespace Paper::UI
{
	static uint32_t s_Counter = 0;
	static char s_IDBuffer[16] = "##";
	static char s_LabelIDBuffer[1024];

	//const char* GenerateID()
	//{
	//	_itoa_s(s_Counter++, s_IDBuffer + 2, sizeof(s_IDBuffer) - 2, 16);
	//	return s_IDBuffer;
	//}

	//const char* GenerateLabelID(std::string_view label)
	//{
	//	*fmt::format_to_n(s_LabelIDBuffer, std::size(s_LabelIDBuffer), "{}##{}", label, s_Counter++).out = 0;
	//	return s_LabelIDBuffer;
	//}

	
}

std::string CutStringAfterLastSlash(std::string val)
{
	for (int i = val.length(); i >= 0; i--)
	{
		if (val[i] == '\\' || val[i] == '/')
		{
			if (i == val.length()) return "";
			std::string out = "";
			for (int j = ++i; j < val.length(); j++) {
				out += val[j];
			}
			return out;

		}
	}
	return val;
}