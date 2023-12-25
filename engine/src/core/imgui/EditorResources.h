#pragma once
#include "Engine.h"
#include "utils/FileSystem.h"
#include "renderer/Texture.h"

namespace PaperED
{
	class EditorResources
	{
	public:

		//Generic
		inline static Shr<Texture> LeftArrowIcon = nullptr;
		inline static Shr<Texture> RightArrowIcon = nullptr;
		inline static Shr<Texture> UpArrowÎcon = nullptr;
		inline static Shr<Texture> ReloadIcon = nullptr;
		inline static Shr<Texture> ClearIcon = nullptr;
		inline static Shr<Texture> GearIcon = nullptr;
		inline static Shr<Texture> SearchIcon = nullptr;

		//Viewport
		inline static Shr<Texture> PauseIcon = nullptr;
		inline static Shr<Texture> PlayIcon = nullptr;
		inline static Shr<Texture> SimulateIcon = nullptr;
		inline static Shr<Texture> StepIcon = nullptr;
		inline static Shr<Texture> StopIcon = nullptr;

		//content browser
		inline static Shr<Texture> FolderIcon = nullptr;
		inline static Shr<Texture> FileIcon = nullptr;



		static void Init()
		{
			//Generic
			LeftArrowIcon = LoadTexture("generic/left.png");
			RightArrowIcon = LoadTexture("generic/right.png");
			UpArrowÎcon = LoadTexture("generic/up.png");
			ReloadIcon = LoadTexture("generic/reload.png");
			ClearIcon = LoadTexture("generic/clear.png");
			GearIcon = LoadTexture("generic/gear.png");
			SearchIcon = LoadTexture("generic/search.png");

			//Viewport
			PauseIcon = LoadTexture("viewport/pause.png");
			PlayIcon = LoadTexture("viewport/play.png");
			SimulateIcon = LoadTexture("viewport/simulate.png");
			StepIcon = LoadTexture("viewport/step.png");
			StopIcon = LoadTexture("viewport/stop.png");

			//content browser
			FolderIcon = LoadTexture("contentbrowser/folder.png");
			FileIcon = LoadTexture("contentbrowser/file.png");
		}

		static void Shutdown()
		{
			//Generic
			LeftArrowIcon.Reset();
			RightArrowIcon.Reset();
			UpArrowÎcon.Reset();
			ReloadIcon.Reset();
			ClearIcon.Reset();
			GearIcon.Reset();
			SearchIcon.Reset();

			//Viewport
			PauseIcon.Reset();
			PlayIcon.Reset();
			SimulateIcon.Reset();
			StepIcon.Reset();
			StopIcon.Reset();

			//content browser
			FolderIcon.Reset();
			FileIcon.Reset();
		}
	private:

		inline static Shr<Texture> LoadTexture(const std::filesystem::path& filePath)
		{
			std::filesystem::path path = std::filesystem::path("resources") / "editor" / filePath;

			if (!FileSystem::Exists(path))
			{
				LOG_ERROR("Could not load icon '{}'", path.string());
				return nullptr;
			}

			return Texture::CreateTexture(TextureSpecification(), path);
		}
	};
}
