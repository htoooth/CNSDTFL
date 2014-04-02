-----------------------------------------
--premake4的配置文件
--生成win下vs的工程文件和GUN的makefile文件
--生成单和多进程程序
--作者：htoo
--时间：2013年02月22日
--使用方法：
--  premake4.exe --help
--win下：
--  premake4.exe vs2010
--linux下：
--  premake4.exe gmake
--  make help
--  make config=release all
--  make config=dubug all
--注意：本配置文件的只能生成vs和g++的工程文件
--      对其它IDE暂时不做支持。
-----------------------------------------

--定义路径
   
--项目工程文件路径
local prj_dir="prj/"
--生成二进制文件路径
local target_dir="bin/"

-----------------------------------------

--环境变量定义:环境变量必须设置才能使用
--gdal环境变量设置
--gdali:链接库名
local gdali=nil
--gdallib:链接库所在路径
local gdallib=nil
--gdalinc:头文件所在路径
local gdalinc=nil

--使用mpich的版本
--mpi环境变量设置
--mpii:链接库名
local mpii=nil
--mpiixx:mpi c++ 链接库名
local mpiixx=nil
--mpilib:链接库所在路径
local mpilib=nil
--mpiinc:头文件所在路径
local mpiinc=nil

-----------------------------------------
--以下这几个环境变量必顺在系统环境中设置
--GDALI:GDAL的链接库名
--GDALLIB:GDAL的链接库路径
--GDALINC:GDAL的头文件路径
--MPII:MPI的链接库名
--MPILIB:MPI的链接库路径
--MPIINC:MPI的头文件路径
-----------------------------------------
--win下环境变量设置值:$()
if os.is("windows") then
    gdali="gdal_i"
    gdallib="$(GDALLIB)"
    gdalinc="$(GDALINC)"

    mpii="$(MPII)"
    mpilib="$(MPILIB)"
    mpiinc="$(MPIINC)"

--linux下环境变量设置值:${}
elseif os.is("linux") then
    gdali="gdal"
    gdalinc="${GDALINC}"
    gdallib="${GDALLIB}"

    mpii="mpi"
    --!!注意，这个东西在linux下使用，在win下测试没有作用
    mpiixx="mpi_cxx"
    mpilib="${MPILIB}"
    mpiinc="${MPIINC}"

end

-----------------------------------------
--生成项目文件
--解决方案名
solution "hpgc"

    --设置针对Debug和Release分别设置
    configurations {"Debug", "Release"}
-----------------------------------------
    --对Debug下的情况进行设置，例如定义调试符号
    configuration "Debug"
        --打开跟踪
        defines {"TRACEON","MPICH_SKIP_MPICXX"}

    --其中MPICH_SKIP_MPICXX是指跳过MPICXX的版本，只用c版本的api
    configuration "Release"
        defines {"MPICH_SKIP_MPICXX"}

    --在gun下使用新标准
    --对生成makefile进行单独设置
    --premake4可以对生成的工程文件进行单独设置
    --如可以对vs设置：configuration "vs*"
    configuration "gmake"
        --构建工具选项 
        buildoptions {"--std=c++0x"}
        
    --对Release下的情况进行设置，例如定义除去调试符号
    --configuration "Release"
    --  defines {"NOBUG"}
-----------------------------------------

    --工程文件生成的目录    
    location(prj_dir)
-----------------------------------------

    --并行框架的源文件
    local 
    framework =
    {
        "src/framework.h",
        "src/framework.cpp",
        "src/framework.algorithm.h",
        "src/framework.algorithm.cpp",
        "src/framework.partition.h",
        "src/framework.partition.cpp",

        "src/geoalgorithm.h",
        "src/geoalgorithm.cpp",
        "src/geoalgorithm.format.h",
        "src/geoalgorithm.format.cpp",

        --投影支持
        "src/fmts.csr.h",
        "src/fmts.csr.cpp",

        "src/mpiobject.h",
        "src/mpiobject.cpp",
        "src/mpimessage.h",
        "src/mpimessage.cpp",
        "src/mpimessage.datblock.h",
        "src/mpimessage.datblock.cpp",
        "src/mpioperator.h",
        "src/mpioperator.cpp",
        "src/mpioperator.sendmsg.h",
        "src/mpioperator.sendmsg.cpp",
        "src/mpioperator.recvmsg.h",
        "src/mpioperator.recvmsg.cpp",
        "src/mpiport.constant.h",

        "src/port.debug.h",
        "src/port.datblock.h",
        "src/port.datblock.cpp",
        "src/port.designpattern.h",
        "src/port.time.h",
        "src/port.cputime.cpp",
        "src/port.realtime.cpp"
    }
    
    --将合并两个表的内容
    function merge(table1,table2)
        for i,v in pairs(table1) do
            table.insert(table2,v)
        end
       
        return table2
    end

