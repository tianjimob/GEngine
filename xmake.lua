add_rules("mode.debug", "mode.release", "mode.releasedbg")

set_languages("c99", "c++17")

function join_third_party_dir (str) 
    local third_party_dir = "src/engine/source/third_party/";
    return third_party_dir..str;
end

-- Reflection
target("meta_parser")
    set_kind("binary")
    add_includedirs(join_third_party_dir("libclang/include"), join_third_party_dir("inja/include"))
    if is_os("windows") then
        add_rpathdirs(join_third_party_dir("libclang/bin/windows"))
        add_links("libclang")
        add_linkdirs(join_third_party_dir("libclang/lib/windows"))
    elseif  is_os("linux") then 
        add_rpathdirs(join_third_party_dir("libclang/bin/linux"))
        add_links("clang", "m", "stdc++")
        add_linkdirs(join_third_party_dir("libclang/bin/linux"))
    end 
    add_files("src/engine/source/tool/meta_parser/**.cpp")
    after_build(function (target)
        local generate_meta = function (module_name, project_dir, gen_to_save_dir, include_path)
            os.exec("$(projectdir)/%s %s %s %s %s", target:targetfile(), module_name, project_dir, gen_to_save_dir, include_path)
        end
        -- generate_meta("test", "test/engine/runtime/reflection", "test/engine/runtime/reflection", "src/engine/source/runtime/core/reflection")
        generate_meta("GEngineRuntime", "src/engine/source/runtime", "src/engine/source/runtime", "src/engine/source/runtime")
    end)

rule("run_script")
    on_run(function ()
        os.exec("echo 'Hello, running script!'")
    end)

