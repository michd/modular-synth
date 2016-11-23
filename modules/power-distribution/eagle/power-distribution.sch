<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="7.5.0">
<drawing>
<settings>
<setting alwaysvectorfont="yes"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="6" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="con-molex">
<description>&lt;b&gt;Molex Connectors&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="22-23-2031">
<description>.100" (2.54mm) Center Header - 3 Pin</description>
<wire x1="-3.81" y1="3.175" x2="3.81" y2="3.175" width="0.254" layer="21"/>
<wire x1="3.81" y1="3.175" x2="3.81" y2="1.27" width="0.254" layer="21"/>
<wire x1="3.81" y1="1.27" x2="3.81" y2="-3.175" width="0.254" layer="21"/>
<wire x1="3.81" y1="-3.175" x2="-3.81" y2="-3.175" width="0.254" layer="21"/>
<wire x1="-3.81" y1="-3.175" x2="-3.81" y2="1.27" width="0.254" layer="21"/>
<wire x1="-3.81" y1="1.27" x2="-3.81" y2="3.175" width="0.254" layer="21"/>
<wire x1="-3.81" y1="1.27" x2="3.81" y2="1.27" width="0.254" layer="21"/>
<pad name="1" x="-2.54" y="0" drill="1" shape="long" rot="R90"/>
<pad name="2" x="0" y="0" drill="1" shape="long" rot="R90"/>
<pad name="3" x="2.54" y="0" drill="1" shape="long" rot="R90"/>
<text x="-3.81" y="3.81" size="1.016" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.81" y="-5.08" size="1.016" layer="27" ratio="10">&gt;VALUE</text>
</package>
</packages>
<symbols>
<symbol name="MV">
<wire x1="1.27" y1="0" x2="0" y2="0" width="0.6096" layer="94"/>
<text x="2.54" y="-0.762" size="1.524" layer="95">&gt;NAME</text>
<text x="-0.762" y="1.397" size="1.778" layer="96">&gt;VALUE</text>
<pin name="S" x="-2.54" y="0" visible="off" length="short" direction="pas"/>
</symbol>
<symbol name="M">
<wire x1="1.27" y1="0" x2="0" y2="0" width="0.6096" layer="94"/>
<text x="2.54" y="-0.762" size="1.524" layer="95">&gt;NAME</text>
<pin name="S" x="-2.54" y="0" visible="off" length="short" direction="pas"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="22-23-2031" prefix="X">
<description>.100" (2.54mm) Center Header - 3 Pin</description>
<gates>
<gate name="-1" symbol="MV" x="0" y="2.54" addlevel="always" swaplevel="1"/>
<gate name="-2" symbol="M" x="0" y="0" addlevel="always" swaplevel="1"/>
<gate name="-3" symbol="M" x="0" y="-2.54" addlevel="always" swaplevel="1"/>
</gates>
<devices>
<device name="" package="22-23-2031">
<connects>
<connect gate="-1" pin="S" pad="1"/>
<connect gate="-2" pin="S" pad="2"/>
<connect gate="-3" pin="S" pad="3"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="MOLEX" constant="no"/>
<attribute name="MPN" value="22-23-2031" constant="no"/>
<attribute name="OC_FARNELL" value="1462950" constant="no"/>
<attribute name="OC_NEWARK" value="30C0862" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="supply2">
<description>&lt;b&gt;Supply Symbols&lt;/b&gt;&lt;p&gt;
GND, VCC, 0V, +5V, -5V, etc.&lt;p&gt;
Please keep in mind, that these devices are necessary for the
automatic wiring of the supply signals.&lt;p&gt;
The pin name defined in the symbol is identical to the net which is to be wired automatically.&lt;p&gt;
In this library the device names are the same as the pin names of the symbols, therefore the correct signal names appear next to the supply symbols in the schematic.&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
</packages>
<symbols>
<symbol name="VCC">
<circle x="0" y="1.27" radius="1.27" width="0.254" layer="94"/>
<text x="-1.905" y="3.175" size="1.778" layer="96">&gt;VALUE</text>
<pin name="VCC" x="0" y="-2.54" visible="off" length="short" direction="sup" rot="R90"/>
</symbol>
<symbol name="VEE">
<circle x="0" y="1.27" radius="1.27" width="0.254" layer="94"/>
<text x="-1.905" y="3.175" size="1.778" layer="96">&gt;VALUE</text>
<pin name="VEE" x="0" y="-2.54" visible="off" length="short" direction="sup" rot="R90"/>
</symbol>
<symbol name="GND">
<wire x1="-1.27" y1="0" x2="1.27" y2="0" width="0.254" layer="94"/>
<wire x1="1.27" y1="0" x2="0" y2="-1.27" width="0.254" layer="94"/>
<wire x1="0" y1="-1.27" x2="-1.27" y2="0" width="0.254" layer="94"/>
<text x="-1.905" y="-3.175" size="1.778" layer="96">&gt;VALUE</text>
<pin name="GND" x="0" y="2.54" visible="off" length="short" direction="sup" rot="R270"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="VCC" prefix="SUPPLY">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="VCC" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="VEE" prefix="SUPPLY">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="VEE" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="GND" prefix="SUPPLY">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="GND" symbol="GND" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="con-wago-500">
<description>&lt;b&gt;Wago Screw Clamps&lt;/b&gt;&lt;p&gt;
Grid 5.00 mm&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="W237-103">
<description>&lt;b&gt;WAGO SCREW CLAMP&lt;/b&gt;</description>
<wire x1="4.093" y1="-2.255" x2="5.897" y2="-0.299" width="0.254" layer="51"/>
<wire x1="-0.911" y1="-2.331" x2="0.994" y2="-0.299" width="0.254" layer="51"/>
<wire x1="-5.991" y1="-2.306" x2="-3.984" y2="-0.299" width="0.254" layer="51"/>
<wire x1="-7.489" y1="-5.481" x2="7.497" y2="-5.481" width="0.1524" layer="21"/>
<wire x1="7.497" y1="3.714" x2="7.497" y2="3.511" width="0.1524" layer="21"/>
<wire x1="7.497" y1="3.714" x2="-7.489" y2="3.714" width="0.1524" layer="21"/>
<wire x1="-7.489" y1="-5.481" x2="-7.489" y2="-3.093" width="0.1524" layer="21"/>
<wire x1="-7.489" y1="-3.093" x2="-5.889" y2="-3.093" width="0.1524" layer="21"/>
<wire x1="-5.889" y1="-3.093" x2="-4.111" y2="-3.093" width="0.1524" layer="51"/>
<wire x1="-4.111" y1="-3.093" x2="-0.885" y2="-3.093" width="0.1524" layer="21"/>
<wire x1="0.893" y1="-3.093" x2="4.119" y2="-3.093" width="0.1524" layer="21"/>
<wire x1="5.897" y1="-3.093" x2="7.497" y2="-3.093" width="0.1524" layer="21"/>
<wire x1="-7.489" y1="-3.093" x2="-7.489" y2="3.511" width="0.1524" layer="21"/>
<wire x1="7.497" y1="-3.093" x2="7.497" y2="-5.481" width="0.1524" layer="21"/>
<wire x1="7.497" y1="3.511" x2="-7.489" y2="3.511" width="0.1524" layer="21"/>
<wire x1="7.497" y1="3.511" x2="7.497" y2="-3.093" width="0.1524" layer="21"/>
<wire x1="-7.489" y1="3.511" x2="-7.489" y2="3.714" width="0.1524" layer="21"/>
<wire x1="-0.885" y1="-3.093" x2="0.893" y2="-3.093" width="0.1524" layer="51"/>
<wire x1="4.119" y1="-3.093" x2="5.897" y2="-3.093" width="0.1524" layer="51"/>
<circle x="-5" y="-1.29" radius="1.4986" width="0.1524" layer="51"/>
<circle x="5.0076" y="-1.29" radius="1.4986" width="0.1524" layer="51"/>
<circle x="-5" y="2.1898" radius="0.508" width="0.1524" layer="21"/>
<circle x="5.0076" y="2.1898" radius="0.508" width="0.1524" layer="21"/>
<circle x="0.0038" y="-1.29" radius="1.4986" width="0.1524" layer="51"/>
<circle x="0.0038" y="2.1898" radius="0.508" width="0.1524" layer="21"/>
<pad name="1" x="-5" y="-1.29" drill="1.1938" shape="long" rot="R90"/>
<pad name="2" x="0" y="-1.29" drill="1.1938" shape="long" rot="R90"/>
<pad name="3" x="5" y="-1.29" drill="1.1938" shape="long" rot="R90"/>
<text x="-6.905" y="0.615" size="1.27" layer="51" ratio="10">1</text>
<text x="-1.8504" y="0.5642" size="1.27" layer="51" ratio="10">2</text>
<text x="3.1534" y="0.615" size="1.27" layer="51" ratio="10">3</text>
<text x="-5.3048" y="-4.9476" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<text x="-5.6858" y="-7.4622" size="1.27" layer="25" ratio="10">&gt;NAME</text>
</package>
</packages>
<symbols>
<symbol name="KL">
<circle x="1.27" y="0" radius="1.27" width="0.254" layer="94"/>
<text x="0" y="0.889" size="1.778" layer="95" rot="R180">&gt;NAME</text>
<pin name="KL" x="5.08" y="0" visible="off" length="short" direction="pas" rot="R180"/>
</symbol>
<symbol name="KL+V">
<circle x="1.27" y="0" radius="1.27" width="0.254" layer="94"/>
<text x="-2.54" y="-3.683" size="1.778" layer="96">&gt;VALUE</text>
<text x="0" y="0.889" size="1.778" layer="95" rot="R180">&gt;NAME</text>
<pin name="KL" x="5.08" y="0" visible="off" length="short" direction="pas" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="W237-103" prefix="X" uservalue="yes">
<description>&lt;b&gt;WAGO SCREW CLAMP&lt;/b&gt;</description>
<gates>
<gate name="-1" symbol="KL" x="0" y="5.08" addlevel="always"/>
<gate name="-2" symbol="KL" x="0" y="0" addlevel="always"/>
<gate name="-3" symbol="KL+V" x="0" y="-5.08" addlevel="always"/>
</gates>
<devices>
<device name="" package="W237-103">
<connects>
<connect gate="-1" pin="KL" pad="1"/>
<connect gate="-2" pin="KL" pad="2"/>
<connect gate="-3" pin="KL" pad="3"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="237-103" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="18M7116" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="X6" library="con-molex" deviceset="22-23-2031" device=""/>
<part name="SUPPLY4" library="supply2" deviceset="VCC" device=""/>
<part name="SUPPLY5" library="supply2" deviceset="VEE" device=""/>
<part name="SUPPLY6" library="supply2" deviceset="GND" device=""/>
<part name="X7" library="con-molex" deviceset="22-23-2031" device=""/>
<part name="X8" library="con-molex" deviceset="22-23-2031" device=""/>
<part name="X9" library="con-molex" deviceset="22-23-2031" device=""/>
<part name="X10" library="con-molex" deviceset="22-23-2031" device=""/>
<part name="X1" library="con-molex" deviceset="22-23-2031" device=""/>
<part name="SUPPLY1" library="supply2" deviceset="VCC" device=""/>
<part name="SUPPLY2" library="supply2" deviceset="VEE" device=""/>
<part name="SUPPLY3" library="supply2" deviceset="GND" device=""/>
<part name="X2" library="con-molex" deviceset="22-23-2031" device=""/>
<part name="X3" library="con-molex" deviceset="22-23-2031" device=""/>
<part name="X4" library="con-molex" deviceset="22-23-2031" device=""/>
<part name="X5" library="con-molex" deviceset="22-23-2031" device=""/>
<part name="X11" library="con-wago-500" deviceset="W237-103" device=""/>
<part name="SUPPLY7" library="supply2" deviceset="VCC" device=""/>
<part name="SUPPLY8" library="supply2" deviceset="GND" device=""/>
<part name="SUPPLY9" library="supply2" deviceset="VEE" device=""/>
<part name="X12" library="con-wago-500" deviceset="W237-103" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="X6" gate="-1" x="7.62" y="58.42" rot="MR270"/>
<instance part="X6" gate="-2" x="10.16" y="58.42" rot="MR270"/>
<instance part="X6" gate="-3" x="12.7" y="58.42" rot="MR270"/>
<instance part="SUPPLY4" gate="G$1" x="7.62" y="73.66"/>
<instance part="SUPPLY5" gate="G$1" x="-5.08" y="58.42" rot="R180"/>
<instance part="SUPPLY6" gate="GND" x="0" y="58.42"/>
<instance part="X7" gate="-1" x="17.78" y="58.42" rot="MR270"/>
<instance part="X7" gate="-2" x="20.32" y="58.42" rot="MR270"/>
<instance part="X7" gate="-3" x="22.86" y="58.42" rot="MR270"/>
<instance part="X8" gate="-1" x="27.94" y="58.42" rot="MR270"/>
<instance part="X8" gate="-2" x="30.48" y="58.42" rot="MR270"/>
<instance part="X8" gate="-3" x="33.02" y="58.42" rot="MR270"/>
<instance part="X9" gate="-1" x="38.1" y="58.42" rot="MR270"/>
<instance part="X9" gate="-2" x="40.64" y="58.42" rot="MR270"/>
<instance part="X9" gate="-3" x="43.18" y="58.42" rot="MR270"/>
<instance part="X10" gate="-1" x="48.26" y="58.42" rot="MR270"/>
<instance part="X10" gate="-2" x="50.8" y="58.42" rot="MR270"/>
<instance part="X10" gate="-3" x="53.34" y="58.42" rot="MR270"/>
<instance part="X1" gate="-1" x="7.62" y="17.78" rot="MR270"/>
<instance part="X1" gate="-2" x="10.16" y="17.78" rot="MR270"/>
<instance part="X1" gate="-3" x="12.7" y="17.78" rot="MR270"/>
<instance part="SUPPLY1" gate="G$1" x="7.62" y="33.02"/>
<instance part="SUPPLY2" gate="G$1" x="-5.08" y="17.78" rot="R180"/>
<instance part="SUPPLY3" gate="GND" x="0" y="17.78"/>
<instance part="X2" gate="-1" x="17.78" y="17.78" rot="MR270"/>
<instance part="X2" gate="-2" x="20.32" y="17.78" rot="MR270"/>
<instance part="X2" gate="-3" x="22.86" y="17.78" rot="MR270"/>
<instance part="X3" gate="-1" x="27.94" y="17.78" rot="MR270"/>
<instance part="X3" gate="-2" x="30.48" y="17.78" rot="MR270"/>
<instance part="X3" gate="-3" x="33.02" y="17.78" rot="MR270"/>
<instance part="X4" gate="-1" x="38.1" y="17.78" rot="MR270"/>
<instance part="X4" gate="-2" x="40.64" y="17.78" rot="MR270"/>
<instance part="X4" gate="-3" x="43.18" y="17.78" rot="MR270"/>
<instance part="X5" gate="-1" x="48.26" y="17.78" rot="MR270"/>
<instance part="X5" gate="-2" x="50.8" y="17.78" rot="MR270"/>
<instance part="X5" gate="-3" x="53.34" y="17.78" rot="MR270"/>
<instance part="X11" gate="-1" x="-53.34" y="63.5"/>
<instance part="X11" gate="-2" x="-53.34" y="58.42"/>
<instance part="X11" gate="-3" x="-53.34" y="53.34"/>
<instance part="SUPPLY7" gate="G$1" x="-45.72" y="68.58"/>
<instance part="SUPPLY8" gate="GND" x="-40.64" y="53.34"/>
<instance part="SUPPLY9" gate="G$1" x="-45.72" y="45.72" rot="R180"/>
<instance part="X12" gate="-1" x="-27.94" y="53.34" rot="R180"/>
<instance part="X12" gate="-2" x="-27.94" y="58.42" rot="R180"/>
<instance part="X12" gate="-3" x="-27.94" y="63.5" rot="R180"/>
</instances>
<busses>
</busses>
<nets>
<net name="VCC" class="0">
<segment>
<pinref part="SUPPLY4" gate="G$1" pin="VCC"/>
<pinref part="X6" gate="-1" pin="S"/>
<wire x1="7.62" y1="71.12" x2="7.62" y2="68.58" width="0.1524" layer="91"/>
<pinref part="X7" gate="-1" pin="S"/>
<wire x1="7.62" y1="68.58" x2="7.62" y2="60.96" width="0.1524" layer="91"/>
<wire x1="17.78" y1="60.96" x2="17.78" y2="68.58" width="0.1524" layer="91"/>
<wire x1="17.78" y1="68.58" x2="7.62" y2="68.58" width="0.1524" layer="91"/>
<junction x="7.62" y="68.58"/>
<wire x1="17.78" y1="68.58" x2="27.94" y2="68.58" width="0.1524" layer="91"/>
<pinref part="X8" gate="-1" pin="S"/>
<wire x1="27.94" y1="68.58" x2="27.94" y2="60.96" width="0.1524" layer="91"/>
<wire x1="27.94" y1="68.58" x2="38.1" y2="68.58" width="0.1524" layer="91"/>
<pinref part="X9" gate="-1" pin="S"/>
<wire x1="38.1" y1="68.58" x2="38.1" y2="60.96" width="0.1524" layer="91"/>
<pinref part="X10" gate="-1" pin="S"/>
<wire x1="38.1" y1="68.58" x2="48.26" y2="68.58" width="0.1524" layer="91"/>
<wire x1="48.26" y1="68.58" x2="48.26" y2="60.96" width="0.1524" layer="91"/>
<junction x="17.78" y="68.58"/>
<junction x="27.94" y="68.58"/>
<junction x="38.1" y="68.58"/>
</segment>
<segment>
<pinref part="SUPPLY1" gate="G$1" pin="VCC"/>
<pinref part="X1" gate="-1" pin="S"/>
<wire x1="7.62" y1="30.48" x2="7.62" y2="27.94" width="0.1524" layer="91"/>
<pinref part="X2" gate="-1" pin="S"/>
<wire x1="7.62" y1="27.94" x2="7.62" y2="20.32" width="0.1524" layer="91"/>
<wire x1="17.78" y1="20.32" x2="17.78" y2="27.94" width="0.1524" layer="91"/>
<wire x1="17.78" y1="27.94" x2="7.62" y2="27.94" width="0.1524" layer="91"/>
<junction x="7.62" y="27.94"/>
<wire x1="17.78" y1="27.94" x2="27.94" y2="27.94" width="0.1524" layer="91"/>
<pinref part="X3" gate="-1" pin="S"/>
<wire x1="27.94" y1="27.94" x2="27.94" y2="20.32" width="0.1524" layer="91"/>
<wire x1="27.94" y1="27.94" x2="38.1" y2="27.94" width="0.1524" layer="91"/>
<pinref part="X4" gate="-1" pin="S"/>
<wire x1="38.1" y1="27.94" x2="38.1" y2="20.32" width="0.1524" layer="91"/>
<pinref part="X5" gate="-1" pin="S"/>
<wire x1="38.1" y1="27.94" x2="48.26" y2="27.94" width="0.1524" layer="91"/>
<wire x1="48.26" y1="27.94" x2="48.26" y2="20.32" width="0.1524" layer="91"/>
<junction x="17.78" y="27.94"/>
<junction x="27.94" y="27.94"/>
<junction x="38.1" y="27.94"/>
</segment>
<segment>
<pinref part="X11" gate="-1" pin="KL"/>
<wire x1="-48.26" y1="63.5" x2="-45.72" y2="63.5" width="0.1524" layer="91"/>
<pinref part="SUPPLY7" gate="G$1" pin="VCC"/>
<wire x1="-45.72" y1="63.5" x2="-45.72" y2="66.04" width="0.1524" layer="91"/>
<pinref part="X12" gate="-3" pin="KL"/>
<wire x1="-33.02" y1="63.5" x2="-45.72" y2="63.5" width="0.1524" layer="91"/>
<junction x="-45.72" y="63.5"/>
</segment>
</net>
<net name="GND" class="0">
<segment>
<pinref part="X6" gate="-2" pin="S"/>
<pinref part="X7" gate="-2" pin="S"/>
<wire x1="10.16" y1="66.04" x2="10.16" y2="60.96" width="0.1524" layer="91"/>
<wire x1="20.32" y1="60.96" x2="20.32" y2="66.04" width="0.1524" layer="91"/>
<wire x1="20.32" y1="66.04" x2="10.16" y2="66.04" width="0.1524" layer="91"/>
<wire x1="20.32" y1="66.04" x2="30.48" y2="66.04" width="0.1524" layer="91"/>
<pinref part="X8" gate="-2" pin="S"/>
<wire x1="30.48" y1="66.04" x2="30.48" y2="60.96" width="0.1524" layer="91"/>
<wire x1="30.48" y1="66.04" x2="40.64" y2="66.04" width="0.1524" layer="91"/>
<pinref part="X9" gate="-2" pin="S"/>
<wire x1="40.64" y1="66.04" x2="40.64" y2="60.96" width="0.1524" layer="91"/>
<pinref part="X10" gate="-2" pin="S"/>
<wire x1="40.64" y1="66.04" x2="50.8" y2="66.04" width="0.1524" layer="91"/>
<wire x1="50.8" y1="66.04" x2="50.8" y2="60.96" width="0.1524" layer="91"/>
<junction x="20.32" y="66.04"/>
<junction x="30.48" y="66.04"/>
<junction x="40.64" y="66.04"/>
<pinref part="SUPPLY6" gate="GND" pin="GND"/>
<wire x1="10.16" y1="66.04" x2="0" y2="66.04" width="0.1524" layer="91"/>
<wire x1="0" y1="66.04" x2="0" y2="60.96" width="0.1524" layer="91"/>
<junction x="10.16" y="66.04"/>
</segment>
<segment>
<pinref part="X1" gate="-2" pin="S"/>
<pinref part="X2" gate="-2" pin="S"/>
<wire x1="10.16" y1="25.4" x2="10.16" y2="20.32" width="0.1524" layer="91"/>
<wire x1="20.32" y1="20.32" x2="20.32" y2="25.4" width="0.1524" layer="91"/>
<wire x1="20.32" y1="25.4" x2="10.16" y2="25.4" width="0.1524" layer="91"/>
<wire x1="20.32" y1="25.4" x2="30.48" y2="25.4" width="0.1524" layer="91"/>
<pinref part="X3" gate="-2" pin="S"/>
<wire x1="30.48" y1="25.4" x2="30.48" y2="20.32" width="0.1524" layer="91"/>
<wire x1="30.48" y1="25.4" x2="40.64" y2="25.4" width="0.1524" layer="91"/>
<pinref part="X4" gate="-2" pin="S"/>
<wire x1="40.64" y1="25.4" x2="40.64" y2="20.32" width="0.1524" layer="91"/>
<pinref part="X5" gate="-2" pin="S"/>
<wire x1="40.64" y1="25.4" x2="50.8" y2="25.4" width="0.1524" layer="91"/>
<wire x1="50.8" y1="25.4" x2="50.8" y2="20.32" width="0.1524" layer="91"/>
<junction x="20.32" y="25.4"/>
<junction x="30.48" y="25.4"/>
<junction x="40.64" y="25.4"/>
<pinref part="SUPPLY3" gate="GND" pin="GND"/>
<wire x1="10.16" y1="25.4" x2="0" y2="25.4" width="0.1524" layer="91"/>
<wire x1="0" y1="25.4" x2="0" y2="20.32" width="0.1524" layer="91"/>
<junction x="10.16" y="25.4"/>
</segment>
<segment>
<pinref part="X11" gate="-2" pin="KL"/>
<wire x1="-48.26" y1="58.42" x2="-40.64" y2="58.42" width="0.1524" layer="91"/>
<pinref part="SUPPLY8" gate="GND" pin="GND"/>
<wire x1="-40.64" y1="58.42" x2="-40.64" y2="55.88" width="0.1524" layer="91"/>
<pinref part="X12" gate="-2" pin="KL"/>
<wire x1="-33.02" y1="58.42" x2="-40.64" y2="58.42" width="0.1524" layer="91"/>
<junction x="-40.64" y="58.42"/>
</segment>
</net>
<net name="VEE" class="0">
<segment>
<pinref part="SUPPLY2" gate="G$1" pin="VEE"/>
<wire x1="-5.08" y1="20.32" x2="-5.08" y2="22.86" width="0.1524" layer="91"/>
<pinref part="X1" gate="-3" pin="S"/>
<pinref part="X2" gate="-3" pin="S"/>
<wire x1="12.7" y1="22.86" x2="12.7" y2="20.32" width="0.1524" layer="91"/>
<wire x1="22.86" y1="20.32" x2="22.86" y2="22.86" width="0.1524" layer="91"/>
<wire x1="22.86" y1="22.86" x2="12.7" y2="22.86" width="0.1524" layer="91"/>
<pinref part="X3" gate="-3" pin="S"/>
<wire x1="22.86" y1="22.86" x2="33.02" y2="22.86" width="0.1524" layer="91"/>
<wire x1="33.02" y1="22.86" x2="33.02" y2="20.32" width="0.1524" layer="91"/>
<pinref part="X4" gate="-3" pin="S"/>
<wire x1="33.02" y1="22.86" x2="43.18" y2="22.86" width="0.1524" layer="91"/>
<wire x1="43.18" y1="22.86" x2="43.18" y2="20.32" width="0.1524" layer="91"/>
<pinref part="X5" gate="-3" pin="S"/>
<wire x1="43.18" y1="22.86" x2="53.34" y2="22.86" width="0.1524" layer="91"/>
<wire x1="53.34" y1="22.86" x2="53.34" y2="20.32" width="0.1524" layer="91"/>
<junction x="22.86" y="22.86"/>
<junction x="33.02" y="22.86"/>
<junction x="43.18" y="22.86"/>
<wire x1="-5.08" y1="22.86" x2="12.7" y2="22.86" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="X11" gate="-3" pin="KL"/>
<wire x1="-48.26" y1="53.34" x2="-45.72" y2="53.34" width="0.1524" layer="91"/>
<pinref part="SUPPLY9" gate="G$1" pin="VEE"/>
<wire x1="-45.72" y1="53.34" x2="-45.72" y2="48.26" width="0.1524" layer="91"/>
<pinref part="X12" gate="-1" pin="KL"/>
<wire x1="-33.02" y1="53.34" x2="-35.56" y2="53.34" width="0.1524" layer="91"/>
<wire x1="-35.56" y1="53.34" x2="-35.56" y2="48.26" width="0.1524" layer="91"/>
<wire x1="-35.56" y1="48.26" x2="-43.18" y2="48.26" width="0.1524" layer="91"/>
<wire x1="-43.18" y1="48.26" x2="-43.18" y2="53.34" width="0.1524" layer="91"/>
<wire x1="-43.18" y1="53.34" x2="-45.72" y2="53.34" width="0.1524" layer="91"/>
<junction x="-45.72" y="53.34"/>
</segment>
<segment>
<pinref part="SUPPLY5" gate="G$1" pin="VEE"/>
<wire x1="-5.08" y1="60.96" x2="-5.08" y2="63.5" width="0.1524" layer="91"/>
<pinref part="X6" gate="-3" pin="S"/>
<pinref part="X7" gate="-3" pin="S"/>
<wire x1="12.7" y1="63.5" x2="12.7" y2="60.96" width="0.1524" layer="91"/>
<wire x1="22.86" y1="60.96" x2="22.86" y2="63.5" width="0.1524" layer="91"/>
<wire x1="22.86" y1="63.5" x2="12.7" y2="63.5" width="0.1524" layer="91"/>
<pinref part="X8" gate="-3" pin="S"/>
<wire x1="22.86" y1="63.5" x2="33.02" y2="63.5" width="0.1524" layer="91"/>
<wire x1="33.02" y1="63.5" x2="33.02" y2="60.96" width="0.1524" layer="91"/>
<pinref part="X9" gate="-3" pin="S"/>
<wire x1="33.02" y1="63.5" x2="43.18" y2="63.5" width="0.1524" layer="91"/>
<wire x1="43.18" y1="63.5" x2="43.18" y2="60.96" width="0.1524" layer="91"/>
<pinref part="X10" gate="-3" pin="S"/>
<wire x1="43.18" y1="63.5" x2="53.34" y2="63.5" width="0.1524" layer="91"/>
<wire x1="53.34" y1="63.5" x2="53.34" y2="60.96" width="0.1524" layer="91"/>
<junction x="22.86" y="63.5"/>
<junction x="33.02" y="63.5"/>
<junction x="43.18" y="63.5"/>
<wire x1="-5.08" y1="63.5" x2="12.7" y2="63.5" width="0.1524" layer="91"/>
<junction x="12.7" y="63.5"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
