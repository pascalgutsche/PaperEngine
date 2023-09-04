local function getAssemblyFiles(directory, is_windows)
	if is_windows then
        handle = io.popen("dir " .. directory .. " /B /A-D")
    else
        handle = io.popen("ls " .. directory)
	end

	t = {}
	for f in handle:lines() do
		if path.hasextension(f, ".dll") then
			if string.find(f, "System.") then
				table.insert(t, f)
			end
		end
	end

	handle:close()
	return t
end

function linkAppReferences(linkScriptCore)
	local hazelDir = os.getenv("PAPER_DIR")
	local monoLibsPath
    local monoLibsFacadesPath
	local is_windows = os.istarget('windows')

	if is_windows then
		monoLibsPath = path.join(hazelDir, "PaperEditor", "mono", "lib", "mono", "4.5"):gsub("/", "\\")
		monoLibsFacadesPath = path.join(monoLibsPath, "Facades"):gsub("/", "\\")
	end

	libdirs { monoLibsPath, monoLibsFacadesPath }
	if linkScriptCore ~= false then
		links { "scriptcore" }
	end

	for k, v in ipairs(getAssemblyFiles(monoLibsPath, is_windows)) do
		--print("Adding reference to: " .. v)
		links { v }
	end

	for k, v in ipairs(getAssemblyFiles(monoLibsFacadesPath, is_windows)) do
        --print("Adding reference to: " .. v)
        links { v }
    end
end