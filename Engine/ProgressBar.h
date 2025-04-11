#pragma once
#ifndef PROGRESS_H

#include "Rendering.h"

class ProgressBar {
public:
	ProgressBar(const std::wstring& title, bool showPercentage = true, bool preRendering = true) : title(title), barWidth(pWindow->GetScrWidth() - 1), showPercentage(showPercentage), preRendering(preRendering){
	}

	void UpdateProgressBar(double percentage)
	{
		if (preRendering)
			pWindow->ClearAllWindow();
		int pos = barWidth * percentage;
		int barHeigth = pWindow->GetScrHeight() - 1;
		pWindow->PrintSymbol(0, barHeigth, L'[', FG_WHITE);
		pWindow->PrintSymbol(barWidth, barHeigth, L']', FG_WHITE);
		if (!title.empty())
			pWindow->PrintMsgInCenter(barHeigth - 1, FG_WHITE, L"%s %c", title.c_str(), spinner[pos % 4]);

		for (int i = 1; i <= pos; i++)
		{
			if (i < pos)
				pWindow->PrintSymbol(i, barHeigth, L'=', FG_WHITE);
			else if(i==pos)
				pWindow->PrintSymbol(i, barHeigth, L'>', FG_CYAN);
		}
		if (showPercentage)
			pWindow->PrintMsg(barWidth - 4, barHeigth, FG_CYAN, L"%d%%", (int)(percentage * 100));

		if (preRendering)
			pWindow->UpdateWindow();
	}
private:
	int barWidth = 0;
	std::wstring title;
	bool showPercentage, preRendering;
	wchar_t spinner[4] = { L'-', L'\\', L'|', L'/' };
};


#endif
