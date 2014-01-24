CNSDTFL
==================
CNSDTFL is China National Spatial Data Transfer Format Library. Main goal will implement that China spatial data formats convert to international spatial data formats, such as ESRI Shapefile, mapinfo, AutoCAD dwg, Edars Image, Geotiff ,etc, by GDAL/OGR. Now, it supports reading China vector data, China grid data, China image data, not supports writing. The three spatial data formats come from CNSDTF. If you want to know more about CNSDTF, please look [1] and [2]. And this library runs in Linux and Win.

Note:  
*require: GDAL/OGR >1.8.0*  
*Scons or Premake4*
************

About CNSDTF
============
CNSDTF is China National Spatial Data Transfer Format.

China Vector
============
CVCT is China vector data.
CVCT 1.0 is from CNSDTF[1] and CVCT 2.0 is from CNSDTF[2]. 

Note:  
Now support CVCT 1.0, but not support CSR 1.0, so data you read from VCT don't include information of spatial reference system. If you want to add srs, please project it by youself using other tools, such as ArcGIS. And not support CVCT 2.0.

China Grid
==========
CGRD is China grid data that also has CGRD 1.0 and CGRD 2.0.  

Note: 
support CGRD 1.0  
not support CSR 1.0
not support CGRD 2.0  

China Image
===========
CIMG is China image data that also has CIMG 1.0 and CIMG 2.0.  

Note:  
support CIMG 1.0  
not support CSR 1.0  
not support CIMG 2.0  

China Spatial Reference System
==============================
CSR is China coordiate system that also has CSR 1.0 and CSR 2.0.   

Note:  
support CSR 2.0  
not support CSR 1.0  

Install
=======
1.
2.
3.
4.
5.

[1]CSNDTF 1.0 is China Standard GB/T 17798-1999  
[2]CSNDTF 2.0 is China Standard GB/T 17798-2007  
[3]GDAL/OGR http://www.gdal.org  