-- compile glsl to spv
target("generate_spv")
    set_kind("phony")  -- 设置为虚拟目标，不编译任何文件
    local glslangValidator = ""
    if is_os("windows") then
        glslangValidator = join_third_party_dir("VulkanSDK/bin/Win32/glslangValidator.exe")
    elseif  is_os("linux") then 
        glslangValidator = join_third_party_dir("VulkanSDK/bin/linux/glslangValidator")
    end 
    after_build(function (target)      
        local files = os.files("src/engine/source/runtime/shaders/glsl/*");
        local dstDir = "src/engine/source/runtime/shaders/generated/spv/"
        for num, file in ipairs(files) do
            print('['..num..'/'..#files.."] "..file)
            local filename = path.basename(file)
            local extension = path.extension(file)
            os.exec("glslangValidator -V %s -o %s", file, dstDir..filename..extension..".spv")
        end
    end)

-- compile spv to cpp
target("generate_cpp")
    set_kind("phony")  -- 设置为虚拟目标，不编译任何文件
    after_build(function (target)
        local files = os.files("src/engine/source/runtime/shaders/generated/spv/*");
        local dstDir = "src/engine/source/runtime/shaders/generated/cpp/"
        for num, filename in ipairs(files) do
            local file = io.open(filename, "rb")  -- 以只读二进制方式打开文件
            local content = file:read("*all")  -- 读取全部内容
            file:close()
            local lastPart = filename:match("[^%/\\]+$")
            lastPart = lastPart:gsub(".spv", "")
            lastPart = lastPart:gsub("%.", "_")
            local dstFilename = lastPart:lower()..".h"
            lastPart = lastPart:upper()
            local newFile = "#include <vector>\nstatic const std::vector<unsigned char> SHADER_CODE_"..lastPart.." {\n"
            local newLine = 1;
            for i = 1, #content do
                newFile = newFile..string.format("0x%02x", string.byte(content, i))..", "
                newLine = newLine + 1
                if newLine == 16 then
                    newFile = newFile..'\n'
                    newLine = 1
                end
            end
            newFile = newFile.."\n};\n"
            local writeFile, err = io.open(dstDir..dstFilename, "w")
            writeFile:write(newFile)
            writeFile:close()
        end
    end)

target("glad")
    set_kind("static")
    add_includedirs(join_third_party_dir("glad/include"))
    add_files(join_third_party_dir("glad/src/*.c"))

target("ufbx")
    set_kind("static")
    add_includedirs(join_third_party_dir("ufbx"))
    add_files(join_third_party_dir("ufbx/ufbx.c"))

target("tinyobjloader")
    set_kind("static")
    add_includedirs(join_third_party_dir("tinyobjloader"))
    add_files(join_third_party_dir("tinyobjloader/tiny_obj_loader.cc"))

target("GEngineRuntime")
    set_kind("binary")
    set_targetdir("bin")

    add_deps("glad", "ufbx", "tinyobjloader")
    
    add_includedirs("src/engine/source/runtime")
    add_includedirs("src/engine/source/third_party")
    add_includedirs(join_third_party_dir("spdlog/include"), 
                    join_third_party_dir("SDL2/include"), 
                    join_third_party_dir("VulkanSDK/include"), 
                    join_third_party_dir("glad/include"),
                    join_third_party_dir("nlohmann/include"),
                    join_third_party_dir("libsimdpp/"),
                    join_third_party_dir("tinyobjloader"),
                    join_third_party_dir("ufbx"))

    add_defines("VULKAN_VALIDATION_ENABLE")

    add_links("SDL2main", "SDL2")
    if is_os("windows") then 
        add_links("Advapi32", "vulkan-1")
        add_linkdirs(join_third_party_dir("SDL2/lib/windows"), join_third_party_dir("VulkanSDK/lib/windows"))
    elseif is_os("linux") then 
        add_links("vulkan", "m", "stdc++")
    end

    add_files("src/engine/source/runtime/**.cpp")

-- target("Editor")
--     set_kind("binary")
--     add_includedirs("src/engine/source/runtime")
--     add_includedirs("src/engine/source/editor")
--     add_files("src/engine/source/editor/**.cpp")

-- tests
local enable_test = true   

target("catch2")
    set_kind("static")
    add_files("test/catch/catch_amalgamated.cpp")

target("test_engine_runtime_reflection")
    set_kind("binary")
    set_targetdir("/tmp/tests")
    add_deps("meta_parser", "catch2")
    add_includedirs("test/engine/runtime/reflection", "test/catch")
    add_includedirs("src/engine/source/runtime/core/reflection")
    add_files("test/engine/runtime/reflection/test_reflection.cpp", "src/engine/source/runtime/core/reflection/reflection.cpp", "test/engine/runtime/reflection/_generated/reflection/all_reflection.cpp")
    after_build(function (target)
        if enable_test  then 
            os.exec("$(projectdir)/%s", target:targetfile())
        end
    end)

target("test_engine_runtime_ini")
    set_kind("binary")
    set_targetdir("/tmp/tests")
    add_deps("catch2")
    add_includedirs("test/catch")
    add_includedirs("src/engine/source/runtime")
    add_includedirs(join_third_party_dir("spdlog/include"))
    add_files("test/engine/runtime/misc/test_ini.cpp", "src/engine/source/runtime/core/misc/config_cache_ini.cpp", "src/engine/source/runtime/core/misc/util_string.cpp", "src/engine/source/runtime/core/log/logger.cpp")
    after_build(function (target)
        if enable_test  then 
            os.exec("$(projectdir)/%s", target:targetfile())
        end
    end)

target("test_engine_runtime_delegate")
    set_kind("binary")
    set_targetdir("/tmp/tests")
    add_deps("catch2")
    add_includedirs("test/catch")
    add_includedirs("src/engine/source/runtime")
    add_files("test/engine/runtime/delegate/test_delegate.cpp")
    after_build(function (target)
        if enable_test  then 
            os.exec("$(projectdir)/%s", target:targetfile())
        end
    end)

target("test_engine_runtime_bounds")
    set_kind("binary")
    set_targetdir("/tmp/tests")
    add_deps("catch2")
    add_includedirs("test/catch")
    add_includedirs("src/engine/source/runtime")
    add_includedirs(join_third_party_dir("libsimdpp/"))
    add_files("test/engine/runtime/misc/test_bounds.cpp")
    after_build(function (target)
        if enable_test  then 
            os.exec("$(projectdir)/%s", target:targetfile())
        end
    end)

target("test_engine_runtime_dynamic_bvh")
    set_kind("binary")
    set_targetdir("/tmp/tests")
    add_deps("catch2")
    add_includedirs("test/catch")
    add_includedirs("src/engine/source/runtime")
    add_files("test/engine/runtime/misc/test_dynamic_bvh.cpp")
    after_build(function (target)
        if enable_test  then 
            os.exec("$(projectdir)/%s", target:targetfile())
        end
    end)

--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro defination
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