-----------------------------------------
    --设置项目
    --项目名
    project "CVCT2GDAL"
        --生成二进制程序名
        targetname "cvct2gdal"
        --二进制文件的路径
        targetdir(target_dir) 
        --项目类型为控制台
        kind "ConsoleApp"
        --采用c++语言
        language"c++"

        --vctl格式的文件路径
        local cvct = 
        {
            "src/fmts.cvct.h",
            "src/fmts.cvct.cpp",
            "src/fmts.cvctbuild.h",
            "src/fmts.cvctbuild.cpp",
            "src/fmts.cvctindex.h",
            "src/fmts.cvctindex.cpp",
            "src/fmts.cvctparse.h",
            "src/fmts.cvctparse.cpp",
            "src/fmts.cvct2gdal.h",
            "src/fmts.cvct2gdal.cpp",

            "src/apps.cvct2gdal.cpp",
            "src/geoalgorithm.cvct2gdal.h",
            "src/geoalgorithm.cvct2gdal.cpp"
        }

        --项目源文件路径
        files (merge(framework,cvct))

        --附加库设置
        --链接库名
        links {gdali,mpii,mpiixx}
        --链接库路径
        libdirs {gdallib,mpilib}
        --库的路径
        includedirs {gdalinc,mpiinc}

-----------------------------------------

    --以下同上
    project "CGRD2GDAL"
        targetname "cgrd2gdal"
        targetdir(target_dir) 
        kind "ConsoleApp"
        language"c++"

        local cgrd =
        {
            "src/fmts.cgrd.h",
            "src/fmts.cgrd.cpp",
            "src/fmts.cgrddataset.h",
            "src/fmts.cgrddataset.cpp",
            "src/fmts.cgrdrasterband.h",
            "src/fmts.cgrdrasterband.cpp",

            "src/apps.cgrd2gdal.cpp",
            "src/geoalgorithm.cgrd2gdal.h",
            "src/geoalgorithm.cgrd2gdal.cpp",
        }
        
        files (merge(framework,cgrd))
        links {gdali,mpii,mpiixx}
        libdirs {gdallib,mpilib}
        includedirs {gdalinc,mpiinc}

-----------------------------------------

    project "CIMG2GDAL"
        targetname "cimg2gdal"
        targetdir(target_dir) 
        kind "ConsoleApp"
        language"c++"

        local cimg =
        {
            "src/fmts.cimg.h",
            "src/fmts.cimg.cpp",
            "src/fmts.cimgdataset.h",
            "src/fmts.cimgdataset.cpp",
            "src/fmts.cimgrasterband.h",
            "src/fmts.cimgrasterband.cpp",

            "src/apps.cimg2gdal.cpp",
            "src/geoalgorithm.cimg2gdal.h",
            "src/geoalgorithm.cimg2gdal.cpp"
        }

        files (merge(framework,cimg))
        links {gdali,mpii,mpiixx}
        libdirs {gdallib,mpilib}
        includedirs {gdalinc,mpiinc}

-----------------------------------------

    project "RASTER2RASTER"
        targetname "raster2raster"
        targetdir(target_dir) 
        kind "ConsoleApp"
        language"c++"

        local raster=
        {
            "src/apps.raster2raster.cpp",
            "src/geoalgorithm.raster2raster.h",
            "src/geoalgorithm.raster2raster.cpp",
        }

        files (merge(framework,raster))
        links {gdali,mpii,mpiixx}
        libdirs {gdallib,mpilib}
        includedirs {gdalinc,mpiinc}
