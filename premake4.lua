-----------------------------------------
--premake4�������ļ�
--����win��vs�Ĺ����ļ���GUN��makefile�ļ�
--���ɵ��Ͷ���̳���
--���ߣ�htoo
--ʱ�䣺2013��02��22��
--ʹ�÷�����
--  premake4.exe --help
--win�£�
--  premake4.exe vs2010
--linux�£�
--  premake4.exe gmake
--  make help
--  make config=release all
--  make config=dubug all
--ע�⣺�������ļ���ֻ������vs��g++�Ĺ����ļ�
--      ������IDE��ʱ����֧�֡�
-----------------------------------------

--����·��
   
--��Ŀ�����ļ�·��
local prj_dir="prj/"
--���ɶ������ļ�·��
local target_dir="bin/"

-----------------------------------------

--������������:���������������ò���ʹ��
--gdal������������
--gdali:���ӿ���
local gdali=nil
--gdallib:���ӿ�����·��
local gdallib=nil
--gdalinc:ͷ�ļ�����·��
local gdalinc=nil

--mpi������������
--mpii:���ӿ���
local mpii=nil
--mpiixx:mpi c++ ���ӿ���
local mpiixx=nil
--mpilib:���ӿ�����·��
local mpilib=nil
--mpiinc:ͷ�ļ�����·��
local mpiinc=nil

-----------------------------------------
--�����⼸������������˳��ϵͳ����������
--GDALI:GDAL�����ӿ���
--GDALLIB:GDAL�����ӿ�·��
--GDALINC:GDAL��ͷ�ļ�·��
--MPII:MPI�����ӿ���
--MPILIB:MPI�����ӿ�·��
--MPIINC:MPI��ͷ�ļ�·��
-----------------------------------------
--win�»�����������ֵ:$()
if os.is("windows") then
    gdali="gdal_i"
    gdallib="$(GDALLIB)"
    gdalinc="$(GDALINC)"

    mpii="$(MPII)"
    mpilib="$(MPILIB)"
    mpiinc="$(MPIINC)"

--linux�»�����������ֵ:${}
elseif os.is("linux") then
    gdali="gdal"
    gdalinc="${GDALINC}"
    gdallib="${GDALLIB}"

    mpii="mpi"
    --!!ע�⣬���������linux��ʹ�ã���win�²���û������
    mpiixx="mpi_cxx"
    mpilib="${MPILIB}"
    mpiinc="${MPIINC}"

end

-----------------------------------------
--������Ŀ�ļ�
--���������
solution "hpgc"

    --�������Debug��Release�ֱ�����
    configurations {"Debug", "Release"}
-----------------------------------------
    --��Debug�µ�����������ã����綨����Է���
    configuration "Debug"
        --�򿪸���
        defines {"TRACEON"}

    --��gun��ʹ���±�׼
    --������makefile���е�������
    --premake4���Զ����ɵĹ����ļ����е�������
    --����Զ�vs���ã�configuration "vs*"
    configuration "gmake"
        --��������ѡ�� 
        buildoptions {"--std=c++0x"}
        
    --��Release�µ�����������ã����綨���ȥ���Է���
    --configuration "Release"
    --  defines {"NOBUG"}
-----------------------------------------

    --�����ļ����ɵ�Ŀ¼    
    location(prj_dir)
-----------------------------------------

    --���п�ܵ�Դ�ļ�
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

        --ͶӰ֧��
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
        "src/port.designpattern.h"
    }
    
    --���ϲ������������
    function merge(table1,table2)
        for i,v in pairs(table1) do
            table.insert(table2,v)
        end
       
        return table2
    end

-----------------------------------------
    --������Ŀ
    --��Ŀ��
    project "CVCT2GDAL"
        --���ɶ����Ƴ�����
        targetname "cvct2gdal"
        --�������ļ���·��
        targetdir(target_dir) 
        --��Ŀ����Ϊ����̨
        kind "ConsoleApp"
        --����c++����
        language"c++"

        --vctl��ʽ���ļ�·��
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

        --��ĿԴ�ļ�·��
        files (merge(framework,cvct))

        --���ӿ�����
        --���ӿ���
        links {gdali,mpii,mpiixx}
        --���ӿ�·��
        libdirs {gdallib,mpilib}
        --���·��
        includedirs {gdalinc,mpiinc}

-----------------------------------------

    --����ͬ��
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
            "src/geoalgorithm.cgrd2gdal.cpp"
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

