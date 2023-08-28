#pragma once

namespace PaperED
{
	class SelectionManager
	{
	public:
		static void Select(PaperID paperID);
		static void Deselect();

		static PaperID GetSelection();
		static bool HasSelection();

	private:
		inline static PaperID selection = 0;
	};
}