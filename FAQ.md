FAQ
===

问题1：cvct，cgrd，cimg中不支持坐标系，为什么?
==============================================
由于历史原因，cvct,cgrd,cimg格式目前能找到的数据都是1999年标准的数据，而在1999年标准中，对投影这一块没有做很详细的规定，故没有做1999年标准中坐标系解析的实现。另外由于2007年标准对投影这一块做了详细的规定，所有对2007年标准中的坐标系信息，我们做了一些工作，实现了一个转换（这部分实现在代码中，实际上并没有使用）。

问题2：也就是说数据读出来，都是没有坐标信息的了？
================================================
是的，写程序时只有1999年标准有数据，而1999年标准中对坐标系这一块定义模糊，故写程序时解析几何数据和属性数据用得是1999年标准（有数据可测试），坐标信息都是unknown。

问题3：没有投影，怎么使用？
==========================
我建议你使用ArcGIS将转换完的数据，定义一个投影吧。如果你不知道原始投影是什么，我也不知道怎么办了。

问题4：为什么没有做2007年标准的工作？
====================================
因为找不到2007年标准的数据，没有数据，怎么做测试，怎么实现代码。如果你有2007年标准的数据，请用邮箱发给我，方便我做2007标准的解析工作。

问题5：总结一下，说的不清楚？
============================
好的，目前代码中，cvct，cgrd，cimg都是针对1999年标准，没有解析坐标信息。所以出来的数据都没有坐标信息。而对2007年标准暂时不支持（相信我，没有数据，光要投影是没有用的）。

问题6：尚未解决的问题
=====================
*  属性值中中文GBK转向UTF-8字符的问题，两种编码的字符数不一致，造成储存属性值的数组超出边界。
*  在写往postgres数据库时，要素类型不匹配，报错，不知道哪里有问题。